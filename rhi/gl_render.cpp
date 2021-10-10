// gl_render.cpp
//

#include "gl_render.h"
#include "image.h"
#include "gl_framebuffer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glew/glew.h"

#include <vector>
#include <memory>

#include <SDL_opengl.h>
#include <SDL_video.h>

SDL_GLContext glContext;
SDL_Window* glWindow;
int screen_width, screen_height;

int numTextures = 0;
byte* outputbuffer;
SDL_Surface *uiSurface; // hd texture surface.
int mainScreenBufferTex = 0;

int global_red = 255;
int global_green = 255;
int global_blue = 255;
int global_alpha = 255;

#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803

void GL_Backend_SetLightingParams(float* attributes, int numValues);
void GL_Backend_ToggleLightRender(bool toggle);
void GL_InitRCAS(void);
void GL_InitEASU(void);
void GL_Compute_SetRCAS(int displayWidth, int displayHeight);

static bool isLightRenderEnabled = false;

devilution::StormImage* gameDrawImage = nullptr;
static devilution::StormRenderTexture* gameRenderTexture = nullptr;

namespace devilution
{
	extern Uint16 gnScreenWidth;
	extern Uint16 gnScreenHeight;

	void app_fatal(const char* pszFmt, ...);

	SDL_Surface* DiabloUiSurface()
	{
		return uiSurface;
	}
}

void GL_Private_EnableLighting(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
	GL_Backend_ToggleLightRender(true);
}

void GL_Private_DisableLighting(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
	GL_Backend_ToggleLightRender(false);
}

void GL_Private_SetFSR(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
	GL_Compute_SetRCAS(gameDrawImage->Width(), gameDrawImage->Height());
}

void GL_Private_BindGameRenderTexture(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
	gameRenderTexture->MakeCurrent();
}

void GL_Private_DisableRenderTexture(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
	devilution::StormRenderTexture::BindNull();
}

void GL_BindLevelRenderTexture(void)
{
	if (gameRenderTexture == nullptr) {
		gameDrawImage = devilution::StormImage::AllocateSytemImage("__screenbuf", devilution::gnScreenWidth, devilution::gnScreenHeight);
		gameRenderTexture = new devilution::StormRenderTexture(gameDrawImage, nullptr);
	}

	ImGui::GetBackgroundDrawList()->AddCallback(GL_Private_BindGameRenderTexture, nullptr);
}

void GL_BindNullRenderTexture(void)
{
	ImGui::GetBackgroundDrawList()->AddCallback(GL_Private_DisableRenderTexture, nullptr);
}

void GL_SetFSR(void)
{
	ImGui::GetBackgroundDrawList()->AddCallback(GL_Private_SetFSR, nullptr);
}

void GL_ResetFSR(void)
{
	ImGui::GetBackgroundDrawList()->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
	ImGui::GetBackgroundDrawList()->AddCallback(GL_Private_DisableLighting, nullptr);
	isLightRenderEnabled = false;
}

void GL_SetColor(float r, float g, float b) {
	if (r < 0)
		r = 0;

	if (g < 0)
		g = 0;

	if (b < 0)
		b = 0;

	global_red = r;
	global_blue = b;
	global_green = g;
}

void GL_SetAlpha(float a)
{
	global_alpha = a;
}

void GL_SetSolidColorMode(bool solidColorsOnly)
{
	if (solidColorsOnly)
		global_alpha = 75;
	else
		global_alpha = 255;
}

/*
============
GL_Init
============
*/
void GL_Init(const char* name, void* sdl_window, HWND hwnd, int width, int height)
{
	screen_width = width;
	screen_height = height;
	glWindow = (SDL_Window*)sdl_window;
	glContext = SDL_GL_CreateContext(glWindow);
	if (glContext == NULL)
	{
		devilution::app_fatal("OpenGL context could not be created!");
	}

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
	// GL 3.2 Core + GLSL 150
	const char* glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

	SDL_GL_MakeCurrent(glWindow, glContext);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	glewInit();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(glWindow, glContext);
	ImGui_ImplOpenGL3_Init(glsl_version);

	outputbuffer = new byte[screen_width * screen_height * 4];
	memset(outputbuffer, 255, screen_width * screen_height * 4);
	mainScreenBufferTex = GL_CreateTexture2D(outputbuffer, width, height, 32);

	uiSurface = SDL_CreateRGBSurfaceWithFormat(SDL_SWSURFACE, width, height, 1, SDL_PIXELFORMAT_RGBA8888);

	GL_InitRCAS();

	GL_InitEASU();

	GL_BeginFrame();
}

unsigned int GL_CreateTexture2D(byte* data, int width, int height, int bpp)
{
	unsigned int texNum = 0;
	glGenTextures(1, &texNum);
	glBindTexture(GL_TEXTURE_2D, texNum);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	if (bpp != 32)
		devilution::app_fatal("Todo");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return texNum;
}

void GL_UpdateLights(float* lightParams, int numLights)
{
	GL_Backend_SetLightingParams(lightParams, numLights);
}

void GL_ResetForLevelChange(void)
{
	isLightRenderEnabled = true;
}

void GL_ToggleLighting(bool toggle)
{
	if (isLightRenderEnabled == toggle)
		return;

	isLightRenderEnabled = toggle;

	if (toggle)
		ImGui::GetBackgroundDrawList()->AddCallback(GL_Private_EnableLighting, nullptr);
	else
		ImGui::GetBackgroundDrawList()->AddCallback(GL_Private_DisableLighting, nullptr);
}

void GL_UploadTexture(unsigned int image, unsigned char* data, int width, int height, int bpp) {
	glBindTexture(GL_TEXTURE_2D, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void GL_DrawLine(float sx, float sy, float dx, float dy, float thickness) {
	ImVec2 mi(sx, sy);
	ImVec2 ma(dx, dy);

	ImGui::GetBackgroundDrawList()->AddLine(mi, ma, IM_COL32(global_red, global_green, global_blue, 255), thickness);
	GL_SetColor(255, 255, 255);
}

void GL_RenderImage(unsigned int image, int x, int y, int width, int height, int startx, int starty, float start_uvx, float start_uvy) {
	ImVec2 mi(x + startx, y);
	ImVec2 ma(x + width, y + height - starty); 

	ImVec2 startUV;
	startUV.x = start_uvx;
	startUV.y = 0;

	ImVec2 endUV;
	endUV.x = 1;
	endUV.y = 1.0f - start_uvy;

	ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)image, mi, ma, startUV, endUV, IM_COL32(global_red, global_green, global_blue, global_alpha));
}

void GL_RenderImageUpsideDown(unsigned int image, int x, int y, int width, int height, int startx, int starty, float start_uvx, float start_uvy) {
	ImVec2 mi(x + startx, y);
	ImVec2 ma(x + width, y + height - starty);

	ImVec2 startUV;
	startUV.x = start_uvx;
	startUV.y = 1.0f - start_uvy;

	ImVec2 endUV;
	endUV.x = 1;
	endUV.y = 0;

	ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)image, mi, ma, startUV, endUV, IM_COL32(global_red, global_green, global_blue, global_alpha));
}

void GL_RenderImageScaledUV(unsigned int image, int x, int y, int width, int height, int startx, int starty, int uvwidth, int uvheight, int imagewidth, int imageheight) {
	ImVec2 mi(x, y);
	ImVec2 ma(x + width, y + height);

	ImVec2 startUV, endUV;

	startUV.x = (float)startx / (float)imagewidth;
	startUV.y = (float)starty / (float)imageheight;

	endUV.x = startUV.x + ((float)uvwidth / (float)imagewidth);
	endUV.y = startUV.y + ((float)uvheight / (float)imageheight);

	ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)image, mi, ma, startUV, endUV, IM_COL32(global_red, global_green, global_blue, global_alpha));
}

void GL_BeginFrame(void) {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void GL_EndFrame(unsigned char*finalScreenBuffer, unsigned char*palette) {

	if (finalScreenBuffer)
	{
		if (palette)
		{
			for (int i = 0; i < screen_width * screen_height; i++)
			{
				outputbuffer[(i * 4) + 0] = palette[(finalScreenBuffer[i] * 4) + 0];
				outputbuffer[(i * 4) + 1] = palette[(finalScreenBuffer[i] * 4) + 1];
				outputbuffer[(i * 4) + 2] = palette[(finalScreenBuffer[i] * 4) + 2];
				outputbuffer[(i * 4) + 3] = 255;
			}

			GL_UploadTexture(mainScreenBufferTex, outputbuffer, screen_width, screen_height, 32);
		}
		else
		{
			for (int i = 0; i < screen_width * screen_height; i++)
			{
				outputbuffer[(i * 4) + 0] = finalScreenBuffer[(i * 4) + 0];
				outputbuffer[(i * 4) + 1] = finalScreenBuffer[(i * 4) + 1];
				outputbuffer[(i * 4) + 2] = finalScreenBuffer[(i * 4) + 2];
				outputbuffer[(i * 4) + 3] = finalScreenBuffer[(i * 4) + 3];
			}
			GL_UploadTexture(mainScreenBufferTex, outputbuffer, screen_width, screen_height, 32);
		}

		GL_RenderImage(mainScreenBufferTex, 0, 0, screen_width, screen_height);
	}

	// Rendering
	ImGui::Render();
	glViewport(0, 0, screen_width, screen_height);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(glWindow);
}

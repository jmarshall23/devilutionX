// gl_render.cpp
//

#include "gl_render.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_opengl3_loader.h"

#include <vector>
#include <memory>

#include <sdl2/SDL_opengl.h>
#include <sdl2/SDL_video.h>

SDL_GLContext glContext;
SDL_Window* glWindow;
int screen_width, screen_height;

unsigned int texture_table[4096];
int numTextures = 0;
byte* outputbuffer;
int mainScreenBufferTex = 0;

#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
namespace devilution
{
	void app_fatal(const char* pszFmt, ...);
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

	GL_BeginFrame();
}

unsigned int GL_CreateTexture2D(byte* data, int width, int height, int bpp)
{
	glGenTextures(1, &texture_table[numTextures]);
	glBindTexture(GL_TEXTURE_2D, texture_table[numTextures]);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	if (bpp != 32)
		devilution::app_fatal("Todo");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return numTextures++;
}

void GL_UploadTexture(unsigned int image, unsigned char* data, int width, int height, int bpp) {
	glBindTexture(GL_TEXTURE_2D, texture_table[image]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void GL_RenderImage(unsigned int image, int x, int y, int width, int height) {
	ImVec2 mi(x, y);
	ImVec2 ma(x + width, y + height);

	ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)texture_table[image], mi, ma);
}

void GL_BeginFrame(void) {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// First command is ALWAYS to render the main screen buffer texture, we will update the d3dtexture at the very end; this just adds this to the imgui command queue.
	GL_RenderImage(mainScreenBufferTex, 0, 0, screen_width, screen_height);

}

void GL_EndFrame(unsigned char*finalScreenBuffer, unsigned char*palette) {
	for (int i = 0; i < screen_width * screen_height; i++)
	{
		outputbuffer[(i * 4) + 0] = palette[(finalScreenBuffer[i] * 4) + 0];
		outputbuffer[(i * 4) + 1] = palette[(finalScreenBuffer[i] * 4) + 1];
		outputbuffer[(i * 4) + 2] = palette[(finalScreenBuffer[i] * 4) + 2];
		outputbuffer[(i * 4) + 3] = 255;
	}

	GL_UploadTexture(mainScreenBufferTex, outputbuffer, screen_width, screen_height, 32);

	// Rendering
	ImGui::Render();
	glViewport(0, 0, screen_width, screen_height);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(glWindow);
}

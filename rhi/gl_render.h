// gl_render.h
//

#include <windows.h>

void GL_Init(const char* name, void *sdl_window, HWND hwnd, int width, int height);

unsigned int GL_CreateTexture2D(byte* data, int width, int height, int bpp);

void GL_UploadTexture(unsigned int image, unsigned char* data, int width, int height, int bpp);

void GL_RenderImage(unsigned int image, int x, int y, int width, int height, int startx = 0, int starty = 0, float start_uvx = 0, float start_uvy = 0);
void GL_RenderImageScaledUV(unsigned int image, int x, int y, int width, int height, int startx, int starty, int uvwidth, int uvheight, int imagewidth, int imageheight);

void GL_BeginFrame(void);
void GL_EndFrame(unsigned char* finalScreenBuffer, unsigned char* palette);

void GL_SetColor(float r, float g, float b);
void GL_SetAlpha(float a);
void GL_SetSolidColorMode(bool solidColorsOnly);

void GL_DrawLine(float sx, float sy, float dx, float dy, float thickness);

void GL_UpdateLights(float* lightParams, int numLights);

void GL_ToggleLighting(bool toggle);
void GL_ResetForLevelChange(void);

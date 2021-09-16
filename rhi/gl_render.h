// gl_render.h
//

#include <windows.h>

void GL_Init(const char* name, void *sdl_window, HWND hwnd, int width, int height);

unsigned int GL_CreateTexture2D(byte* data, int width, int height, int bpp);

void GL_UploadTexture(unsigned int image, unsigned char* data, int width, int height, int bpp);

void GL_RenderImage(unsigned int image, int x, int y, int width, int height);

void GL_BeginFrame(void);
void GL_EndFrame(unsigned char* finalScreenBuffer, unsigned char* palette);

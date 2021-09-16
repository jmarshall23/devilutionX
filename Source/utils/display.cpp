#include "utils/display.h"

#ifdef __vita__
#include <psp2/power.h>
#endif

#ifdef __3DS__
#include "platform/ctr/display.hpp"
#endif

#include "DiabloUI/diabloui.h"
#include "control.h"
#include "controls/controller.h"
#include "controls/devices/game_controller.h"
#include "controls/devices/joystick.h"
#include "controls/devices/kbcontroller.h"
#include "controls/game_controls.h"
#include "options.h"
#include "utils/log.hpp"
#include "utils/sdl_wrap.h"

#include "../../rhi/gl_render.h"
#include <SDL2/SDL_syswm.h>

namespace devilution {

extern SDL_Surface *renderer_texture_surface; /** defined in dx.cpp */

Uint16 gnScreenWidth;
Uint16 gnScreenHeight;
Uint16 gnViewportHeight;

SDL_Surface mainScreenSurface;

SDL_Surface *GetOutputSurface(void)
{
	return &mainScreenSurface;
}

bool IsFullScreen()
{
	return (SDL_GetWindowFlags(ghMainWnd) & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP)) != 0;
}

void AdjustToScreenGeometry(int width, int height)
{
	gnScreenWidth = width;
	gnScreenHeight = height;

	gnViewportHeight = gnScreenHeight;
	if (gnScreenWidth <= PANEL_WIDTH) {
		// Part of the screen is fully obscured by the UI
		gnViewportHeight -= PANEL_HEIGHT;
	}
}

void CalculatePreferdWindowSize(int &width, int &height)
{
	SDL_DisplayMode mode;
	if (SDL_GetDesktopDisplayMode(0, &mode) != 0) {
		ErrSdl();
	}

	if (!sgOptions.Graphics.bIntegerScaling) {
		float wFactor = (float)mode.w / width;
		float hFactor = (float)mode.h / height;

		if (wFactor > hFactor) {
			width = mode.w * height / mode.h;
		} else {
			height = mode.h * width / mode.w;
		}
		return;
	}

	int wFactor = mode.w / width;
	int hFactor = mode.h / height;

	if (wFactor > hFactor) {
		width = mode.w / hFactor;
		height = mode.h / hFactor;
	} else {
		width = mode.w / wFactor;
		height = mode.h / wFactor;
	}
}

bool SpawnWindow(const char *lpWindowName)
{
	// The default WASAPI backend causes distortions
	// https://github.com/diasurgical/devilutionX/issues/1434
	SDL_setenv("SDL_AUDIODRIVER", "winmm", /*overwrite=*/false);

	int initFlags = SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO;
	initFlags |= SDL_INIT_GAMECONTROLLER;

	SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
	if (SDL_Init(initFlags) <= -1) {
		ErrSdl();
	}

	if (sgOptions.Controller.szMapping[0] != '\0') {
		SDL_GameControllerAddMapping(sgOptions.Controller.szMapping);
	}

	int width = sgOptions.Graphics.nWidth;
	int height = sgOptions.Graphics.nHeight;

	if (sgOptions.Graphics.bUpscale && sgOptions.Graphics.bFitToScreen) {
		CalculatePreferdWindowSize(width, height);
	}
	AdjustToScreenGeometry(width, height);

	int flags = 0;
	if (!gbForceWindowed && sgOptions.Graphics.bFullscreen) {
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	if (sgOptions.Gameplay.bGrabInput) {
		flags |= SDL_WINDOW_INPUT_GRABBED;
	}

	flags |= SDL_WINDOW_OPENGL;

	ghMainWnd = SDL_CreateWindow(lpWindowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
	if (ghMainWnd == nullptr) {
		ErrSdl();
	}

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(ghMainWnd, &wmInfo);

	GL_Init(lpWindowName, ghMainWnd, wmInfo.info.win.window, width, height);

	mainScreenSurface.pitch = width;
	mainScreenSurface.w = width;
	mainScreenSurface.h = height;
	mainScreenSurface.pixels = malloc(width * height);

	int refreshRate = 60;
	SDL_DisplayMode mode;
	SDL_GetDisplayMode(0, 0, &mode);
	if (mode.refresh_rate != 0) {
		refreshRate = mode.refresh_rate;
	}
	refreshDelay = 1000000 / refreshRate;

	return ghMainWnd != nullptr;
}

} // namespace devilution

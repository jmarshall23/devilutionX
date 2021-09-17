#pragma once

#include <cstdint>

#include "utils/sdl_ptrs.h"


namespace devilution {

struct Art {
	class StormImage *image;
	int frames;
	int logical_width;
	int frame_height;
	unsigned int palette_version;

	Art()
	{
		image = NULL;
		frames = 1;
		logical_width = 0;
		frame_height = 0; // logical frame height (before scaling)
		palette_version = 0;
	}

	int w() const
	{
		return logical_width;
	}

	int h() const
	{
		return frame_height;
	}

	void Unload()
	{
		image = nullptr;
	}
};

void LoadArt(const char *pszFile, Art *art, int frames = 1, SDL_Color *pPalette = NULL);
void LoadMaskedArt(const char *pszFile, Art *art, int frames = 1, int mask = 250);

} // namespace devilution

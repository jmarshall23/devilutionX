#include "DiabloUI/art.h"

#include <cstddef>
#include <cstdint>
#include <memory>

#include "storm/storm.h"
#include "utils/display.h"
#include "utils/log.hpp"
#include "utils/sdl_compat.h"
#include "utils/sdl_wrap.h"
#include "../engine/load_file.hpp"
#include "../rhi/image.h"

namespace devilution {

void LoadMaskedArt(const char *pszFile, Art *art, int frames, int mask)
{
	LoadArt(pszFile, art, frames);
	//if (art->surface != nullptr)
	//	SDLC_SetColorKey(art->surface, mask);
}

void LoadArt(const char *pszFile, Art *art, int frames, SDL_Color *pPalette)
{
	if (art == nullptr || art->image != nullptr)
		return;

	art->frames = frames;

	art->image = StormImage::LoadImageSequence(pszFile, false, false);

	art->logical_width = art->image->Width();
	art->frame_height = art->image->Height() / frames;
}

} // namespace devilution

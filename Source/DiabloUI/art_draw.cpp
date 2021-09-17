#include "DiabloUI/art_draw.h"

#include "DiabloUI/diabloui.h"
#include "utils/display.h"
#include "utils/sdl_compat.h"

#include "../../rhi/gl_render.h"
#include "../../rhi/image.h"

namespace devilution {

void DrawArt(Point screenPosition, Art *art, int nFrame, Uint16 srcW, Uint16 srcH)
{
	if (screenPosition.y >= gnScreenHeight || screenPosition.x >= gnScreenWidth || art->image == nullptr)
		return;

	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = nFrame * art->h();
	srcRect.w = art->w();
	srcRect.h = art->h();

	ScaleOutputRect(&srcRect);

	if (srcW != 0 && srcW < srcRect.w)
		srcRect.w = srcW;
	if (srcH != 0 && srcH < srcRect.h)
		srcRect.h = srcH;
	SDL_Rect dstRect = MakeSdlRect(screenPosition.x, screenPosition.y, srcRect.w, srcRect.h);
	ScaleOutputRect(&dstRect);

	Surface temp;
	art->image->Draw2D(1, dstRect.x, dstRect.y, dstRect.w, dstRect.h, srcRect.x, srcRect.y, srcRect.w, srcRect.h);
}

void DrawArt(const Surface &out, Point screenPosition, Art *art, int nFrame, Uint16 srcW, Uint16 srcH)
{
	if (screenPosition.y >= gnScreenHeight || screenPosition.x >= gnScreenWidth || art->image == nullptr)
		return;

	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = nFrame * art->h();
	srcRect.w = art->w();
	srcRect.h = art->h();

	if (srcW != 0 && srcW < srcRect.w)
		srcRect.w = srcW;
	if (srcH != 0 && srcH < srcRect.h)
		srcRect.h = srcH;
	SDL_Rect dstRect = MakeSdlRect(screenPosition.x, screenPosition.y, srcRect.w, srcRect.h);

	Surface temp;
	art->image->Draw2D(1, dstRect.x, dstRect.y, dstRect.w, dstRect.h, srcRect.x, srcRect.y, srcRect.w, srcRect.h);
}

void DrawAnimatedArt(Art *art, Point screenPosition)
{
	DrawArt(screenPosition, art, GetAnimationFrame(art->frames));
}

int GetAnimationFrame(int frames, int fps)
{
	int frame = (SDL_GetTicks() / fps) % frames;

	return frame > frames ? 0 : frame;
}

} // namespace devilution

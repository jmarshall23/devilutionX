#include "hwcursor.hpp"

#include <cstdint>
#include <tuple>

#if SDL_VERSION_ATLEAST(2, 0, 0)
#include <SDL_mouse.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#endif

#include "DiabloUI/diabloui.h"

#include "appfat.h"
#include "cursor.h"
#include "engine.h"
#include "utils/display.h"
#include "utils/sdl_bilinear_scale.hpp"
#include "utils/sdl_wrap.h"

#include "../rhi/image.h"

namespace devilution {
namespace {
CursorInfo CurrentCursorInfo;

#if SDL_VERSION_ATLEAST(2, 0, 0)
SDLCursorUniquePtr CurrentCursor;

enum class HotpointPosition {
	TopLeft,
	Center,
};

Size ScaledSize(Size size)
{
	if (renderer != nullptr) {
		float scaleX;
		float scaleY;
		SDL_RenderGetScale(renderer, &scaleX, &scaleY);
		size.width = static_cast<int>(size.width * scaleX);
		size.height = static_cast<int>(size.height * scaleY);
	}
	return size;
}

bool IsCursorSizeAllowed(Size size)
{
	if (sgOptions.Graphics.nHardwareCursorMaxSize <= 0)
		return true;
	size = ScaledSize(size);
	return size.width <= sgOptions.Graphics.nHardwareCursorMaxSize
	    && size.height <= sgOptions.Graphics.nHardwareCursorMaxSize;
}

Point GetHotpointPosition(const SDL_Surface &surface, HotpointPosition position)
{
	switch (position) {
	case HotpointPosition::TopLeft:
		return { 0, 0 };
	case HotpointPosition::Center:
		return { surface.w / 2, surface.h / 2 };
	}
	app_fatal("Unhandled enum value");
}

bool ShouldUseBilinearScaling()
{
	return sgOptions.Graphics.szScaleQuality[0] != '0';
}

bool SetHardwareCursor(SDL_Surface *surface, HotpointPosition hotpointPosition)
{
	SDLCursorUniquePtr newCursor;
	const Size size { surface->w, surface->h };
	const Size scaledSize = ScaledSize(size);

	if (size == scaledSize) {
		const Point hotpoint = GetHotpointPosition(*surface, hotpointPosition);
		newCursor = SDLCursorUniquePtr { SDL_CreateColorCursor(surface, hotpoint.x, hotpoint.y) };
	} else {
		// SDL does not support BlitScaled from 8-bit to RGBA.
		SDLSurfaceUniquePtr converted { SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0) };

		SDLSurfaceUniquePtr scaledSurface = SDLWrap::CreateRGBSurfaceWithFormat(0, scaledSize.width, scaledSize.height, 32, SDL_PIXELFORMAT_ARGB8888);
		if (ShouldUseBilinearScaling()) {
			BilinearScale32(converted.get(), scaledSurface.get());
		} else {
			SDL_BlitScaled(converted.get(), nullptr, scaledSurface.get(), nullptr);
		}
		const Point hotpoint = GetHotpointPosition(*scaledSurface, hotpointPosition);
		newCursor = SDLCursorUniquePtr { SDL_CreateColorCursor(scaledSurface.get(), hotpoint.x, hotpoint.y) };
	}
	if (newCursor == nullptr)
		return false;
	SDL_SetCursor(newCursor.get());
	CurrentCursor = std::move(newCursor);
	return true;
}

bool SetHardwareCursorFromSprite(int pcurs)
{
	const bool isItem = IsItemSprite(pcurs);
	if (isItem && !sgOptions.Graphics.bHardwareCursorForItems)
		return false;

	const int outlineWidth = isItem ? 1 : 0;

	const ImageFrame_t *imageFrame = GetInvItemSprite(pcurs);

	Size size = { imageFrame->width, imageFrame->height };
	size.width += 2 * outlineWidth;
	size.height += 2 * outlineWidth;

	if (!IsCursorSizeAllowed(size))
		return false;

	OwnedSurface out { size };
	SDL_SetSurfacePalette(out.surface, Palette);

	// Transparent color must not be used in the sprite itself.
	// Colors 1-127 are outside of the UI palette so are safe to use.
	constexpr std::uint8_t TransparentColor = 255;
	SDL_FillRect(out.surface, nullptr, TransparentColor);
	SDL_SetColorKey(out.surface, 1, TransparentColor);
	//CelDrawCursor(out, { outlineWidth, size.height - outlineWidth }, pcurs);


	const bool result = SetHardwareCursor(out.surface, isItem ? HotpointPosition::Center : HotpointPosition::TopLeft);
	return result;
}
#endif

} // namespace

CursorInfo GetCurrentCursorInfo()
{
	return CurrentCursorInfo;
}

void SetHardwareCursor(CursorInfo cursorInfo)
{
	if (ArtCursor.image == nullptr) {
		return;
	}

	if (!CurrentCursorInfo.Enabled())
		return;

	int x, y;
	SDL_PumpEvents();
	SDL_GetMouseState(&x, &y);
	Surface temp;

	if (cursorInfo.type() == CursorType::UserInterface) {
		const bool isItem = IsItemSprite(pcurs);
		if (isItem && !sgOptions.Graphics.bHardwareCursorForItems)
			return;

		const int outlineWidth = isItem ? 1 : 0;

		RenderItemSprite(temp, pcurs, x, y);

		return;
	} else if (cursorInfo.type() == CursorType::UserInterface) {
		ArtCursor.image->ClipRenderNoLighting(temp, x, y, 1);
	}
	/*
	CurrentCursorInfo = cursorInfo;

	
	

	switch (cursorInfo.type()) {
	case CursorType::Game:
		CurrentCursorInfo.SetEnabled(SetHardwareCursorFromSprite(cursorInfo.id()));
		break;
	case CursorType::UserInterface:
		// ArtCursor is null while loading the game on the progress screen,
		// called via palette fade from ShowProgress.
		if (ArtCursor.image != nullptr) {
			CurrentCursorInfo.SetEnabled(
			    IsCursorSizeAllowed(Size { ArtCursor.surface->w, ArtCursor.surface->h })
			    && SetHardwareCursor(ArtCursor.surface, HotpointPosition::TopLeft));
		}
		break;
	case CursorType::Unknown:
		CurrentCursorInfo.SetEnabled(false);
		break;
	}
	if (!CurrentCursorInfo.Enabled())
		SetHardwareCursorVisible(false);
	*/
}

} // namespace devilution

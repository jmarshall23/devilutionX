/**
 * @file dun_render.cpp
 *
 * Implementation of functionality for rendering the level tiles.
 */
#include "engine/render/dun_render.hpp"

#include <algorithm>
#include <climits>
#include <cstdint>

#include "lighting.h"
#include "options.h"
#include "utils/attributes.h"

namespace devilution {

namespace {


/** Width of a tile rendering primitive. */
constexpr std::int_fast16_t Width = TILE_WIDTH / 2;

/** Height of a tile rendering primitive (except triangles). */
constexpr std::int_fast16_t Height = TILE_HEIGHT;

/** Height of the lower triangle of a triangular or a trapezoid tile. */
constexpr std::int_fast16_t LowerHeight = TILE_HEIGHT / 2;

/** Height of the upper triangle of a triangular tile. */
constexpr std::int_fast16_t TriangleUpperHeight = TILE_HEIGHT / 2 - 1;

/** Height of the upper rectangle of a trapezoid tile. */
constexpr std::int_fast16_t TrapezoidUpperHeight = TILE_HEIGHT / 2;

constexpr std::int_fast16_t TriangleHeight = LowerHeight + TriangleUpperHeight;

/** For triangles, for each pixel drawn vertically, this many pixels are drawn horizontally. */
constexpr std::int_fast16_t XStep = 2;

// Debugging variables
// #define DEBUG_RENDER_COLOR
// #define DEBUG_RENDER_OFFSET_X 5
// #define DEBUG_RENDER_OFFSET_Y 5

#ifdef DEBUG_RENDER_COLOR
int DBGCOLOR = 0;

int GetTileDebugColor(TileType tile)
{
	// clang-format off
	switch (tile) {
		case TileType::Square: return PAL16_YELLOW + 5;
		case TileType::TransparentSquare: return PAL16_ORANGE + 5;
		case TileType::LeftTriangle: return PAL16_GRAY + 5;
		case TileType::RightTriangle: return PAL16_BEIGE;
		case TileType::LeftTrapezoid: return PAL16_RED + 5;
		case TileType::RightTrapezoid: return PAL16_BLUE + 5;
		default: return 0;
	}
	// clang-format on
}
#endif // DEBUG_RENDER_COLOR

/** Fully transparent variant of WallMask. */
const std::uint32_t WallMaskFullyTrasparent[TILE_HEIGHT] = {
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000
};
/** Transparent variant of RightMask. */
const std::uint32_t RightMaskTransparent[TILE_HEIGHT] = {
	0xC0000000,
	0xF0000000,
	0xFC000000,
	0xFF000000,
	0xFFC00000,
	0xFFF00000,
	0xFFFC0000,
	0xFFFF0000,
	0xFFFFC000,
	0xFFFFF000,
	0xFFFFFC00,
	0xFFFFFF00,
	0xFFFFFFC0,
	0xFFFFFFF0,
	0xFFFFFFFC,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF
};
/** Transparent variant of LeftMask. */
const std::uint32_t LeftMaskTransparent[TILE_HEIGHT] = {
	0x00000003,
	0x0000000F,
	0x0000003F,
	0x000000FF,
	0x000003FF,
	0x00000FFF,
	0x00003FFF,
	0x0000FFFF,
	0x0003FFFF,
	0x000FFFFF,
	0x003FFFFF,
	0x00FFFFFF,
	0x03FFFFFF,
	0x0FFFFFFF,
	0x3FFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF
};
/** Specifies the draw masks used to render transparency of the right side of tiles. */
const std::uint32_t RightMask[TILE_HEIGHT] = {
	0xEAAAAAAA,
	0xF5555555,
	0xFEAAAAAA,
	0xFF555555,
	0xFFEAAAAA,
	0xFFF55555,
	0xFFFEAAAA,
	0xFFFF5555,
	0xFFFFEAAA,
	0xFFFFF555,
	0xFFFFFEAA,
	0xFFFFFF55,
	0xFFFFFFEA,
	0xFFFFFFF5,
	0xFFFFFFFE,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF
};
/** Specifies the draw masks used to render transparency of the left side of tiles. */
const std::uint32_t LeftMask[TILE_HEIGHT] = {
	0xAAAAAAAB,
	0x5555555F,
	0xAAAAAABF,
	0x555555FF,
	0xAAAAABFF,
	0x55555FFF,
	0xAAAABFFF,
	0x5555FFFF,
	0xAAABFFFF,
	0x555FFFFF,
	0xAABFFFFF,
	0x55FFFFFF,
	0xABFFFFFF,
	0x5FFFFFFF,
	0xBFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF
};
/** Specifies the draw masks used to render transparency of wall tiles. */
const std::uint32_t WallMask[TILE_HEIGHT] = {
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555,
	0xAAAAAAAA,
	0x55555555
};
/** Fully opaque mask */
const std::uint32_t SolidMask[TILE_HEIGHT] = {
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF
};
/** Used to mask out the left half of the tile diamond and only render additional content */
const std::uint32_t RightFoliageMask[TILE_HEIGHT] = {
	0xFFFFFFFF,
	0x3FFFFFFF,
	0x0FFFFFFF,
	0x03FFFFFF,
	0x00FFFFFF,
	0x003FFFFF,
	0x000FFFFF,
	0x0003FFFF,
	0x0000FFFF,
	0x00003FFF,
	0x00000FFF,
	0x000003FF,
	0x000000FF,
	0x0000003F,
	0x0000000F,
	0x00000003,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};
/** Used to mask out the left half of the tile diamond and only render additional content */
const std::uint32_t LeftFoliageMask[TILE_HEIGHT] = {
	0xFFFFFFFF,
	0xFFFFFFFC,
	0xFFFFFFF0,
	0xFFFFFFC0,
	0xFFFFFF00,
	0xFFFFFC00,
	0xFFFFF000,
	0xFFFFC000,
	0xFFFF0000,
	0xFFFC0000,
	0xFFF00000,
	0xFFC00000,
	0xFF000000,
	0xFC000000,
	0xF0000000,
	0xC0000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};

inline int CountLeadingZeros(std::uint32_t mask)
{
	// Note: This function assumes that the argument is not zero,
	// which means there is at least one bit set.
	static_assert(
	    sizeof(std::uint32_t) == sizeof(uint32_t),
	    "CountLeadingZeros: std::uint32_t must be 32bits");
#if defined(__GNUC__) || defined(__clang__)
	return __builtin_clz(mask);
#else
	// Count the number of leading zeros using binary search.
	int n = 0;
	if ((mask & 0xFFFF0000) == 0)
		n += 16, mask <<= 16;
	if ((mask & 0xFF000000) == 0)
		n += 8, mask <<= 8;
	if ((mask & 0xF0000000) == 0)
		n += 4, mask <<= 4;
	if ((mask & 0xC0000000) == 0)
		n += 2, mask <<= 2;
	if ((mask & 0x80000000) == 0)
		n += 1;
	return n;
#endif
}

template <typename F>
DVL_ALWAYS_INLINE DVL_ATTRIBUTE_HOT void ForEachSetBit(std::uint32_t mask, const F &f)
{
	int i = 0;
	while (mask != 0) {
		int z = CountLeadingZeros(mask);
		i += z, mask <<= z;
		for (; mask & 0x80000000; i++, mask <<= 1)
			f(i);
	}
}

enum class TransparencyType {
	Solid,
	Blended,
	Stippled,
};

enum class LightType {
	FullyDark,
	PartiallyLit,
	FullyLit,
};

template <LightType Light>
DVL_ALWAYS_INLINE DVL_ATTRIBUTE_HOT void RenderLineOpaque(std::uint8_t *dst, const std::uint8_t *src, std::uint_fast8_t n, const std::uint8_t *tbl)
{
	if (Light == LightType::FullyDark) {
		for (size_t i = 0; i < n; i++) {
			if (src[i] == 255)
				continue;

			dst[i] = 0;
		}
	} else if (Light == LightType::FullyLit) {
		for (size_t i = 0; i < n; i++) {
			if (src[i] == 255)
				continue;

			dst[i] = src[i];
		}
	} else { // Partially lit
		for (size_t i = 0; i < n; i++) {
			if (src[i] == 255)
				continue;
			dst[i] = tbl[src[i]];
		}
	}
}

template <LightType Light>
DVL_ALWAYS_INLINE DVL_ATTRIBUTE_HOT void RenderLineBlended(std::uint8_t *dst, const std::uint8_t *src, std::uint_fast8_t n, const std::uint8_t *tbl, std::uint32_t mask)
{
#ifndef DEBUG_RENDER_COLOR
	if (Light == LightType::FullyDark) {
		for (size_t i = 0; i < n; i++, mask <<= 1) {
			if ((mask & 0x80000000) != 0)
				dst[i] = 0;
			else
				dst[i] = paletteTransparencyLookup[0][dst[i]];
		}
	} else if (Light == LightType::FullyLit) {
		for (size_t i = 0; i < n; i++, mask <<= 1) {
			if ((mask & 0x80000000) != 0)
				dst[i] = src[i];
			else
				dst[i] = paletteTransparencyLookup[dst[i]][src[i]];
		}
	} else { // Partially lit
		for (size_t i = 0; i < n; i++, mask <<= 1) {
			if ((mask & 0x80000000) != 0)
				dst[i] = tbl[src[i]];
			else
				dst[i] = paletteTransparencyLookup[dst[i]][tbl[src[i]]];
		}
	}
#else
	for (size_t i = 0; i < n; i++, mask <<= 1) {
		if ((mask & 0x80000000) != 0)
			dst[i] = tbl[DBGCOLOR];
		else
			dst[i] = paletteTransparencyLookup[dst[i]][tbl[DBGCOLOR]];
	}
#endif
}

template <LightType Light>
DVL_ALWAYS_INLINE DVL_ATTRIBUTE_HOT void RenderLineStippled(std::uint8_t *dst, const std::uint8_t *src, const std::uint8_t *tbl, std::uint32_t mask)
{
	if (Light == LightType::FullyDark) {
		ForEachSetBit(mask, [=](int i) { dst[i] = 0; });
	} else if (Light == LightType::FullyLit) {
#ifndef DEBUG_RENDER_COLOR
		ForEachSetBit(mask, [=](int i) { dst[i] = src[i]; });
#else
		ForEachSetBit(mask, [=](int i) { dst[i] = DBGCOLOR; });
#endif
	} else { // Partially lit
		ForEachSetBit(mask, [=](int i) { dst[i] = tbl[src[i]]; });
	}
}

template <TransparencyType Transparency, LightType Light>
DVL_ALWAYS_INLINE DVL_ATTRIBUTE_HOT void RenderLine(std::uint8_t *dst, const std::uint8_t *src, std::uint_fast8_t n, const std::uint8_t *tbl)
{
	//if (Transparency == TransparencyType::Solid) {
	//	RenderLineOpaque<Light>(dst, src, n, tbl);
	//} else {
	//	// The number of iterations is limited by the size of the mask.
	//	// So we can limit it by ANDing the mask with another mask that only keeps
	//	// iterations that are lower than n. We can now avoid testing if i < n
	//	// at every loop iteration.
	//	assert(n != 0 && n <= sizeof(std::uint32_t) * CHAR_BIT);
	//	const std::uint32_t firstNOnes = std::uint32_t(-1) << ((sizeof(std::uint32_t) * CHAR_BIT) - n);
	//	mask &= firstNOnes;
	//	if (mask == firstNOnes) {
	//		RenderLineOpaque<Light>(dst, src, n, tbl);
	//	} else if (Transparency == TransparencyType::Blended) {
	//		RenderLineBlended<Light>(dst, src, n, tbl, mask);
	//	} else {
	//		RenderLineStippled<Light>(dst, src, tbl, mask);
	//	}
	//}

	RenderLineOpaque<Light>(dst, src, n, tbl);
}

struct Clip {
	std::int_fast16_t top;
	std::int_fast16_t bottom;
	std::int_fast16_t left;
	std::int_fast16_t right;
	std::int_fast16_t width;
	std::int_fast16_t height;
};

Clip CalculateClip(std::int_fast16_t x, std::int_fast16_t y, std::int_fast16_t w, std::int_fast16_t h, const Surface &out)
{
	Clip clip;
	clip.top = y + 1 < h ? h - (y + 1) : 0;
	clip.bottom = y + 1 > out.h() ? (y + 1) - out.h() : 0;
	clip.left = x < 0 ? -x : 0;
	clip.right = x + w > out.w() ? x + w - out.w() : 0;
	clip.width = w - clip.left - clip.right;
	clip.height = h - clip.top - clip.bottom;
	return clip;
}

/** Vertical clip for the lower and upper triangles of a diamond tile (L/RTRIANGLE).*/
struct DiamondClipY {
	std::int_fast16_t lowerBottom;
	std::int_fast16_t lowerTop;
	std::int_fast16_t upperBottom;
	std::int_fast16_t upperTop;
};

template <std::int_fast16_t UpperHeight = TriangleUpperHeight>
DiamondClipY CalculateDiamondClipY(const Clip &clip)
{
	DiamondClipY result;
	if (clip.bottom > LowerHeight) {
		result.lowerBottom = LowerHeight;
		result.upperBottom = clip.bottom - LowerHeight;
		result.lowerTop = result.upperTop = 0;
	} else if (clip.top > UpperHeight) {
		result.upperTop = UpperHeight;
		result.lowerTop = clip.top - UpperHeight;
		result.upperBottom = result.lowerBottom = 0;
	} else {
		result.upperTop = clip.top;
		result.lowerBottom = clip.bottom;
		result.lowerTop = result.upperBottom = 0;
	}
	return result;
}

std::size_t CalculateTriangleSourceSkipLowerBottom(std::int_fast16_t numLines)
{
	return XStep * numLines * (numLines + 1) / 2 + 2 * ((numLines + 1) / 2);
}

std::size_t CalculateTriangleSourceSkipUpperBottom(std::int_fast16_t numLines)
{
	return 2 * TriangleUpperHeight * numLines - numLines * (numLines - 1) + 2 * ((numLines + 1) / 2);
}

// Blit with left and vertical clipping.
void RenderBlackTileClipLeftAndVertical(std::uint8_t *dst, int dstPitch, int sx, DiamondClipY clipY)
{
	dst += XStep * (LowerHeight - clipY.lowerBottom - 1);
	// Lower triangle (drawn bottom to top):
	const auto lowerMax = LowerHeight - clipY.lowerTop;
	for (auto i = clipY.lowerBottom + 1; i <= lowerMax; ++i, dst -= dstPitch + XStep) {
		const auto w = 2 * XStep * i;
		const auto curX = sx + TILE_WIDTH / 2 - XStep * i;
		if (curX >= 0) {
			memset(dst, 0, w);
		} else if (-curX <= w) {
			memset(dst - curX, 0, w + curX);
		}
	}
	dst += 2 * XStep + XStep * clipY.upperBottom;
	// Upper triangle (drawn bottom to top):
	const auto upperMax = TriangleUpperHeight - clipY.upperTop;
	for (auto i = clipY.upperBottom; i < upperMax; ++i, dst -= dstPitch - XStep) {
		const auto w = 2 * XStep * (TriangleUpperHeight - i);
		const auto curX = sx + TILE_WIDTH / 2 - XStep * (TriangleUpperHeight - i);
		if (curX >= 0) {
			memset(dst, 0, w);
		} else if (-curX <= w) {
			memset(dst - curX, 0, w + curX);
		} else {
			break;
		}
	}
}

// Blit with right and vertical clipping.
void RenderBlackTileClipRightAndVertical(std::uint8_t *dst, int dstPitch, std::int_fast16_t maxWidth, DiamondClipY clipY)
{
	dst += XStep * (LowerHeight - clipY.lowerBottom - 1);
	// Lower triangle (drawn bottom to top):
	const auto lowerMax = LowerHeight - clipY.lowerTop;
	for (auto i = clipY.lowerBottom + 1; i <= lowerMax; ++i, dst -= dstPitch + XStep) {
		const auto width = 2 * XStep * i;
		const auto endX = TILE_WIDTH / 2 + XStep * i;
		const auto skip = endX > maxWidth ? endX - maxWidth : 0;
		if (width > skip)
			memset(dst, 0, width - skip);
	}
	dst += 2 * XStep + XStep * clipY.upperBottom;
	// Upper triangle (drawn bottom to top):
	const auto upperMax = TriangleUpperHeight - clipY.upperTop;
	for (auto i = 1 + clipY.upperBottom; i <= upperMax; ++i, dst -= dstPitch - XStep) {
		const auto width = TILE_WIDTH - 2 * XStep * i;
		const auto endX = TILE_WIDTH / 2 + XStep * (TriangleUpperHeight - i + 1);
		const auto skip = endX > maxWidth ? endX - maxWidth : 0;
		if (width <= skip)
			break;
		memset(dst, 0, width - skip);
	}
}

// Blit with vertical clipping only.
void RenderBlackTileClipY(std::uint8_t *dst, int dstPitch, DiamondClipY clipY)
{
	dst += XStep * (LowerHeight - clipY.lowerBottom - 1);
	// Lower triangle (drawn bottom to top):
	const auto lowerMax = LowerHeight - clipY.lowerTop;
	for (auto i = 1 + clipY.lowerBottom; i <= lowerMax; ++i, dst -= dstPitch + XStep) {
		memset(dst, 0, 2 * XStep * i);
	}
	dst += 2 * XStep + XStep * clipY.upperBottom;
	// Upper triangle (drawn bottom to top):
	const auto upperMax = TriangleUpperHeight - clipY.upperTop;
	for (auto i = 1 + clipY.upperBottom; i <= upperMax; ++i, dst -= dstPitch - XStep) {
		memset(dst, 0, TILE_WIDTH - 2 * XStep * i);
	}
}

// Blit a black tile without clipping (must be fully in bounds).
void RenderBlackTileFull(std::uint8_t *dst, int dstPitch)
{
	dst += XStep * (LowerHeight - 1);
	// Tile is fully in bounds, can use constant loop boundaries.
	// Lower triangle (drawn bottom to top):
	for (unsigned i = 1; i <= LowerHeight; ++i, dst -= dstPitch + XStep) {
		memset(dst, 0, 2 * XStep * i);
	}
	dst += 2 * XStep;
	// Upper triangle (drawn bottom to to top):
	for (unsigned i = 1; i <= TriangleUpperHeight; ++i, dst -= dstPitch - XStep) {
		memset(dst, 0, TILE_WIDTH - 2 * XStep * i);
	}
}

template <TransparencyType Transparency, LightType Light>
DVL_ATTRIBUTE_HOT void RenderSquareNew(std::uint8_t *dst, int dstPitch, const std::uint8_t *src, const std::uint8_t *tbl, Clip clip)
{
	for (auto i = 0; i < clip.height; ++i, dst -= dstPitch) {
		RenderLine<Transparency, Light>(dst, src, clip.width, tbl);
		src += clip.width;
	}
}


} // namespace


void RenderTile(const Surface &out, int x, int y)
{
	//const auto tile = level_cel_block.type;
	//const auto *mask = GetMask(tile);
	//if (mask == nullptr)
	//	return;

#ifdef DEBUG_RENDER_OFFSET_X
	x += DEBUG_RENDER_OFFSET_X;
#endif
#ifdef DEBUG_RENDER_OFFSET_Y
	y += DEBUG_RENDER_OFFSET_Y;
#endif
#ifdef DEBUG_RENDER_COLOR
	DBGCOLOR = GetTileDebugColor(tile);
#endif

	if (level_cel_block.celid>= pDungeonCels.size())
		return;

	Clip clip = CalculateClip(x, y, pDungeonCels[level_cel_block.celid - 1].width, pDungeonCels[level_cel_block.celid - 1].height, out);
	if (clip.width <= 0 || clip.height <= 0)
		return;


	const std::uint8_t *tbl = &LightTables[256 * LightTableIndex];
	//const auto *pFrameTable = reinterpret_cast<const std::uint32_t *>(pDungeonCels.get());
	const uint8_t *src = (const uint8_t *)pDungeonCels[level_cel_block.celid - 1].buffer;
	std::uint8_t *dst = out.at(static_cast<int>(x + clip.left), static_cast<int>(y - clip.bottom));
	const auto dstPitch = out.pitch();

	if (LightTableIndex == LightsMax) {
		RenderSquareNew<TransparencyType::Solid, LightType::FullyDark>(dst, dstPitch, src, tbl, clip);
	} else if (LightTableIndex == 0) {
		RenderSquareNew<TransparencyType::Solid, LightType::FullyLit>(dst, dstPitch, src, tbl, clip);
	} else {
		RenderSquareNew<TransparencyType::Solid, LightType::PartiallyLit>(dst, dstPitch, src, tbl, clip);
	}
}

void world_draw_black_tile(const Surface &out, int sx, int sy)
{
#ifdef DEBUG_RENDER_OFFSET_X
	sx += DEBUG_RENDER_OFFSET_X;
#endif
#ifdef DEBUG_RENDER_OFFSET_Y
	sy += DEBUG_RENDER_OFFSET_Y;
#endif
	auto clip = CalculateClip(sx, sy, TILE_WIDTH, TriangleHeight, out);
	if (clip.width <= 0 || clip.height <= 0)
		return;

	auto clipY = CalculateDiamondClipY(clip);
	std::uint8_t *dst = out.at(sx, static_cast<int>(sy - clip.bottom));
	if (clip.width == TILE_WIDTH) {
		if (clip.height == TriangleHeight) {
			RenderBlackTileFull(dst, out.pitch());
		} else {
			RenderBlackTileClipY(dst, out.pitch(), clipY);
		}
	} else {
		if (clip.right == 0) {
			RenderBlackTileClipLeftAndVertical(dst, out.pitch(), sx, clipY);
		} else {
			RenderBlackTileClipRightAndVertical(dst, out.pitch(), clip.width, clipY);
		}
	}
}

} // namespace devilution

/**
 * @file palette.cpp
 *
 * Implementation of functions for handling the engines color palette.
 */

#include "dx.h"
#include "engine/load_file.hpp"
#include "engine/random.hpp"
#include "hwcursor.hpp"
#include "options.h"
#include "utils/display.h"
#include "utils/sdl_compat.h"

namespace devilution {

SDL_Color logical_palette[256];
SDL_Color system_palette[256];
SDL_Color orig_palette[256];
Uint8 paletteTransparencyLookup[256][256];

namespace {

/** Specifies whether the palette has max brightness. */
bool sgbFadedIn = true;

void LoadGamma()
{
	int gammaValue = sgOptions.Graphics.nGammaCorrection;
	gammaValue = clamp(gammaValue, 30, 100);
	sgOptions.Graphics.nGammaCorrection = gammaValue - gammaValue % 5;
}

Uint8 FindBestMatchForColor(SDL_Color *palette, SDL_Color color, int skipFrom, int skipTo)
{
	Uint8 best;
	Uint32 bestDiff = SDL_MAX_UINT32;
	for (int i = 0; i < 256; i++) {
		if (i >= skipFrom && i <= skipTo)
			continue;
		int diffr = palette[i].r - color.r;
		int diffg = palette[i].g - color.g;
		int diffb = palette[i].b - color.b;
		Uint32 diff = diffr * diffr + diffg * diffg + diffb * diffb;

		if (bestDiff > diff) {
			best = i;
			bestDiff = diff;
		}
	}
	return best;
}

/**
 * @brief Generate lookup table for transparency
 *
 * This is based of the same technique found in Quake2.
 *
 * To mimic 50% transparency we figure out what colors in the existing palette are the best match for the combination of any 2 colors.
 * We save this into a lookup table for use during rendering.
 *
 * @param palette The colors to operate on
 * @param skipFrom Do not use colors between this index and skipTo
 * @param skipTo Do not use colors between skipFrom and this index
 * @param toUpdate Only update the first n colors
 */
void GenerateBlendedLookupTable(SDL_Color *palette, int skipFrom, int skipTo, int toUpdate = 256)
{
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			if (i == j) { // No need to calculate transparency between 2 identical colors
				paletteTransparencyLookup[i][j] = j;
				continue;
			}
			if (i > j) { // Half the blends will be mirror identical ([i][j] is the same as [j][i]), so simply copy the existing combination.
				paletteTransparencyLookup[i][j] = paletteTransparencyLookup[j][i];
				continue;
			}
			if (i > toUpdate && j > toUpdate) {
				continue;
			}

			SDL_Color blendedColor;
			blendedColor.r = ((int)palette[i].r + (int)palette[j].r) / 2;
			blendedColor.g = ((int)palette[i].g + (int)palette[j].g) / 2;
			blendedColor.b = ((int)palette[i].b + (int)palette[j].b) / 2;
			Uint8 best = FindBestMatchForColor(palette, blendedColor, skipFrom, skipTo);
			paletteTransparencyLookup[i][j] = best;
		}
	}
}

/**
 * @brief Cycle the given range of colors in the palette
 * @param from First color index of the range
 * @param to First color index of the range
 */
void CycleColors(int from, int to)
{
	{
		SDL_Color col = system_palette[from];
		for (int i = from; i < to; i++) {
			system_palette[i] = system_palette[i + 1];
		}
		system_palette[to] = col;
	}

	if (!sgOptions.Graphics.bBlendedTransparancy)
		return;

	for (auto &palette : paletteTransparencyLookup) {
		Uint8 col = palette[from];
		for (int j = from; j < to; j++) {
			palette[j] = palette[j + 1];
		}
		palette[to] = col;
	}

	Uint8 colRow[256];
	memcpy(colRow, &paletteTransparencyLookup[from], sizeof(*paletteTransparencyLookup));
	for (int i = from; i < to; i++) {
		memcpy(&paletteTransparencyLookup[i], &paletteTransparencyLookup[i + 1], sizeof(*paletteTransparencyLookup));
	}
	memcpy(&paletteTransparencyLookup[to], colRow, sizeof(colRow));
}

/**
 * @brief Cycle the given range of colors in the palette in reverse direction
 * @param from First color index of the range
 * @param to First color index of the range
 */
void CycleColorsReverse(int from, int to)
{
	{
		SDL_Color col = system_palette[to];
		for (int i = to; i > from; i--) {
			system_palette[i] = system_palette[i - 1];
		}
		system_palette[from] = col;
	}

	if (!sgOptions.Graphics.bBlendedTransparancy)
		return;

	for (auto &palette : paletteTransparencyLookup) {
		Uint8 col = palette[to];
		for (int j = to; j > from; j--) {
			palette[j] = palette[j - 1];
		}
		palette[from] = col;
	}

	Uint8 colRow[256];
	memcpy(colRow, &paletteTransparencyLookup[to], sizeof(*paletteTransparencyLookup));
	for (int i = to; i > from; i--) {
		memcpy(&paletteTransparencyLookup[i], &paletteTransparencyLookup[i - 1], sizeof(*paletteTransparencyLookup));
	}
	memcpy(&paletteTransparencyLookup[from], colRow, sizeof(colRow));
}

} // namespace

void palette_update()
{
	assert(Palette);
	if (SDLC_SetSurfaceAndPaletteColors(pal_surface, Palette, system_palette, 0, 256) < 0) {
		ErrSdl();
	}
	pal_surface_palette_version++;
}

void ApplyGamma(SDL_Color *dst, const SDL_Color *src, int n)
{
	double g = sgOptions.Graphics.nGammaCorrection / 100.0;

	for (int i = 0; i < n; i++) {
		dst[i].r = static_cast<Uint8>(pow(src[i].r / 256.0, g) * 256.0);
		dst[i].g = static_cast<Uint8>(pow(src[i].g / 256.0, g) * 256.0);
		dst[i].b = static_cast<Uint8>(pow(src[i].b / 256.0, g) * 256.0);
	}
	force_redraw = 255;
}

void palette_init()
{
	LoadGamma();
	memcpy(system_palette, orig_palette, sizeof(orig_palette));
	InitPalette();
}

void LoadPalette(const char *pszFileName, bool blend /*= true*/)
{
	extern const char *pal_name;
	pal_name = pszFileName;
}

void LoadRndLvlPal(dungeon_type l)
{
	if (l == DTYPE_TOWN) {
		LoadPalette("Town");
		return;
	}

	int rv = GenerateRnd(4) + 1;
	if (l == DTYPE_CRYPT) {
		LoadPalette("L5Base");
		return;
	}

	static char szFileName[27];
	if (l == DTYPE_NEST) {
		if (!gbNestArt) {
			rv++;
		}
		sprintf(szFileName, "L%iBase%i",  6, rv);
	} else {
		sprintf(szFileName, "L%i_%i", l, rv);
	}
	LoadPalette(szFileName);
}

void IncreaseGamma()
{
	if (sgOptions.Graphics.nGammaCorrection < 100) {
		sgOptions.Graphics.nGammaCorrection += 5;
		if (sgOptions.Graphics.nGammaCorrection > 100)
			sgOptions.Graphics.nGammaCorrection = 100;
		ApplyGamma(system_palette, logical_palette, 256);
		palette_update();
	}
}

void DecreaseGamma()
{
	if (sgOptions.Graphics.nGammaCorrection > 30) {
		sgOptions.Graphics.nGammaCorrection -= 5;
		if (sgOptions.Graphics.nGammaCorrection < 30)
			sgOptions.Graphics.nGammaCorrection = 30;
		ApplyGamma(system_palette, logical_palette, 256);
		palette_update();
	}
}

int UpdateGamma(int gamma)
{
	if (gamma > 0) {
		sgOptions.Graphics.nGammaCorrection = 130 - gamma;
		ApplyGamma(system_palette, logical_palette, 256);
		palette_update();
	}
	return 130 - sgOptions.Graphics.nGammaCorrection;
}

void SetFadeLevel(int fadeval)
{
	for (int i = 0; i < 256; i++) {
		system_palette[i].r = (fadeval * logical_palette[i].r) / 256;
		system_palette[i].g = (fadeval * logical_palette[i].g) / 256;
		system_palette[i].b = (fadeval * logical_palette[i].b) / 256;
	}
	palette_update();
	if (IsHardwareCursor()) {
		ReinitializeHardwareCursor();
	}
}

void BlackPalette()
{
	SetFadeLevel(0);
}

void PaletteFadeIn(int fr)
{
	
}

void PaletteFadeOut(int fr)
{
	
}

void palette_update_caves()
{
	CycleColors(1, 31);
	palette_update();
}

void palette_update_crypt()
{
	static int laveDelay = 0;
	static int glowDelay = 0;

	if (laveDelay > 1) {
		CycleColorsReverse(1, 15);
		laveDelay = 0;
	} else {
		laveDelay++;
	}
	if (glowDelay > 0) {
		CycleColorsReverse(16, 31);
		palette_update();
		glowDelay++;
	} else {
		glowDelay = 1;
	}
}

void palette_update_hive()
{
	static int waveDelay = 0;
	static int bubbleDelay = 0;

	if (waveDelay == 2) {
		CycleColorsReverse(1, 8);
		waveDelay = 0;
	} else {
		waveDelay++;
	}
	if (bubbleDelay == 2) {
		CycleColorsReverse(9, 15);
		palette_update();
		bubbleDelay = 0;
	} else {
		bubbleDelay++;
	}
}

void palette_update_quest_palette(int n)
{
	int i = 32 - n;
	logical_palette[i] = orig_palette[i];
	ApplyGamma(system_palette, logical_palette, 32);
	palette_update();
	if (sgOptions.Graphics.bBlendedTransparancy) {
		// Update blended transparency, but only for the color that was updated
		for (int j = 0; j < 256; j++) {
			if (i == j) { // No need to calculate transparency between 2 identical colors
				paletteTransparencyLookup[i][j] = j;
				continue;
			}
			SDL_Color blendedColor;
			blendedColor.r = ((int)logical_palette[i].r + (int)logical_palette[j].r) / 2;
			blendedColor.g = ((int)logical_palette[i].g + (int)logical_palette[j].g) / 2;
			blendedColor.b = ((int)logical_palette[i].b + (int)logical_palette[j].b) / 2;
			Uint8 best = FindBestMatchForColor(logical_palette, blendedColor, 1, 31);
			paletteTransparencyLookup[i][j] = paletteTransparencyLookup[j][i] = best;
		}
	}
}

} // namespace devilution

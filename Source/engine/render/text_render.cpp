/**
 * @file text_render.cpp
 *
 * Text rendering.
 */
#include "text_render.hpp"

#include "DiabloUI/ui_item.h"
#include "cel_render.hpp"
#include "engine.h"
#include "engine/load_cel.hpp"
#include "engine/point.hpp"
#include "palette.h"

#include "../../rhi/image.h"
#include "../../rhi/gl_render.h"

namespace devilution {

namespace {

/**
 * Maps ASCII character code to font index, as used by the
 * small, medium and large sized fonts; which corresponds to smaltext.cel,
 * medtexts.cel and bigtgold.cel respectively.
 */
const uint8_t FontIndex[256] = {
	// clang-format off
	'\0', 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	' ',  '!',  '\"', '#',  '$',  '%',  '&',  '\'', '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
	'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
	'@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
	'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '[',  '\\', ']',  '^',  '_',
	'`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
	'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '{',  '|',  '}',  '~',  0x01,
	'C',  'u',  'e',  'a',  'a',  'a',  'a',  'c',  'e',  'e',  'e',  'i',  'i',  'i',  'A',  'A',
	'E',  'a',  'A',  'o',  'o',  'o',  'u',  'u',  'y',  'O',  'U',  'c',  'L',  'Y',  'P',  'f',
	'a',  'i',  'o',  'u',  'n',  'N',  'a',  'o',  '?',  0x01, 0x01, 0x01, 0x01, '!',  '<',  '>',
	'o',  '+',  '2',  '3',  '\'', 'u',  'P',  '.',  ',',  '1',  '0',  '>',  0x01, 0x01, 0x01, '?',
	'A',  'A',  'A',  'A',  'A',  'A',  'A',  'C',  'E',  'E',  'E',  'E',  'I',  'I',  'I',  'I',
	'D',  'N',  'O',  'O',  'O',  'O',  'O',  'X',  '0',  'U',  'U',  'U',  'U',  'Y',  'b',  'B',
	'a',  'a',  'a',  'a',  'a',  'a',  'a',  'c',  'e',  'e',  'e',  'e',  'i',  'i',  'i',  'i',
	'o',  'n',  'o',  'o',  'o',  'o',  'o',  '/',  '0',  'u',  'u',  'u',  'u',  'y',  'b',  'y',
	// clang-format on
};

/** Maps from font index to cel frame number. */
const uint8_t FontFrame[3][128] = {
	{
	    // clang-format off
	     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	     0, 54, 44, 57, 58, 56, 55, 47, 40, 41, 59, 39, 50, 37, 51, 52,
	    36, 27, 28, 29, 30, 31, 32, 33, 34, 35, 48, 49, 60, 38, 61, 53,
	    62,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
	    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 42, 63, 43, 64, 65,
	     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
	    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 40, 66, 41, 67,  0,
	    // clang-format on
	},
	{
	    // clang-format off
	     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	     0, 37, 49, 38,  0, 39, 40, 47, 42, 43, 41, 45, 52, 44, 53, 55,
	    36, 27, 28, 29, 30, 31, 32, 33, 34, 35, 51, 50, 48, 46, 49, 54,
	     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
	    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 42,  0, 43,  0,  0,
	     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
	    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 48,  0, 49,  0,  0,
	    // clang-format on
	},
	{
	    // clang-format off
	     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	     0, 37, 49, 38,  0, 39, 40, 47, 42, 43, 41, 45, 52, 44, 53, 55,
	    36, 27, 28, 29, 30, 31, 32, 33, 34, 35, 51, 50,  0, 46,  0, 54,
	     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
	    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 42,  0, 43,  0,  0,
	     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
	    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 20,  0, 21,  0,  0,
	    // clang-format on
	},
};

/**
 * Maps from cel frame number to character width. Note, the character width
 * may be distinct from the frame width, which is the same for every cel frame.
 */
const uint8_t FontKern[3][68] = {
	{
	    // clang-format off
		 8, 10,  7,  9,  8,  7,  6,  8,  8,  3,
		 3,  8,  6, 11,  9, 10,  6,  9,  9,  6,
		 9, 11, 10, 13, 10, 11,  7,  5,  7,  7,
		 8,  7,  7,  7,  7,  7, 10,  4,  5,  6,
		 3,  3,  4,  3,  6,  6,  3,  3,  3,  3,
		 3,  2,  7,  6,  3, 10, 10,  6,  6,  7,
		 4,  4,  9,  6,  6, 12,  3,  7
	    // clang-format on
	},
	{
	    // clang-format off
		 5, 15, 10, 13, 14, 10,  9, 13, 11,  5,
		 5, 11, 10, 16, 13, 16, 10, 15, 12, 10,
		14, 17, 17, 22, 17, 16, 11,  5, 11, 11,
		11, 10, 11, 11, 11, 11, 15,  5, 10, 18,
		15,  8,  6,  6,  7, 10,  9,  6, 10, 10,
		 5,  5,  5,  5, 11, 12
	    // clang-format on
	},
	{
	    // clang-format off
		18, 33, 21, 26, 28, 19, 19, 26, 25, 11,
		12, 25, 19, 34, 28, 32, 20, 32, 28, 20,
		28, 36, 35, 46, 33, 33, 24, 11, 23, 22,
		22, 21, 22, 21, 21, 21, 32, 10, 20, 36,
		31, 17, 13, 12, 13, 18, 16, 11, 20, 21,
		11, 10, 12, 11, 21, 23
	    // clang-format on
	}
};

enum text_color : uint8_t {
	ColorWhite,
	ColorBlue,
	ColorRed,
	ColorGold,
	ColorBlack,
};

int LineHeights[3] = { 12, 38, 50 };

/** Graphics for the fonts */
StormImage *fonts[3];

uint8_t fontColorTableGold[256];
uint8_t fontColorTableBlue[256];
uint8_t fontColorTableRed[256];

void DrawChar(const Surface &out, Point position, GameFontTables size, int nCel, text_color color)
{
	switch (color) {
	case ColorWhite:
		GL_SetColor(255, 255, 255);
		break;
	case ColorBlue:
		GL_SetColor(91, 89, 174);
		break;
	case ColorRed:
		GL_SetColor(255, 0, 0);
		break;
	case ColorGold:
		GL_SetColor(218, 198, 153);
		break;
	case ColorBlack:
		LightTableIndex = 15;
		return;
	}

	fonts[size]->ClipRenderNoLighting(out, position.x, position.y, nCel);
	GL_SetColor(255, 255, 255);
}

} // namespace

StormImage *pSPentSpn2Cels;

void InitText()
{
	fonts[GameFontSmall] = StormImage::LoadImageSequence("CtrlPan\\SmalText", false, false);
	fonts[GameFontMed] = StormImage::LoadImageSequence("Data\\MedTextS", false, false);
	fonts[GameFontBig] = StormImage::LoadImageSequence("Data\\BigTGold", false, false);

	pSPentSpn2Cels = StormImage::LoadImageSequence("Data\\PentSpn2", false, false);

	for (int i = 0; i < 256; i++) {
		uint8_t pix = i;
		if (pix >= PAL16_GRAY + 14)
			pix = PAL16_BLUE + 15;
		else if (pix >= PAL16_GRAY)
			pix -= PAL16_GRAY - (PAL16_BLUE + 2);
		fontColorTableBlue[i] = pix;
	}

	for (int i = 0; i < 256; i++) {
		uint8_t pix = i;
		if (pix >= PAL16_GRAY)
			pix -= PAL16_GRAY - PAL16_RED;
		fontColorTableRed[i] = pix;
	}

	for (int i = 0; i < 256; i++) {
		uint8_t pix = i;
		if (pix >= PAL16_GRAY + 14)
			pix = PAL16_YELLOW + 15;
		else if (pix >= PAL16_GRAY)
			pix -= PAL16_GRAY - (PAL16_YELLOW + 2);
		fontColorTableGold[i] = pix;
	}
}

int GetLineWidth(string_view text, GameFontTables size, int spacing, int *charactersInLine)
{
	int lineWidth = 0;

	size_t i = 0;
	for (; i < text.length(); i++) {
		if (text[i] == '\n')
			break;

		uint8_t frame = FontFrame[size][FontIndex[static_cast<uint8_t>(text[i])]];
		lineWidth += FontKern[size][frame] + spacing;
	}

	if (charactersInLine != nullptr)
		*charactersInLine = i;

	return lineWidth != 0 ? (lineWidth - spacing) : 0;
}

int AdjustSpacingToFitHorizontally(int &lineWidth, int maxSpacing, int charactersInLine, int availableWidth)
{
	if (lineWidth <= availableWidth || charactersInLine < 2)
		return maxSpacing;

	const int overhang = lineWidth - availableWidth;
	const int spacingRedux = (overhang + charactersInLine - 2) / (charactersInLine - 1);
	lineWidth -= spacingRedux * (charactersInLine - 1);
	return maxSpacing - spacingRedux;
}

void WordWrapGameString(char *text, size_t width, GameFontTables size, int spacing)
{
	const size_t textLength = strlen(text);
	size_t lineStart = 0;
	size_t lineWidth = 0;
	for (unsigned i = 0; i < textLength; i++) {
		if (text[i] == '\n') { // Existing line break, scan next line
			lineStart = i + 1;
			lineWidth = 0;
			continue;
		}

		uint8_t frame = FontFrame[size][FontIndex[static_cast<uint8_t>(text[i])]];
		lineWidth += FontKern[size][frame] + spacing;

		if (lineWidth - spacing <= width) {
			continue; // String is still within the limit, continue to the next line
		}

		size_t j; // Backtrack to the previous space
		for (j = i; j >= lineStart; j--) {
			if (text[j] == ' ') {
				break;
			}
		}

		if (j == lineStart) { // Single word longer than width
			if (i == textLength)
				break;
			j = i;
		}

		// Break line and continue to next line
		i = j;
		text[i] = '\n';
		lineStart = i + 1;
		lineWidth = 0;
	}
}

/**
 * @todo replace Rectangle with cropped Surface
 */
uint16_t DrawString(const Surface &out, string_view text, const Rectangle &rect, UiFlags flags, int spacing, int lineHeight, bool drawTextCursor)
{
	GameFontTables size = GameFontSmall;
	if (HasAnyOf(flags, UiFlags::FontMedium))
		size = GameFontMed;
	else if (HasAnyOf(flags, UiFlags::FontHuge))
		size = GameFontBig;

	text_color color = ColorGold;
	if (HasAnyOf(flags, UiFlags::ColorSilver))
		color = ColorWhite;
	else if (HasAnyOf(flags, UiFlags::ColorBlue))
		color = ColorBlue;
	else if (HasAnyOf(flags, UiFlags::ColorRed))
		color = ColorRed;
	else if (HasAnyOf(flags, UiFlags::ColorBlack))
		color = ColorBlack;

	int charactersInLine = 0;
	int lineWidth = 0;
	if (HasAnyOf(flags, (UiFlags::AlignCenter | UiFlags::AlignRight | UiFlags::KerningFitSpacing)))
		lineWidth = GetLineWidth(text, size, spacing, &charactersInLine);

	int maxSpacing = spacing;
	if (HasAnyOf(flags, UiFlags::KerningFitSpacing))
		spacing = AdjustSpacingToFitHorizontally(lineWidth, maxSpacing, charactersInLine, rect.size.width);

	Point characterPosition = rect.position;
	if (HasAnyOf(flags, UiFlags::AlignCenter))
		characterPosition.x += (rect.size.width - lineWidth) / 2;
	else if (HasAnyOf(flags, UiFlags::AlignRight))
		characterPosition.x += rect.size.width - lineWidth;

	int rightMargin = rect.position.x + rect.size.width;
	int bottomMargin = rect.size.height != 0 ? rect.position.y + rect.size.height : out.h();

	if (lineHeight == -1)
		lineHeight = LineHeights[size];

	uint16_t i = 0;
	for (; i < text.length(); i++) {
		uint8_t frame = FontFrame[size][FontIndex[static_cast<uint8_t>(text[i])]];
		int symbolWidth = FontKern[size][frame];
		if (text[i] == '\n' || characterPosition.x + symbolWidth > rightMargin) {
			if (characterPosition.y + lineHeight >= bottomMargin)
				break;
			characterPosition.y += lineHeight;

			if (HasAnyOf(flags, (UiFlags::AlignCenter | UiFlags::AlignRight | UiFlags::KerningFitSpacing)))
				lineWidth = GetLineWidth(&text[i + 1], size, spacing, &charactersInLine);

			if (HasAnyOf(flags, UiFlags::KerningFitSpacing))
				spacing = AdjustSpacingToFitHorizontally(lineWidth, maxSpacing, charactersInLine, rect.size.width);

			characterPosition.x = rect.position.x;
			if (HasAnyOf(flags, UiFlags::AlignCenter))
				characterPosition.x += (rect.size.width - lineWidth) / 2;
			else if (HasAnyOf(flags, UiFlags::AlignRight))
				characterPosition.x += rect.size.width - lineWidth;
		}
		if (frame != 0) {
			DrawChar(out, characterPosition, size, frame, color);
		}
		if (text[i] != '\n')
			characterPosition.x += symbolWidth + spacing;
	}
	if (drawTextCursor) {
		//CelDrawTo(out, characterPosition, *pSPentSpn2Cels, PentSpn2Spin());
		pSPentSpn2Cels->ClipRenderNoLighting(out, characterPosition.x, characterPosition.y, PentSpn2Spin());
	}

	return i;
}

uint8_t PentSpn2Spin()
{
	return (SDL_GetTicks() / 50) % 8 + 1;
}

} // namespace devilution

#include "DiabloUI/text_draw.h"

#include "DiabloUI/art_draw.h"
#include "DiabloUI/diabloui.h"
#include "DiabloUI/fonts.h"
#include "DiabloUI/text.h"
#include "DiabloUI/ui_item.h"
#include "utils/display.h"

namespace devilution {

namespace {


int AlignXOffset(UiFlags flags, const SDL_Rect &dest, int w)
{
	if (HasAnyOf(flags, UiFlags::AlignCenter))
		return (dest.w - w) / 2;
	if (HasAnyOf(flags, UiFlags::AlignRight))
		return dest.w - w;
	return 0;
}

} // namespace

void DrawTTF(const char *text, const SDL_Rect &rectIn, UiFlags flags,
    const SDL_Color &textColor, const SDL_Color &shadowColor,
    TtfSurfaceCache &renderCache)
{
	
}

void DrawArtStr(const char *text, const SDL_Rect &rect, UiFlags flags, bool drawTextCursor)
{
	_artFontTables size = AFT_SMALL;
	_artFontColors color = HasAnyOf(flags, UiFlags::ColorGold) ? AFC_GOLD : AFC_SILVER;

	if (HasAnyOf(flags, UiFlags::FontMedium))
		size = AFT_MED;
	else if (HasAnyOf(flags, UiFlags::FontBig))
		size = AFT_BIG;
	else if (HasAnyOf(flags, UiFlags::FontHuge))
		size = AFT_HUGE;

	const int x = rect.x + AlignXOffset(flags, rect, GetArtStrWidth(text, size));
	const int y = rect.y + (HasAnyOf(flags, UiFlags::VerticalCenter) ? (rect.h - ArtFonts[size][color].h()) / 2 : 0);

	int sx = x;
	int sy = y;
	for (size_t i = 0, n = strlen(text); i < n; i++) {
		if (text[i] == '\n') {
			sx = x;
			sy += ArtFonts[size][color].h();
			continue;
		}
		uint8_t w = FontTables[size][static_cast<uint8_t>(text[i]) + 2];
		w = (w != 0) ? w : FontTables[size][0];
		DrawArt({ sx, sy }, &ArtFonts[size][color], static_cast<uint8_t>(text[i]), w);
		sx += w;
	}
	if (drawTextCursor && GetAnimationFrame(2, 500) != 0) {
		DrawArt({ sx, sy }, &ArtFonts[size][color], '|');
	}
}

} // namespace devilution

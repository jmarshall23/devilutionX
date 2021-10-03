#include "scrollbar.h"

namespace devilution {

Art ArtScrollBarBackground;
Art ArtScrollBarThumb;
Art ArtScrollBarArrow;

void LoadScrollBar()
{
	LoadArt("ui_art\\sb_bg.png", &ArtScrollBarBackground);
	LoadArt("ui_art\\sb_thumb.png", &ArtScrollBarThumb);
	LoadArt("ui_art\\sb_arrow.png", &ArtScrollBarArrow, 4);
}

void UnloadScrollBar()
{
	ArtScrollBarArrow.Unload();
	ArtScrollBarThumb.Unload();
	ArtScrollBarBackground.Unload();
}

} // namespace devilution

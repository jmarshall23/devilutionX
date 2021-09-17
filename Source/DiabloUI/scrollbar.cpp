#include "scrollbar.h"

namespace devilution {

Art ArtScrollBarBackground;
Art ArtScrollBarThumb;
Art ArtScrollBarArrow;

void LoadScrollBar()
{
	LoadArt("ui_art\\sb_bg.tga", &ArtScrollBarBackground);
	LoadArt("ui_art\\sb_thumb.tga", &ArtScrollBarThumb);
	LoadArt("ui_art\\sb_arrow.tga", &ArtScrollBarArrow, 4);
}

void UnloadScrollBar()
{
	ArtScrollBarArrow.Unload();
	ArtScrollBarThumb.Unload();
	ArtScrollBarBackground.Unload();
}

} // namespace devilution

#include "DiabloUI/fonts.h"

#include "diablo.h"
#include "engine/load_file.hpp"
#include "utils/file_util.h"
#include "utils/log.hpp"
#include "utils/paths.h"

namespace devilution {

std::unique_ptr<uint8_t[]> FontTables[4];
Art ArtFonts[4][2];
/** This is so we know ttf has been init when we get to the diablo_deinit() function */
bool was_fonts_init = false;

namespace {

void LoadArtFont(const char *pszFile, int size, int color)
{
	LoadMaskedArt(pszFile, &ArtFonts[size][color], 256, 32);
}

} // namespace

void LoadArtFonts()
{
	FontTables[AFT_SMALL] = LoadFileInMem<uint8_t>("ui_art\\font16.bin");
	FontTables[AFT_MED] = LoadFileInMem<uint8_t>("ui_art\\font24.bin");
	FontTables[AFT_BIG] = LoadFileInMem<uint8_t>("ui_art\\font30.bin");
	FontTables[AFT_HUGE] = LoadFileInMem<uint8_t>("ui_art\\font42.bin");
	LoadArtFont("ui_art\\font16s.tga", AFT_SMALL, AFC_SILVER);
	LoadArtFont("ui_art\\font16g.tga", AFT_SMALL, AFC_GOLD);
	LoadArtFont("ui_art\\font24s.tga", AFT_MED, AFC_SILVER);
	LoadArtFont("ui_art\\font24g.tga", AFT_MED, AFC_GOLD);
	LoadArtFont("ui_art\\font30s.tga", AFT_BIG, AFC_SILVER);
	LoadArtFont("ui_art\\font30g.tga", AFT_BIG, AFC_GOLD);
	LoadArtFont("ui_art\\font42g.tga", AFT_HUGE, AFC_GOLD);
}

void UnloadArtFonts()
{
	ArtFonts[AFT_SMALL][AFC_SILVER].Unload();
	ArtFonts[AFT_SMALL][AFC_GOLD].Unload();
	ArtFonts[AFT_MED][AFC_SILVER].Unload();
	ArtFonts[AFT_MED][AFC_GOLD].Unload();
	ArtFonts[AFT_BIG][AFC_SILVER].Unload();
	ArtFonts[AFT_BIG][AFC_GOLD].Unload();
	ArtFonts[AFT_HUGE][AFC_GOLD].Unload();
	FontTables[AFT_SMALL] = nullptr;
	FontTables[AFT_MED] = nullptr;
	FontTables[AFT_BIG] = nullptr;
	FontTables[AFT_HUGE] = nullptr;
}

void LoadTtfFont()
{
	
}

void UnloadTtfFont()
{
	
}

void FontsCleanup()
{
	
}

} // namespace devilution

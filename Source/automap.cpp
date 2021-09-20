/**
 * @file automap.cpp
 *
 * Implementation of the in-game map overlay.
 */
#include "automap.h"

#include <fmt/format.h>

#include "control.h"
#include "engine/load_file.hpp"
#include "engine/render/automap_render.hpp"
#include "inv.h"
#include "monster.h"
#include "palette.h"
#include "player.h"
#include "setmaps.h"
#include "utils/language.h"
#include "utils/stdcompat/algorithm.hpp"
#include "utils/ui_fwd.h"

#include "../rhi/gl_render.h"
#include "datatable.h"

namespace devilution {

namespace {
Point Automap;

enum MapColors : uint8_t {
	/** color used to draw the player's arrow */
	MapColorsPlayer = (PAL8_ORANGE + 1),
	/** color for bright map lines (doors, stairs etc.) */
	MapColorsBright = PAL8_YELLOW,
	/** color for dim map lines/dots */
	MapColorsDim = (PAL16_YELLOW + 8),
	/** color for items on automap */
	MapColorsItem = (PAL8_BLUE + 1),
};

struct AutomapTile {
	/** The general shape of the tile */
	enum class Types : uint8_t {
		None,
		Diamond,
		Vertical,
		Horizontal,
		Cross,
		FenceVertical,
		FenceHorizontal,
		Corner,
		CaveHorizontalCross,
		CaveVerticalCross,
		CaveHorizontal,
		CaveVertical,
		CaveCross,
	};

	Types type;

	/** Additional details about the given tile */
	enum class Flags : uint8_t {
		// clang-format off
		VerticalDoor      = 1 << 0,
		HorizontalDoor    = 1 << 1,
		VerticalArch      = 1 << 2,
		HorizontalArch    = 1 << 3,
		VerticalGrate     = 1 << 4,
		HorizontalGrate   = 1 << 5,
		VerticalPassage   = VerticalDoor | VerticalArch | VerticalGrate,
		HorizontalPassage = HorizontalDoor | HorizontalArch | HorizontalGrate,
		Dirt              = 1 << 6,
		Stairs            = 1 << 7,
		// clang-format on
	};

	Flags flags;

	constexpr bool HasFlag(Flags test) const
	{
		return (static_cast<uint8_t>(flags) & static_cast<uint8_t>(test)) != 0;
	}
};

/**
 * Maps from tile_id to automap type.
 */
std::array<AutomapTile, 256> AutomapTypeTiles;

void DrawDiamond(const Surface &out, Point center, int r, int g, int b)
{
	const Point left { center.x - AmLine16, center.y };
	const Point top { center.x, center.y - AmLine8 };
	const Point bottom { center.x, center.y + AmLine8 };

	GL_SetColor(r, g, b);
	DrawMapLineNE(out, left, AmLine8);

	GL_SetColor(r, g, b);
	DrawMapLineSE(out, left, AmLine8);

	GL_SetColor(r, g, b);
	DrawMapLineSE(out, top, AmLine8);

	GL_SetColor(r, g, b);
	DrawMapLineNE(out, bottom, AmLine8);
}

void DrawMapVerticalDoor(const Surface &out, Point center)
{
	GL_SetColor(120, 111, 73);
	DrawMapLineNE(out, { center.x + AmLine8, center.y - AmLine4 }, AmLine4);

	GL_SetColor(120, 111, 73);
	DrawMapLineNE(out, { center.x - AmLine16, center.y + AmLine8 }, AmLine4);

	DrawDiamond(out, center, 255, 253, 159);
}

void DrawMapHorizontalDoor(const Surface &out, Point center)
{
	GL_SetColor(120, 111, 73);
	DrawMapLineSE(out, { center.x - AmLine16, center.y - AmLine8 }, AmLine4);

	GL_SetColor(120, 111, 73);
	DrawMapLineSE(out, { center.x + AmLine8, center.y + AmLine4 }, AmLine4);

	DrawDiamond(out, center, 255, 253, 159);
}

void DrawDirt(const Surface &out, Point center)
{
// jmarshall
//	out.SetPixel(center, MapColorsDim);
//	out.SetPixel({ center.x - AmLine8, center.y - AmLine4 }, MapColorsDim);
//	out.SetPixel({ center.x - AmLine8, center.y + AmLine4 }, MapColorsDim);
//	out.SetPixel({ center.x + AmLine8, center.y - AmLine4 }, MapColorsDim);
//	out.SetPixel({ center.x + AmLine8, center.y + AmLine4 }, MapColorsDim);
//	out.SetPixel({ center.x - AmLine16, center.y }, MapColorsDim);
//	out.SetPixel({ center.x + AmLine16, center.y }, MapColorsDim);
//	out.SetPixel({ center.x, center.y - AmLine8 }, MapColorsDim);
//	out.SetPixel({ center.x, center.y + AmLine8 }, MapColorsDim);
//	out.SetPixel({ center.x + AmLine8 - AmLine32, center.y + AmLine4 }, MapColorsDim);
//	out.SetPixel({ center.x - AmLine8 + AmLine32, center.y + AmLine4 }, MapColorsDim);
//	out.SetPixel({ center.x - AmLine16, center.y + AmLine8 }, MapColorsDim);
//	out.SetPixel({ center.x + AmLine16, center.y + AmLine8 }, MapColorsDim);
//	out.SetPixel({ center.x - AmLine8, center.y + AmLine16 - AmLine4 }, MapColorsDim);
//	out.SetPixel({ center.x + AmLine8, center.y + AmLine16 - AmLine4 }, MapColorsDim);
//	out.SetPixel({ center.x, center.y + AmLine16 }, MapColorsDim);
// jmarshall end
}

void DrawStairs(const Surface &out, Point center)
{
	constexpr int NumStairSteps = 4;
	const Displacement offset = { -AmLine8, AmLine4 };
	Point p = { center.x - AmLine8, center.y - AmLine8 - AmLine4 };
	for (int i = 0; i < NumStairSteps; ++i) {
		GL_SetColor(255,253,159);
		DrawMapLineSE(out, p, AmLine16);
		p += offset;
	}
}

/**
 * Left-facing obstacle
 */
void DrawHorizontal(const Surface &out, Point center, AutomapTile tile)
{
	if (!tile.HasFlag(AutomapTile::Flags::HorizontalPassage)) {
		GL_SetColor(120, 111, 73);
		DrawMapLineSE(out, { center.x, center.y - AmLine16 }, AmLine16);
		return;
	}
	if (tile.HasFlag(AutomapTile::Flags::HorizontalDoor)) {
		DrawMapHorizontalDoor(out, { center.x + AmLine16, center.y - AmLine8 });
	}
	if (tile.HasFlag(AutomapTile::Flags::HorizontalGrate)) {
		GL_SetColor(120, 111, 73);
		DrawMapLineSE(out, { center.x + AmLine16, center.y - AmLine8 }, AmLine8);
		DrawDiamond(out, { center.x, center.y - AmLine8 }, 120, 111, 73);
	} else if (tile.HasFlag(AutomapTile::Flags::HorizontalArch)) {
		DrawDiamond(out, { center.x, center.y - AmLine8 }, 120, 111, 73);
	}
}

/**
 * Right-facing obstacle
 */
void DrawVertical(const Surface &out, Point center, AutomapTile tile)
{
	if (!tile.HasFlag(AutomapTile::Flags::VerticalPassage)) {
		GL_SetColor(120, 111, 73);
		DrawMapLineNE(out, { center.x - AmLine32, center.y }, AmLine16);
		return;
	}
	if (tile.HasFlag(AutomapTile::Flags::VerticalDoor)) { // two wall segments with a door in the middle
		DrawMapVerticalDoor(out, { center.x - AmLine16, center.y - AmLine8 });
	}
	if (tile.HasFlag(AutomapTile::Flags::VerticalGrate)) { // right-facing half-wall
		GL_SetColor(120, 111, 73);
		DrawMapLineNE(out, { center.x - AmLine32, center.y }, AmLine8);
		DrawDiamond(out, { center.x, center.y - AmLine8 }, 120, 111, 73);
	} else if (tile.HasFlag(AutomapTile::Flags::VerticalArch)) { // window or passable column
		DrawDiamond(out, { center.x, center.y - AmLine8 }, 120, 111, 73);
	}
}

/**
 * For caves the horizontal/vertical flags are swapped
 */
void DrawCaveHorizontal(const Surface &out, Point center, AutomapTile tile)
{
	if (tile.HasFlag(AutomapTile::Flags::VerticalDoor)) {
		DrawMapHorizontalDoor(out, { center.x - AmLine16, center.y + AmLine8 });
	} else {
		GL_SetColor(120, 111, 73);
		DrawMapLineSE(out, { center.x - AmLine32, center.y }, AmLine16);
	}
}

/**
 * For caves the horizontal/vertical flags are swapped
 */
void DrawCaveVertical(const Surface &out, Point center, AutomapTile tile)
{
	if (tile.HasFlag(AutomapTile::Flags::HorizontalDoor)) {
		DrawMapVerticalDoor(out, { center.x + AmLine16, center.y + AmLine8 });
	} else {
		GL_SetColor(120, 111, 73);
		DrawMapLineNE(out, { center.x, center.y + AmLine16 }, AmLine16);
	}
}

/**
 * @brief Renders the given automap shape at the specified screen coordinates.
 */
void DrawAutomapTile(const Surface &out, Point center, AutomapTile tile)
{
	if (tile.HasFlag(AutomapTile::Flags::Dirt)) {
		DrawDirt(out, center);
	}

	if (tile.HasFlag(AutomapTile::Flags::Stairs)) {
		DrawStairs(out, center);
	}

	switch (tile.type) {
	case AutomapTile::Types::Diamond: // stand-alone column or other unpassable object
		DrawDiamond(out, { center.x, center.y - AmLine8 }, 120, 111, 73);
		break;
	case AutomapTile::Types::Vertical:
	case AutomapTile::Types::FenceVertical:
		DrawVertical(out, center, tile);
		break;
	case AutomapTile::Types::Horizontal:
	case AutomapTile::Types::FenceHorizontal:
		DrawHorizontal(out, center, tile);
		break;
	case AutomapTile::Types::Cross:
		DrawVertical(out, center, tile);
		DrawHorizontal(out, center, tile);
		break;
	case AutomapTile::Types::CaveHorizontalCross:
		DrawVertical(out, center, tile);
		DrawCaveHorizontal(out, center, tile);
		break;
	case AutomapTile::Types::CaveVerticalCross:
		DrawHorizontal(out, center, tile);
		DrawCaveVertical(out, center, tile);
		break;
	case AutomapTile::Types::CaveHorizontal:
		DrawCaveHorizontal(out, center, tile);
		break;
	case AutomapTile::Types::CaveVertical:
		DrawCaveVertical(out, center, tile);
		break;
	case AutomapTile::Types::CaveCross:
		DrawCaveHorizontal(out, center, tile);
		DrawCaveVertical(out, center, tile);
		break;
	case AutomapTile::Types::Corner:
	case AutomapTile::Types::None:
		break;
	}
}

void SearchAutomapItem(const Surface &out, const Displacement &myPlayerOffset)
{
	auto &myPlayer = Players[MyPlayerId];
	Point tile = myPlayer.position.tile;
	if (myPlayer._pmode == PM_WALK3) {
		tile = myPlayer.position.future;
		if (myPlayer._pdir == DIR_W)
			tile.x++;
		else
			tile.y++;
	}

	const int startX = clamp(tile.x - 8, 0, MAXDUNX);
	const int startY = clamp(tile.y - 8, 0, MAXDUNY);

	const int endX = clamp(tile.x + 8, 0, MAXDUNX);
	const int endY = clamp(tile.y + 8, 0, MAXDUNY);

	for (int i = startX; i < endX; i++) {
		for (int j = startY; j < endY; j++) {
			if (dItem[i][j] == 0)
				continue;

			int px = i - 2 * AutomapOffset.deltaX - ViewX;
			int py = j - 2 * AutomapOffset.deltaY - ViewY;

			Point screen = {
				(myPlayerOffset.deltaX * AutoMapScale / 100 / 2) + (px - py) * AmLine16 + gnScreenWidth / 2,
				(myPlayerOffset.deltaY * AutoMapScale / 100 / 2) + (px + py) * AmLine8 + (gnScreenHeight - PANEL_HEIGHT) / 2
			};

			if (CanPanelsCoverView()) {
				if (invflag || sbookflag)
					screen.x -= 160;
				if (chrflag || QuestLogIsOpen)
					screen.x += 160;
			}
			screen.y -= AmLine8;
			DrawDiamond(out, screen, 87, 87, 255);
		}
	}
}

/**
 * @brief Renders an arrow on the automap, centered on and facing the direction of the player.
 */
void DrawAutomapPlr(const Surface &out, const Displacement &myPlayerOffset, int playerId)
{
	//int playerColor = MapColorsPlayer + (8 * playerId) % 128;

	auto &player = Players[playerId];
	Point tile = player.position.tile;
	if (player._pmode == PM_WALK3) {
		tile = player.position.future;
		if (player._pdir == DIR_W)
			tile.x++;
		else
			tile.y++;
	}

	int px = tile.x - 2 * AutomapOffset.deltaX - ViewX;
	int py = tile.y - 2 * AutomapOffset.deltaY - ViewY;

	Displacement playerOffset = player.position.offset;
	if (player.IsWalking())
		playerOffset = GetOffsetForWalking(player.AnimInfo, player._pdir);

	Point base = {
		((playerOffset.deltaX + myPlayerOffset.deltaX) * AutoMapScale / 100 / 2) + (px - py) * AmLine16 + gnScreenWidth / 2,
		((playerOffset.deltaY + myPlayerOffset.deltaY) * AutoMapScale / 100 / 2) + (px + py) * AmLine8 + (gnScreenHeight - PANEL_HEIGHT) / 2
	};

	if (CanPanelsCoverView()) {
		if (invflag || sbookflag)
			base.x -= gnScreenWidth / 4;
		if (chrflag || QuestLogIsOpen)
			base.x += gnScreenWidth / 4;
	}
	base.y -= AmLine8;

	switch (player._pdir) {
	case DIR_N: {
		const Point point { base.x, base.y - AmLine16 };
		DrawVerticalLine(out, point, AmLine16);
		DrawMapLineSteepNE(out, { point.x - AmLine4, point.y + 2 * AmLine4 }, AmLine4);
		DrawMapLineSteepNW(out, { point.x + AmLine4, point.y + 2 * AmLine4 }, AmLine4);
	} break;
	case DIR_NE: {
		const Point point { base.x + AmLine16, base.y - AmLine8 };
		DrawHorizontalLine(out, { point.x - AmLine8, point.y }, AmLine8);
		DrawMapLineNE(out, { point.x - 2 * AmLine8, point.y + AmLine8 }, AmLine8);
		DrawMapLineSteepSW(out, point, AmLine4);
	} break;
	case DIR_E: {
		const Point point { base.x + AmLine16, base.y };
		DrawMapLineNW(out, point, AmLine4);
		DrawHorizontalLine(out, { point.x - AmLine16, point.y }, AmLine16);
		DrawMapLineSW(out, point, AmLine4);
	} break;
	case DIR_SE: {
		const Point point { base.x + AmLine16, base.y + AmLine8 };
		DrawMapLineSE(out, { point.x - 2 * AmLine8, point.y - AmLine8 }, AmLine8);
		DrawHorizontalLine(out, { point.x - (AmLine8 + 1), point.y }, AmLine8 + 1);
		DrawMapLineSteepNW(out, point, AmLine4);
	} break;
	case DIR_S: {
		const Point point { base.x, base.y + AmLine16 };
		DrawVerticalLine(out, { point.x, point.y - AmLine16 }, AmLine16);
		DrawMapLineSteepSW(out, { point.x + AmLine4, point.y - 2 * AmLine4 }, AmLine4);
		DrawMapLineSteepSE(out, { point.x - AmLine4, point.y - 2 * AmLine4 }, AmLine4);
	} break;
	case DIR_SW: {
		const Point point { base.x - AmLine16, base.y + AmLine8 };
		DrawMapLineSteepNE(out, point, AmLine4);
		DrawMapLineSW(out, { point.x + 2 * AmLine8, point.y - AmLine8 }, AmLine8);
		DrawHorizontalLine(out, point, AmLine8 + 1);
	} break;
	case DIR_W: {
		const Point point { base.x - AmLine16, base.y };
		DrawMapLineNE(out, point, AmLine4);
		DrawHorizontalLine(out, point, AmLine16 + 1);
		DrawMapLineSE(out, point, AmLine4);
	} break;
	case DIR_NW: {
		const Point point { base.x - AmLine16, base.y - AmLine8 };
		DrawMapLineNW(out, { point.x + 2 * AmLine8, point.y + AmLine8 }, AmLine8);
		DrawHorizontalLine(out, point, AmLine8 + 1);
		DrawMapLineSteepSE(out, point, AmLine4);
	} break;
	}
}

/**
 * @brief Check if a given tile has the provided AutomapTile flag
 */
bool HasAutomapFlag(Point position, AutomapTile::Flags type)
{
	if (position.x < 0 || position.x >= DMAXX || position.y < 0 || position.y >= DMAXX) {
		return false;
	}

	return AutomapTypeTiles[dungeon[position.x][position.y]].HasFlag(type);
}

/**
 * @brief Returns the automap shape at the given coordinate.
 */
AutomapTile GetAutomapType(Point position)
{
	if (position.x < 0 || position.x >= DMAXX || position.y < 0 || position.y >= DMAXX) {
		return {};
	}

	AutomapTile tile = AutomapTypeTiles[dungeon[position.x][position.y]];
	if (tile.type == AutomapTile::Types::Corner) {
		if (HasAutomapFlag({ position.x - 1, position.y }, AutomapTile::Flags::HorizontalArch)) {
			if (HasAutomapFlag({ position.x, position.y - 1 }, AutomapTile::Flags::VerticalArch)) {
				tile.type = AutomapTile::Types::Diamond;
			}
		}
	}

	return tile;
}

/**
 * @brief Returns the automap shape at the given coordinate.
 */
AutomapTile GetAutomapTypeView(Point map)
{
	if (map.x == -1 && map.y >= 0 && map.y < DMAXY && AutomapView[0][map.y]) {
		if (HasAutomapFlag({ 0, map.y + 1 }, AutomapTile::Flags::Dirt) && HasAutomapFlag({ 0, map.y }, AutomapTile::Flags::Dirt) && HasAutomapFlag({ 0, map.y - 1 }, AutomapTile::Flags::Dirt)) {
			return {};
		}
		return { AutomapTile::Types::None, AutomapTile::Flags::Dirt };
	}

	if (map.y == -1 && map.x >= 0 && map.x < DMAXY && AutomapView[map.x][0]) {
		if (HasAutomapFlag({ map.x + 1, 0 }, AutomapTile::Flags::Dirt) && HasAutomapFlag({ map.x, 0 }, AutomapTile::Flags::Dirt) && HasAutomapFlag({ map.x - 1, 0 }, AutomapTile::Flags::Dirt)) {
			return {};
		}
		return { AutomapTile::Types::None, AutomapTile::Flags::Dirt };
	}

	if (map.x < 0 || map.x >= DMAXX) {
		return {};
	}
	if (map.y < 0 || map.y >= DMAXX) {
		return {};
	}
	if (!AutomapView[map.x][map.y]) {
		return {};
	}

	return GetAutomapType(map);
}

/**
 * @brief Renders game info, such as the name of the current level, and in multi player the name of the game and the game password.
 */
void DrawAutomapText(const Surface &out)
{
	char desc[256];
	Point linePosition { 8, 20 };

	if (gbIsMultiplayer) {
		if (strcasecmp("0.0.0.0", szPlayerName) != 0) {
			strcat(strcpy(desc, _("game: ")), szPlayerName);
			DrawString(out, desc, linePosition);
			linePosition.y += 15;
		}

		if (szPlayerDescript[0] != '\0') {
			strcat(strcpy(desc, _("password: ")), szPlayerDescript);
			DrawString(out, desc, linePosition);
			linePosition.y += 15;
		}
	}

	if (setlevel) {
		DrawString(out, _(QuestLevelNames[setlvlnum]), linePosition);
		return;
	}

	if (currlevel != 0) {
		if (currlevel >= 17 && currlevel <= 20) {
			strcpy(desc, fmt::format(_("Level: Nest {:d}"), currlevel - 16).c_str());
		} else if (currlevel >= 21 && currlevel <= 24) {
			strcpy(desc, fmt::format(_("Level: Crypt {:d}"), currlevel - 20).c_str());
		} else {
			strcpy(desc, fmt::format(_("Level: {:d}"), currlevel).c_str());
		}

		DrawString(out, desc, linePosition);
	}
}

#define AUTOMAP_STRING_TO_ID(x) \
	if (str == #x) {            \
		buf_ptr[i].type = AutomapTile::Types::##x;  \
		buf_ptr[i].flags = (AutomapTile::Flags)flags;		\
		continue;               \
	}

std::unique_ptr<AutomapTile[]> ParseAutomapData(const char *fileName, size_t &tileCount)
{
	DataTable *automapData = new DataTable(fileName);
	tileCount = automapData->NumRows();

	std::unique_ptr<AutomapTile[]> buf { new AutomapTile[tileCount] };

	AutomapTile *buf_ptr = buf.get();

	for (int i = 0; i < tileCount; i++) {
		std::string str = automapData->GetValue("type", i);
		int flags = automapData->GetInt("flags", i);

		AUTOMAP_STRING_TO_ID(None);
		AUTOMAP_STRING_TO_ID(Diamond);
		AUTOMAP_STRING_TO_ID(Vertical);
		AUTOMAP_STRING_TO_ID(Horizontal);
		AUTOMAP_STRING_TO_ID(Cross);
		AUTOMAP_STRING_TO_ID(FenceVertical);
		AUTOMAP_STRING_TO_ID(FenceHorizontal);
		AUTOMAP_STRING_TO_ID(Corner);
		AUTOMAP_STRING_TO_ID(CaveHorizontalCross);
		AUTOMAP_STRING_TO_ID(CaveVerticalCross);
		AUTOMAP_STRING_TO_ID(CaveHorizontal);
		AUTOMAP_STRING_TO_ID(CaveVertical);
		AUTOMAP_STRING_TO_ID(CaveCross);

		devilution::app_fatal("Unknown automap cell type!");
	}

	delete automapData;
	return buf;
}

std::unique_ptr<AutomapTile[]> LoadAutomapData(size_t &tileCount)
{
	switch (leveltype) {
	case DTYPE_CATHEDRAL:
		if (currlevel < 21)
			return ParseAutomapData("Levels\\L1Data\\automap.amptxt", tileCount);
		return ParseAutomapData("NLevels\\L5Data\\automap.amptxt", tileCount);
	case DTYPE_CATACOMBS:
		return ParseAutomapData("Levels\\L2Data\\automap.amptxt", tileCount);
	case DTYPE_CAVES:
		if (currlevel < 17)
			return ParseAutomapData("Levels\\L3Data\\automap.amptxt", tileCount);
		return ParseAutomapData("NLevels\\L6Data\\automap.amptxt", tileCount);
	case DTYPE_HELL:
		return ParseAutomapData("Levels\\L4Data\\automap.amptxt", tileCount);
	default:
		return nullptr;
	}
}

} // namespace

bool AutomapActive;
bool AutomapView[DMAXX][DMAXY];
int AutoMapScale;
Displacement AutomapOffset;
int AmLine64;
int AmLine32;
int AmLine16;
int AmLine8;
int AmLine4;

void InitAutomapOnce()
{
	AutomapActive = false;
	AutoMapScale = 50;
	AmLine64 = 32;
	AmLine32 = 16;
	AmLine16 = 8;
	AmLine8 = 4;
	AmLine4 = 2;
}

void InitAutomap()
{
	size_t tileCount = 0;
	std::unique_ptr<AutomapTile[]> tileTypes = LoadAutomapData(tileCount);
	for (unsigned i = 0; i < tileCount; i++) {
		AutomapTypeTiles[i + 1] = tileTypes[i];
	}

	memset(AutomapView, 0, sizeof(AutomapView));

	for (auto &column : dFlags)
		for (auto &dFlag : column)
			dFlag &= ~BFLAG_EXPLORED;
}

void StartAutomap()
{
	AutomapOffset = { 0, 0 };
	AutomapActive = true;
}

void AutomapUp()
{
	AutomapOffset.deltaX--;
	AutomapOffset.deltaY--;
}

void AutomapDown()
{
	AutomapOffset.deltaX++;
	AutomapOffset.deltaY++;
}

void AutomapLeft()
{
	AutomapOffset.deltaX--;
	AutomapOffset.deltaY++;
}

void AutomapRight()
{
	AutomapOffset.deltaX++;
	AutomapOffset.deltaY--;
}

void AutomapZoomIn()
{
	if (AutoMapScale >= 200)
		return;

	AutoMapScale += 5;
	AmLine64 = (AutoMapScale * 64) / 100;
	AmLine32 = AmLine64 / 2;
	AmLine16 = AmLine32 / 2;
	AmLine8 = AmLine16 / 2;
	AmLine4 = AmLine8 / 2;
}

void AutomapZoomOut()
{
	if (AutoMapScale <= 50)
		return;

	AutoMapScale -= 5;
	AmLine64 = (AutoMapScale * 64) / 100;
	AmLine32 = AmLine64 / 2;
	AmLine16 = AmLine32 / 2;
	AmLine8 = AmLine16 / 2;
	AmLine4 = AmLine8 / 2;
}

void DrawAutomap(const Surface &out)
{
	if (leveltype == DTYPE_TOWN) {
		DrawAutomapText(out);
		return;
	}

	Automap = { (ViewX - 16) / 2, (ViewY - 16) / 2 };
	while (Automap.x + AutomapOffset.deltaX < 0)
		AutomapOffset.deltaX++;
	while (Automap.x + AutomapOffset.deltaX >= DMAXX)
		AutomapOffset.deltaX--;

	while (Automap.y + AutomapOffset.deltaY < 0)
		AutomapOffset.deltaY++;
	while (Automap.y + AutomapOffset.deltaY >= DMAXY)
		AutomapOffset.deltaY--;

	Automap += AutomapOffset;

	const auto &myPlayer = Players[MyPlayerId];
	Displacement myPlayerOffset = ScrollInfo.offset;
	if (myPlayer.IsWalking())
		myPlayerOffset = GetOffsetForWalking(myPlayer.AnimInfo, myPlayer._pdir, true);

	int d = (AutoMapScale * 64) / 100;
	int cells = 2 * (gnScreenWidth / 2 / d) + 1;
	if (((gnScreenWidth / 2) % d) != 0)
		cells++;
	if (((gnScreenWidth / 2) % d) >= (AutoMapScale * 32) / 100)
		cells++;
	if ((myPlayerOffset.deltaX + myPlayerOffset.deltaY) != 0)
		cells++;

	Point screen {
		gnScreenWidth / 2,
		(gnScreenHeight - PANEL_HEIGHT) / 2
	};
	if ((cells & 1) != 0) {
		screen.x -= AmLine64 * ((cells - 1) / 2);
		screen.y -= AmLine32 * ((cells + 1) / 2);
	} else {
		screen.x -= AmLine64 * (cells / 2) - AmLine32;
		screen.y -= AmLine32 * (cells / 2) + AmLine16;
	}
	if ((ViewX & 1) != 0) {
		screen.x -= AmLine16;
		screen.y -= AmLine8;
	}
	if ((ViewY & 1) != 0) {
		screen.x += AmLine16;
		screen.y -= AmLine8;
	}

	screen.x += AutoMapScale * myPlayerOffset.deltaX / 100 / 2;
	screen.y += AutoMapScale * myPlayerOffset.deltaY / 100 / 2;

	if (CanPanelsCoverView()) {
		if (invflag || sbookflag) {
			screen.x -= gnScreenWidth / 4;
		}
		if (chrflag || QuestLogIsOpen) {
			screen.x += gnScreenWidth / 4;
		}
	}

	Point map = { Automap.x - cells, Automap.y - 1 };

	for (int i = 0; i <= cells + 1; i++) {
		Point tile1 = screen;
		for (int j = 0; j < cells; j++) {
			DrawAutomapTile(out, tile1, GetAutomapTypeView({ map.x + j, map.y - j }));
			tile1.x += AmLine64;
		}
		map.y++;

		Point tile2 { screen.x - AmLine32, screen.y + AmLine16 };
		for (int j = 0; j <= cells; j++) {
			DrawAutomapTile(out, tile2, GetAutomapTypeView({ map.x + j, map.y - j }));
			tile2.x += AmLine64;
		}
		map.x++;
		screen.y += AmLine32;
	}

	for (int playerId = 0; playerId < MAX_PLRS; playerId++) {
		auto &player = Players[playerId];
		if (player.plrlevel == myPlayer.plrlevel && player.plractive && !player._pLvlChanging) {
			DrawAutomapPlr(out, myPlayerOffset, playerId);
		}
	}

	if (AutoMapShowItems)
		SearchAutomapItem(out, myPlayerOffset);

	DrawAutomapText(out);
}

void SetAutomapView(Point position)
{
	const Point map { (position.x - 16) / 2, (position.y - 16) / 2 };

	if (map.x < 0 || map.x >= DMAXX || map.y < 0 || map.y >= DMAXY) {
		return;
	}

	AutomapView[map.x][map.y] = true;

	AutomapTile tile = GetAutomapType(map);
	bool solid = tile.HasFlag(AutomapTile::Flags::Dirt);

	switch (tile.type) {
	case AutomapTile::Types::Vertical:
		if (solid) {
			auto tileSW = GetAutomapType({ map.x, map.y + 1 });
			if (tileSW.type == AutomapTile::Types::Corner && tileSW.HasFlag(AutomapTile::Flags::Dirt))
				AutomapView[map.x][map.y + 1] = true;
		} else if (HasAutomapFlag({ map.x - 1, map.y }, AutomapTile::Flags::Dirt)) {
			AutomapView[map.x - 1][map.y] = true;
		}
		break;
	case AutomapTile::Types::Horizontal:
		if (solid) {
			auto tileSE = GetAutomapType({ map.x + 1, map.y });
			if (tileSE.type == AutomapTile::Types::Corner && tileSE.HasFlag(AutomapTile::Flags::Dirt))
				AutomapView[map.x + 1][map.y] = true;
		} else if (HasAutomapFlag({ map.x, map.y - 1 }, AutomapTile::Flags::Dirt)) {
			AutomapView[map.x][map.y - 1] = true;
		}
		break;
	case AutomapTile::Types::Cross:
		if (solid) {
			auto tileSW = GetAutomapType({ map.x, map.y + 1 });
			if (tileSW.type == AutomapTile::Types::Corner && tileSW.HasFlag(AutomapTile::Flags::Dirt))
				AutomapView[map.x][map.y + 1] = true;
			auto tileSE = GetAutomapType({ map.x + 1, map.y });
			if (tileSE.type == AutomapTile::Types::Corner && tileSE.HasFlag(AutomapTile::Flags::Dirt))
				AutomapView[map.x + 1][map.y] = true;
		} else {
			if (HasAutomapFlag({ map.x - 1, map.y }, AutomapTile::Flags::Dirt))
				AutomapView[map.x - 1][map.y] = true;
			if (HasAutomapFlag({ map.x, map.y - 1 }, AutomapTile::Flags::Dirt))
				AutomapView[map.x][map.y - 1] = true;
			if (HasAutomapFlag({ map.x - 1, map.y - 1 }, AutomapTile::Flags::Dirt))
				AutomapView[map.x - 1][map.y - 1] = true;
		}
		break;
	case AutomapTile::Types::FenceVertical:
		if (solid) {
			if (HasAutomapFlag({ map.x, map.y - 1 }, AutomapTile::Flags::Dirt))
				AutomapView[map.x][map.y - 1] = true;
			auto tileSW = GetAutomapType({ map.x, map.y + 1 });
			if (tileSW.type == AutomapTile::Types::Corner && tileSW.HasFlag(AutomapTile::Flags::Dirt))
				AutomapView[map.x][map.y + 1] = true;
		} else if (HasAutomapFlag({ map.x - 1, map.y }, AutomapTile::Flags::Dirt)) {
			AutomapView[map.x - 1][map.y] = true;
		}
		break;
	case AutomapTile::Types::FenceHorizontal:
		if (solid) {
			if (HasAutomapFlag({ map.x - 1, map.y }, AutomapTile::Flags::Dirt))
				AutomapView[map.x - 1][map.y] = true;
			auto tileSE = GetAutomapType({ map.x + 1, map.y });
			if (tileSE.type == AutomapTile::Types::Corner && tileSE.HasFlag(AutomapTile::Flags::Dirt))
				AutomapView[map.x + 1][map.y] = true;
		} else if (HasAutomapFlag({ map.x, map.y - 1 }, AutomapTile::Flags::Dirt)) {
			AutomapView[map.x][map.y - 1] = true;
		}
		break;
	default:
		break;
	}
}

void AutomapZoomReset()
{
	AutomapOffset = { 0, 0 };
	AmLine64 = (AutoMapScale * 64) / 100;
	AmLine32 = AmLine64 / 2;
	AmLine16 = AmLine32 / 2;
	AmLine8 = AmLine16 / 2;
	AmLine4 = AmLine8 / 2;
}

} // namespace devilution

/**
 * @file setmaps.cpp
 *
 * Implementation of functionality the special quest dungeons.
 */
#include "setmaps.h"

#include "drlg_l1.h"
#include "drlg_l2.h"
#include "drlg_l3.h"
#include "engine/load_file.hpp"
#include "objdat.h"
#include "objects.h"
#include "palette.h"
#include "quests.h"
#include "trigs.h"
#include "utils/language.h"

#include "datatable.h"
#include "lighting.h"

namespace devilution {

/** Maps from quest level to quest level names. */
const char *const QuestLevelNames[] = {
	"",
	N_("Skeleton King's Lair"),
	N_("Chamber of Bone"),
	N_("Maze"),
	N_("Poisoned Water Supply"),
	N_("Archbishop Lazarus' Lair"),
};

namespace {

// BUGFIX: constant data should be const
BYTE SkelKingTrans1[] = {
	19, 47, 26, 55,
	26, 49, 30, 53
};

BYTE SkelKingTrans2[] = {
	33, 19, 47, 29,
	37, 29, 43, 39
};

BYTE SkelKingTrans3[] = {
	27, 53, 35, 61,
	27, 35, 34, 42,
	45, 35, 53, 43,
	45, 53, 53, 61,
	31, 39, 49, 57
};

BYTE SkelKingTrans4[] = {
	49, 45, 58, 51,
	57, 31, 62, 37,
	63, 31, 69, 40,
	59, 41, 73, 55,
	63, 55, 69, 65,
	73, 45, 78, 51,
	79, 43, 89, 53
};

BYTE SkelChamTrans1[] = {
	43, 19, 50, 26,
	51, 19, 59, 26,
	35, 27, 42, 34,
	43, 27, 49, 34,
	50, 27, 59, 34
};

BYTE SkelChamTrans2[] = {
	19, 31, 34, 47,
	34, 35, 42, 42
};

BYTE SkelChamTrans3[] = {
	43, 35, 50, 42,
	51, 35, 62, 42,
	63, 31, 66, 46,
	67, 31, 78, 34,
	67, 35, 78, 42,
	67, 43, 78, 46,
	35, 43, 42, 51,
	43, 43, 49, 51,
	50, 43, 59, 51
};

void AddSKingObjs()
{
	constexpr Rectangle SmallSecretRoom { { 20, 7 }, { 3, 3 } };
	ObjectAtPosition({ 64, 34 }).InitializeLoadedObject(SmallSecretRoom, 1);

	constexpr Rectangle Gate { { 20, 14 }, { 1, 2 } };
	ObjectAtPosition({ 64, 59 }).InitializeLoadedObject(Gate, 2);

	constexpr Rectangle LargeSecretRoom { { 8, 1 }, { 7, 10 } };
	ObjectAtPosition({ 27, 37 }).InitializeLoadedObject(LargeSecretRoom, 3);
	ObjectAtPosition({ 46, 35 }).InitializeLoadedObject(LargeSecretRoom, 3);
	ObjectAtPosition({ 49, 53 }).InitializeLoadedObject(LargeSecretRoom, 3);
	ObjectAtPosition({ 27, 53 }).InitializeLoadedObject(LargeSecretRoom, 3);
}

void AddSChamObjs()
{
	ObjectAtPosition({ 37, 30 }).InitializeLoadedObject({ { 17, 0 }, { 4, 5 } }, 1);
	ObjectAtPosition({ 37, 46 }).InitializeLoadedObject({ { 13, 0 }, { 3, 5 } }, 2);
}

void AddVileObjs()
{
	ObjectAtPosition({ 26, 45 }).InitializeLoadedObject({ { 1, 1 }, { 8, 9 } }, 1);
	ObjectAtPosition({ 45, 46 }).InitializeLoadedObject({ { 11, 1 }, { 9, 9 } }, 2);
	ObjectAtPosition({ 35, 36 }).InitializeLoadedObject({ { 7, 11 }, { 6, 7 } }, 3);
}

void SetmapTransparancy(DungeonLevelId levelId)
{
	auto dunData = LoadLevelSetPiece(levelId);

	int width = SDL_SwapLE16(dunData[0]);
	int height = SDL_SwapLE16(dunData[1]);

	int layer2Offset = 2 + width * height;

	// The rest of the layers are at dPiece scale
	width *= 2;
	height *= 2;

	const uint16_t *transparantLayer = &dunData[layer2Offset + width * height * 3];

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			dTransVal[16 + i][16 + j] = SDL_SwapLE16(*transparantLayer);
			transparantLayer++;
		}
	}
}

} // namespace

Object &ObjectAtPosition(Point position)
{
	for (int i = 0; i < ActiveObjectCount; i++) {
		int oi = ActiveObjects[i];
		if (Objects[oi].position == position)
			return Objects[oi];
	}
	app_fatal("ObjectAtPosition: Active object not found at (%i,%i)", position.x, position.y);
}

void AddButcherObjs(void)
{
//	LoadObj(OFILE_TNUDEM);
	LoadObj(OFILE_TNUDEW);

	AddObjectWithFrame(OBJ_TNUDEW1, { 24, 24 }, 1);
	AddObjectWithFrame(OBJ_TNUDEW1, { 27, 24 }, 1);

	//AddObjectWithFrame(OBJ_TNUDEW1, { 23, 26 }, 0);
	//AddObjectWithFrame(OBJ_TNUDEW1, { 23, 28 }, 2);
	AddObjectWithFrame(OBJ_TNUDEW1, { 23, 30 }, 0);

	//AddObjectWithFrame(OBJ_TNUDEW1, { 28, 26 }, 2);
	//AddObjectWithFrame(OBJ_TNUDEW1, { 28, 28 }, 0);
	AddObjectWithFrame(OBJ_TNUDEW1, { 28, 30 }, 2);
}

/**
 * @brief Load a quest map, the given map is specified via the global setlvlnum
 */
void LoadSetMap()
{
	switch (setlvlnum) {
	case SL_BUTCHERLAIR:
		LoadPreL1Dungeon(DUNGEON_BUTCHER_LAIR);
		LoadL1Dungeon(DUNGEON_BUTCHER_LAIR, lvlSetPiecesTables->GetInt("StartX", DUNGEON_VILE_L1), lvlSetPiecesTables->GetInt("StartY", DUNGEON_VILE_L1));
		LoadPalette(lvlSetPiecesTables->GetValue("CustomPalette", DUNGEON_BUTCHER_LAIR));
		AddL1Objs(0, 0, MAXDUNX, MAXDUNY);
		AddButcherObjs();
		InitNoTriggers();
		SetAmbient(lvlSetPiecesTables->GetInt("LevelAmbientR", DUNGEON_BUTCHER_LAIR), lvlSetPiecesTables->GetInt("LevelAmbientG", DUNGEON_BUTCHER_LAIR), lvlSetPiecesTables->GetInt("LevelAmbientB", DUNGEON_BUTCHER_LAIR));
		break;

	case SL_SKELKING:
		if (Quests[Q_SKELKING]._qactive == QUEST_INIT) {
			Quests[Q_SKELKING]._qactive = QUEST_ACTIVE;
			Quests[Q_SKELKING]._qvar1 = 1;
		}
		LoadPreL1Dungeon(DUNGEON_SKELKING_PREL1);
		LoadL1Dungeon(DUNGEON_SKELKING_L1, lvlSetPiecesTables->GetInt("StartX", DUNGEON_SKELKING_L1), lvlSetPiecesTables->GetInt("StartY", DUNGEON_SKELKING_L1));
		LoadPalette(lvlSetPiecesTables->GetValue("CustomPalette", DUNGEON_SKELKING_L1));
		DRLG_AreaTrans(sizeof(SkelKingTrans1) / 4, &SkelKingTrans1[0]);
		DRLG_ListTrans(sizeof(SkelKingTrans2) / 4, &SkelKingTrans2[0]);
		DRLG_AreaTrans(sizeof(SkelKingTrans3) / 4, &SkelKingTrans3[0]);
		DRLG_ListTrans(sizeof(SkelKingTrans4) / 4, &SkelKingTrans4[0]);
		AddL1Objs(0, 0, MAXDUNX, MAXDUNY);
		AddSKingObjs();
		InitSKingTriggers();
		break;
	case SL_BONECHAMB:
		LoadPreL2Dungeon(DUNGEON_BONECHAMBER_PREL2);
		LoadL2Dungeon(DUNGEON_BONECHAMBER_L2, lvlSetPiecesTables->GetInt("StartX", DUNGEON_BONECHAMBER_L2), lvlSetPiecesTables->GetInt("StartY", DUNGEON_BONECHAMBER_L2));
		LoadPalette(lvlSetPiecesTables->GetValue("CustomPalette", DUNGEON_BONECHAMBER_L2));
		DRLG_ListTrans(sizeof(SkelChamTrans1) / 4, &SkelChamTrans1[0]);
		DRLG_AreaTrans(sizeof(SkelChamTrans2) / 4, &SkelChamTrans2[0]);
		DRLG_ListTrans(sizeof(SkelChamTrans3) / 4, &SkelChamTrans3[0]);
		AddL2Objs(0, 0, MAXDUNX, MAXDUNY);
		AddSChamObjs();
		InitSChambTriggers();
		break;
	case SL_MAZE:
		LoadPreL1Dungeon(DUNGEON_MAZE_PRE1);
		LoadL1Dungeon(DUNGEON_MAZE_L1, lvlSetPiecesTables->GetInt("StartX", DUNGEON_MAZE_L1), lvlSetPiecesTables->GetInt("StartY", DUNGEON_MAZE_L1));
		LoadPalette(lvlSetPiecesTables->GetValue("CustomPalette", DUNGEON_MAZE_L1));
		AddL1Objs(0, 0, MAXDUNX, MAXDUNY);
		SetmapTransparancy(DUNGEON_MAZE_PRE1);
		break;
	case SL_POISONWATER:
		if (Quests[Q_PWATER]._qactive == QUEST_INIT)
			Quests[Q_PWATER]._qactive = QUEST_ACTIVE;
		LoadPreL3Dungeon(DUNGEON_FOULWATER_PREL3);
		LoadL3Dungeon(DUNGEON_FOULWATER_L3, lvlSetPiecesTables->GetInt("StartX", DUNGEON_FOULWATER_L3), lvlSetPiecesTables->GetInt("StartY", DUNGEON_FOULWATER_L3));
		LoadPalette(lvlSetPiecesTables->GetValue("CustomPalette", DUNGEON_FOULWATER_L3));
		InitPWaterTriggers();
		break;
	case SL_VILEBETRAYER:
		if (Quests[Q_BETRAYER]._qactive == QUEST_DONE) {
			Quests[Q_BETRAYER]._qvar2 = 4;
		} else if (Quests[Q_BETRAYER]._qactive == QUEST_ACTIVE) {
			Quests[Q_BETRAYER]._qvar2 = 3;
		}
		LoadPreL1Dungeon(DUNGEON_VILE_PREL1);
		LoadL1Dungeon(DUNGEON_VILE_L1, lvlSetPiecesTables->GetInt("StartX", DUNGEON_VILE_L1), lvlSetPiecesTables->GetInt("StartY", DUNGEON_VILE_L1));
		LoadPalette(lvlSetPiecesTables->GetValue("CustomPalette", DUNGEON_VILE_L1));
		AddL1Objs(0, 0, MAXDUNX, MAXDUNY);
		AddVileObjs();
		SetmapTransparancy(DUNGEON_VILE_PREL1);
		InitNoTriggers();
		break;
	case SL_NONE:
		break;
	}
}

} // namespace devilution

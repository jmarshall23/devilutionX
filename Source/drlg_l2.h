/**
 * @file drlg_l2.h
 *
 * Interface of the catacombs level generation algorithms.
 */
#pragma once

#include "gendung.h"

#include "dunload.h"

namespace devilution {

struct HALLNODE {
	int nHallx1;
	int nHally1;
	int nHallx2;
	int nHally2;
	int nHalldir;
};

struct ROOMNODE {
	int nRoomx1;
	int nRoomy1;
	int nRoomx2;
	int nRoomy2;
};

extern BYTE predungeon[DMAXX][DMAXY];

void LoadL2Dungeon(DungeonLevelId levelId, int vx, int vy);
void LoadPreL2Dungeon(DungeonLevelId levelId);
void CreateL2Dungeon(uint32_t rseed, lvl_entry entry);

} // namespace devilution

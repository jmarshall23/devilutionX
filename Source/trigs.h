/**
 * @file trigs.h
 *
 * Interface of functionality for triggering events when the player enters an area.
 */
#pragma once

#include "engine/point.hpp"
#include "gendung.h"
#include "interfac.h"

namespace devilution {

enum WarpTable_t {
	WARP_NEW_GAME = 0,
	WARP_CATHEDRAL_DOWN,
	WARP_CATHEDRAL_RETURN,
	WARP_CATACOMBS_DOWN,
	WARP_CATACOMBS_RETURN,
	WARP_CAVE_DOWN,
	WARP_CAVE_RETURN,
	WARP_HELL_DOWN,
	WARP_HELL_RETURN
};

Point GetWarpLocation(WarpTable_t warpId);


#define MAXTRIGGERS 7

struct TriggerStruct {
	Point position;
	interface_mode _tmsg;
	int _tlvl;
};

extern bool trigflag;
extern int numtrigs;
extern TriggerStruct trigs[MAXTRIGGERS];
extern int TWarpFrom;

void InitNoTriggers();
bool IsWarpOpen(dungeon_type type);
void InitTownTriggers();
void InitL1Triggers();
void InitL2Triggers();
void InitL3Triggers();
void InitL4Triggers();
void InitSKingTriggers();
void InitSChambTriggers();
void InitPWaterTriggers();
void InitVPTriggers();
void Freeupstairs();
void CheckTrigForce();
void CheckTriggers();

} // namespace devilution

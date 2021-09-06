/**
 * @file drlg_l3.h
 *
 * Interface of the caves level generation algorithms.
 */
#pragma once

#include "gendung.h"

#include "dunload.h"

namespace devilution {

void CreateL3Dungeon(uint32_t rseed, lvl_entry entry);
void LoadL3Dungeon(DungeonLevelId levelId, int vx, int vy);
void LoadPreL3Dungeon(DungeonLevelId levelId);

} // namespace devilution

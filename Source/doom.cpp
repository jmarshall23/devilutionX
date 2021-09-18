/**
 * @file doom.cpp
 *
 * Implementation of the map of the stars quest.
 */
#include "doom.h"

#include "control.h"
#include "engine.h"
#include "utils/stdcompat/optional.hpp"

namespace devilution {

bool DoomFlag;

void doom_init()
{
	//DoomCel = LoadCel("Items\\Map\\MapZtown.CEL", 640);
	DoomFlag = true;
}

void doom_close()
{
	DoomFlag = false;
}

void doom_draw(const Surface &out)
{
	if (!DoomFlag) {
		return;
	}

	//CelDrawTo(out, { PANEL_X, PANEL_Y - 1 }, *DoomCel, 1);
}

} // namespace devilution

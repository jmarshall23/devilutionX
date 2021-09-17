/**
* @file monhealthbar.cpp
*
* Adds monster health bar QoL feature
*/

#include "DiabloUI/art_draw.h"
#include "control.h"
#include "cursor.h"
#include "options.h"
#include "qol/common.h"
#include "utils/language.h"

namespace devilution {
namespace {

Art healthBox;
Art resistance;
Art health;

} // namespace

void InitMonsterHealthBar()
{
	
}

void FreeMonsterHealthBar()
{
	healthBox.Unload();
	health.Unload();
	resistance.Unload();
}

void DrawMonsterHealthBar(const Surface &out)
{
	
}

} // namespace devilution

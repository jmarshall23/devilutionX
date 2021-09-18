/**
 * @file cursor.h
 *
 * Interface of cursor tracking functionality.
 */
#pragma once

#include <cstdint>
#include <utility>

#include "engine.h"
#include "miniwin/miniwin.h"
#include "utils/stdcompat/optional.hpp"

namespace devilution {

enum cursor_id : uint8_t {
	CURSOR_NONE,
	CURSOR_HAND,
	CURSOR_IDENTIFY,
	CURSOR_REPAIR,
	CURSOR_RECHARGE,
	CURSOR_DISARM,
	CURSOR_OIL,
	CURSOR_TELEKINESIS,
	CURSOR_RESURRECT,
	CURSOR_TELEPORT,
	CURSOR_HEALOTHER,
	CURSOR_HOURGLASS,
	CURSOR_FIRSTITEM,
};

extern Size cursSize;
extern int pcursmonst;
extern Size icursSize28;
extern Size icursSize;
extern int8_t pcursinvitem;
extern int8_t pcursitem;
extern int8_t pcursobj;
extern int8_t pcursplr;
extern Point cursPosition;
extern int pcurs;

void InitCursor();
void FreeCursor();
void SetICursor(int cursId);
void NewCursor(int cursId);
void InitLevelCursor();
void CheckRportal();
void CheckTown();
void CheckCursMove();

inline bool IsItemSprite(int cursId)
{
	return cursId >= CURSOR_FIRSTITEM;
}

void CelDrawCursor(const Surface &out, Point position, int cursId);

class ImageFrame_t;
/** Returns the sprite for the given inventory index. */
const ImageFrame_t *GetInvItemSprite(int i);

void RenderItemSprite(const Surface &out, int cursId, int x, int y);
void RenderItemSpriteOutline(const Surface &out, int cursId, int x, int y);

} // namespace devilution

/**
* @file xpbar.cpp
*
* Adds XP bar QoL feature
*/
#include "xpbar.h"

#include <array>

#include <fmt/format.h>

#include "DiabloUI/art_draw.h"
#include "common.h"
#include "control.h"
#include "engine/point.hpp"
#include "options.h"
#include "utils/language.h"

namespace devilution {

namespace {

constexpr int BarWidth = 307;

using ColorGradient = std::array<Uint8, 12>;
constexpr ColorGradient GoldGradient = { 0xCF, 0xCE, 0xCD, 0xCC, 0xCB, 0xCA, 0xC9, 0xC8, 0xC7, 0xC6, 0xC5, 0xC4 };
constexpr ColorGradient SilverGradient = { 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8, 0xF7, 0xF6, 0xF5, 0xF4, 0xF3 };

constexpr int BackWidth = 313;
constexpr int BackHeight = 9;

Art xpbarArt;

void DrawBar(const Surface &out, Point screenPosition, int width, const ColorGradient &gradient)
{
	UnsafeDrawHorizontalLine(out, screenPosition + Displacement { 0, 1 }, width, gradient[gradient.size() * 3 / 4 - 1]);
	UnsafeDrawHorizontalLine(out, screenPosition + Displacement { 0, 2 }, width, gradient[gradient.size() - 1]);
	UnsafeDrawHorizontalLine(out, screenPosition + Displacement { 0, 3 }, width, gradient[gradient.size() / 2 - 1]);
}

void DrawEndCap(const Surface &out, Point point, int idx, const ColorGradient &gradient)
{
	out.SetPixel({ point.x, point.y + 1 }, gradient[idx * 3 / 4]);
	out.SetPixel({ point.x, point.y + 2 }, gradient[idx]);
	out.SetPixel({ point.x, point.y + 3 }, gradient[idx / 2]);
}

} // namespace

void InitXPBar()
{
	
}

void FreeXPBar()
{
	xpbarArt.Unload();
}

void DrawXPBar(const Surface &out)
{
	
}

bool CheckXPBarInfo()
{
	

	return true;
}

} // namespace devilution

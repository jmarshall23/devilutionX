/**
 * @file automap_render.cpp
 *
 * Line drawing routines for the automap.
 */
#include "engine/render/automap_render.hpp"

#include "../../rhi/gl_render.h"

namespace devilution {
namespace {

enum class DirectionX {
	EAST = 1,
	WEST = -1,
};

enum class DirectionY {
	SOUTH = 1,
	NORTH = -1,
};

template <DirectionX DirX, DirectionY DirY>
void DrawMapLine(const Surface &out, Point from, int height)
{
	Point dest;
	dest.x = static_cast<int>(DirX) * (height * 2);
	dest.y = static_cast<int>(DirY) * height;

	GL_DrawLine(from.x, from.y, from.x + dest.x, from.y + dest.y, 1.0f);
	//out.SetPixel({ from.x, from.y + 1 }, 0);
	//out.SetPixel(from, colorIndex);
}

template <DirectionX DirX, DirectionY DirY>
void DrawMapLineSteep(const Surface &out, Point from, int width)
{
	Point dest;
	dest.x = static_cast<int>(DirX) * width;
	dest.y = static_cast<int>(DirY) * (width * 2);

	GL_DrawLine(from.x, from.y, from.x + dest.x, from.y + dest.y, 1.0f);
	//out.SetPixel({ from.x, from.y + 1 }, 0);
	//out.SetPixel(from, colorIndex);
}

} // namespace

void DrawMapLineNE(const Surface &out, Point from, int height)
{
	DrawMapLine<DirectionX::EAST, DirectionY::NORTH>(out, from, height);
}

void DrawMapLineSE(const Surface &out, Point from, int height)
{
	DrawMapLine<DirectionX::EAST, DirectionY::SOUTH>(out, from, height);
}

void DrawMapLineNW(const Surface &out, Point from, int height)
{
	DrawMapLine<DirectionX::WEST, DirectionY::NORTH>(out, from, height);
}

void DrawMapLineSW(const Surface &out, Point from, int height)
{
	DrawMapLine<DirectionX::WEST, DirectionY::SOUTH>(out, from, height);
}

void DrawMapLineSteepNE(const Surface &out, Point from, int width)
{
	DrawMapLineSteep<DirectionX::EAST, DirectionY::NORTH>(out, from, width);
}

void DrawMapLineSteepSE(const Surface &out, Point from, int width)
{
	DrawMapLineSteep<DirectionX::EAST, DirectionY::SOUTH>(out, from, width);
}

void DrawMapLineSteepNW(const Surface &out, Point from, int width)
{
	DrawMapLineSteep<DirectionX::WEST, DirectionY::NORTH>(out, from, width);
}

void DrawMapLineSteepSW(const Surface &out, Point from, int width)
{
	DrawMapLineSteep<DirectionX::WEST, DirectionY::SOUTH>(out, from, width);
}

} // namespace devilution

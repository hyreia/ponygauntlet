#ifndef DIRECTIONS_HPP
#define DIRECTIONS_HPP

#include "XYPair.hpp"

namespace gauntlet
{

enum DIRECTIONS
{
	NO_DIRECTION = -1,
	FIRST_DIRECTION = 0,
	DIR_RIGHT = 0,
	DIR_UPRIGHT,
	DIR_UP,
	DIR_UPLEFT,
	DIR_LEFT,
	DIR_DOWNLEFT,
	DIR_DOWN,
	DIR_DOWNRIGHT,
	LAST_DIRECTION = DIR_DOWNRIGHT,
	TOTAL_DIRECTIONS
};

/* Use to find if a direction is just a mirror of another one.
If it is, we use the former
0 = not mirrored, 1 = mirrored, directions are listed above*/
const int DIRECTION_FLAGS[TOTAL_DIRECTIONS] =
{ 0, 0, 0, 1, 1, 1, 0, 0};

/* All this basically says is that down left, left and up left have
the same source as down right, right and up right. */
const int DRAW_ROW_BY_CHAR_DIRECTION[TOTAL_DIRECTIONS] =
{ 2, 1, 0, 1, 2, 3, 4, 3 };


inline XYPair<double> GetUnitVector(int direction)
{
	//0.7071067811865475
	const double DIAGONAL = 0.7071067811865475;

	if(direction == DIR_UP)
		return XYPair<double>(0.0f, -1.0f);
	else if(direction == DIR_UPRIGHT)
		return XYPair<double>(DIAGONAL, -DIAGONAL);
	else if(direction == DIR_RIGHT)
		return XYPair<double>(1.0f, 0.0f);
	else if(direction == DIR_DOWNRIGHT)
		return XYPair<double>(DIAGONAL, DIAGONAL);
	else if(direction == DIR_DOWN)
		return XYPair<double>(0.0f, 1.0f);
	else if(direction == DIR_DOWNLEFT)
		return XYPair<double>(-DIAGONAL, DIAGONAL);
	else if(direction == DIR_LEFT)
		return XYPair<double>(-1.0f, 0.0f);
	else if(direction == DIR_UPLEFT)
		return XYPair<double>(-1.0f, -1.0f);
	else
		return XYPair<double>(0.0f, 0.0f);
}

};

#endif
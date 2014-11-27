#ifndef PLAYER_INPUT_HPP
#define PLAYER_INPUT_HPP


#include <allegro5/allegro.h>
#include "Directions.hpp"
#include "XYPair.hpp"
#include "Button.hpp"

namespace gauntlet
{

enum MOUSE_INPUT
{
	MOUSE_BUTTON_1 = ALLEGRO_KEY_MAX + 1,
	MOUSE_BUTTON_2,
	MOUSE_BUTTON_3,
	MOUSE_Z_INCREMENT,
	MOUSE_Z_DECREMENT
};

enum PLAYER_BUTTONS
{
	MOVE_UP,
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_LEFT,
	USE_MAIN_ITEM,
	USE_SEC_ITEM,
	INC_SECONDARY,
	DEC_SECONDARY,
	PAUSE,
	MENU,
	TOTAL_PLAYER_BUTTONS
};


struct PlayerInput
{
	Button buttons[TOTAL_PLAYER_BUTTONS];

	PlayerInput(){}
	~PlayerInput(){}

	XYPair<double> GetMoveVector()
	{
		XYPair<double> ret(0,0);
		if(buttons[MOVE_UP].IsHeld()) ret.y--;
		if(buttons[MOVE_DOWN].IsHeld()) ret.y++;
		if(buttons[MOVE_LEFT].IsHeld()) ret.x--;
		if(buttons[MOVE_RIGHT].IsHeld()) ret.x++;

		if(ret.x != 0.0 && ret.y != 0.0)
		{
			const double DIAGONAL = 0.7071067811865475;
			ret.x *= DIAGONAL;
			ret.y *= DIAGONAL;
		}

		return ret;
	}

	XYPair<int> GetMovementXY()
	{
		int x = 0, y = 0;
		if(buttons[MOVE_UP].IsHeld()) y--;
		if(buttons[MOVE_DOWN].IsHeld()) y++;
		if(buttons[MOVE_LEFT].IsHeld()) x--;
		if(buttons[MOVE_RIGHT].IsHeld()) x++;

		return XYPair<int>(x, y);
	}

	int GetMoveDirection()
	{
		int x = 0, y = 0;
		auto v = GetMovementXY();
		x = v.x;
		y = v.y;

		if(y == -1)
		{
			if(x == -1)
			{
				return DIR_UPLEFT;
			}
			else if(x == 1)
			{
				return DIR_UPRIGHT;
			}
			else
			{
				return DIR_UP;
			}
		}
		else if(y == 1)
		{
			if(x == -1)
			{
				return DIR_DOWNLEFT;
			}
			else if(x == 1)
			{
				return DIR_DOWNRIGHT;
			}
			else
			{
				return DIR_DOWN;
			}
		}
		else if(x == -1)
		{
			return DIR_LEFT;
		}
		else if(x == 1)
		{
			return DIR_RIGHT;
		}
		else
		{
			return NO_DIRECTION;
		}

	}

	

};

};

#endif
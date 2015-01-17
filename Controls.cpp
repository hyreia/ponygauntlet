#include <allegro5/allegro.h>

#include "Button.hpp"		//Need to implement
#include "Directions.hpp"	//Need to implement
#include "XYPair.hpp"
#include "PlayerInput.hpp"	//Need to implement
#include "Controls.hpp"
#include "GauntletApp.hpp"
#include <cmath>

#include <iostream>

using namespace gauntlet;

const int Controls::DEFAULT_CONTROLS[TOTAL_PLAYER_BUTTONS] =
{
	ALLEGRO_KEY_W, //move up
	ALLEGRO_KEY_D, //move right
	ALLEGRO_KEY_S, //move down
	ALLEGRO_KEY_A, //move left
	MOUSE_BUTTON_1, //main item
	MOUSE_BUTTON_2, //secondary item
	MOUSE_Z_DECREMENT, //increment secondary item
	MOUSE_Z_INCREMENT, //decrement secondary item

	ALLEGRO_KEY_P, //pause button
	ALLEGRO_KEY_ESCAPE //menu
};

Controls::Controls()
{
	mousePositions[0] = 0;
	mousePositions[1] = 0;
	mousePositions[2] = 0;

	for(int button = 0; button < TOTAL_PLAYER_BUTTONS; button++)
	{
		controls[FIRST_PLAYER][button] = DEFAULT_CONTROLS[button];
	}

	//Zero out other controls
	for(int player = PLAYER_ONE; player < MAX_PLAYERS; player++)
	for(int button = 0; button < TOTAL_PLAYER_BUTTONS; button++)
	{
		controls[player][button] = 0;
	}

}

Controls::~Controls(){}

void Controls::Initialize()
{
	//future version: this could load different controls from a file
}

void Controls::Destroy()
{
	//Do nothing, a future version might have them save settings
}
void Controls::ReportKeyDown(int keycode)
{
	const bool IS_BUTTON_DOWN = true;
	/* Like the rest of the input reported functions, this just checks all the buttons to see
	if they match since there's such a short list */
	for(int player = FIRST_PLAYER; player < MAX_PLAYERS; player++)
	for(int button = 0; button < TOTAL_PLAYER_BUTTONS; button++)
	{
		if(controls[player][button] == keycode)
		{
			SetButtonPosition(player, button, IS_BUTTON_DOWN);
			break;
		}
	}

}

void Controls::ReportKeyUp(int keycode)
{
	const bool IS_BUTTON_DOWN = false;
	/* Like the rest of the input reported functions, this just checks all the buttons to see
	if they match since there's such a short list */
	for(int player = FIRST_PLAYER; player < MAX_PLAYERS; player++)
	for(int button = 0; button < TOTAL_PLAYER_BUTTONS; button++)
	{
		if(controls[player][button] == keycode)
		{
			SetButtonPosition(player, button, IS_BUTTON_DOWN);
			break;
		}
	}
}

void Controls::ReportKeyPress(int keycode)
{
	//Do nothing. This is more for keys being held, like when typing
	//Later when we care about a player typing in input we'll care more
}

void Controls::ReportMouseButtonDown(int button)
{
	int code = button + MOUSE_BUTTON_1 - 1;
	const bool IS_BUTTON_DOWN = true;
	/* Like the rest of the input reported functions, this just checks all the buttons to see
	if they match since there's such a short list */
	for(int player = FIRST_PLAYER; player < MAX_PLAYERS; player++)
	for(int button = 0; button < TOTAL_PLAYER_BUTTONS; button++)
	{
		if(controls[player][button] == code)
		{
			SetButtonPosition(player, button, IS_BUTTON_DOWN);
			break;
		}
	}
}

void Controls::ReportMouseButtonUp(int button)
{
	int code = button + MOUSE_BUTTON_1 - 1;
	const bool IS_BUTTON_DOWN = false;
	/* Like the rest of the input reported functions, this just checks all the buttons to see
	if they match since there's such a short list */
	for(int player = FIRST_PLAYER; player < MAX_PLAYERS; player++)
	for(int button = 0; button < TOTAL_PLAYER_BUTTONS; button++)
	{
		if(controls[player][button] == code)
		{
			SetButtonPosition(player, button, IS_BUTTON_DOWN);
			break;
		}
	}
}

void Controls::ReportMousePosition(int x, int y)
{
	mousePositions[0] = x;
	mousePositions[1] = y;
}

void Controls::PollMousePosition()
{
	ALLEGRO_MOUSE_STATE mouse;
	al_get_mouse_state(&mouse);
	app->allegro.TranslateMousePositionToBufferPosition(mouse.x, mouse.y);
	mousePositions[0] = mouse.x;
	mousePositions[1] = mouse.y;

	
}

void Controls::AdjustMouseZPosition(bool isIncrementing)
{

	int code = isIncrementing? MOUSE_Z_INCREMENT: MOUSE_Z_DECREMENT;
	int otherCode = isIncrementing? MOUSE_Z_DECREMENT: MOUSE_Z_INCREMENT;

	/* Like the rest of the input reported functions, this just checks all the buttons to see
	if they match since there's such a short list */
	for(int player = FIRST_PLAYER; player < MAX_PLAYERS; player++)
	for(int button = 0; button < TOTAL_PLAYER_BUTTONS; button++)
	{
		if(controls[player][button] == code)
		{
			SetButtonPosition(player, button, true);
			//break;
		}
		else if(controls[player][button] == otherCode)
		{
			SetButtonPosition(player, button, false);
		}
	}
}

PlayerInput *Controls::GetPlayerInput(unsigned int playerNo)
{
	playerNo--;

	return &playerInput[playerNo];
}

bool Controls::IsMouseButtonClicked(int buttonNo)
{
	if(buttonNo == 1) return mouseButtons[0].IsPressed();
	if(buttonNo == 2) return mouseButtons[1].IsPressed();
	if(buttonNo == 3) return mouseButtons[2].IsPressed();
	else return false;
} 

/* return radian angle to mouse position*/
double Controls::GetAngleToMouse( double srcX, double srcY)
{
	return atan2((double)srcY - double(mousePositions[1]), double(mousePositions[0]) - (double)srcX);
	//return atan2(double(mousePositions[1]) - (double)srcY, double(mousePositions[0]) - (double)srcX);
}

double Controls::MouseX(){ return mousePositions[0]; }
double Controls::MouseY(){ return mousePositions[1]; }

void Controls::SetButtonPosition(int player, int button, bool isDown)
{
	//for(int player = FIRST_PLAYER; player < MAX_PLAYERS; player++)
	if(button >= 0 && button < TOTAL_PLAYER_BUTTONS)
	{
		if(isDown) playerInput[player].buttons[button].Down();
		if(!isDown) playerInput[player].buttons[button].Up();
	}
}

void Controls::HandlePlayerControls(ALLEGRO_EVENT &ev)
{
	switch(ev.type)
	{
	case ALLEGRO_EVENT_KEY_DOWN: ReportKeyDown(ev.keyboard.keycode);
		break;
	case ALLEGRO_EVENT_KEY_UP: ReportKeyUp(ev.keyboard.keycode);
		break;
	case ALLEGRO_EVENT_KEY_CHAR: ReportKeyPress(ev.keyboard.keycode);
		break;
	case ALLEGRO_EVENT_MOUSE_AXES: ReportMousePosition(ev.mouse.x, ev.mouse.y);
		break;
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: ReportMouseButtonDown(ev.mouse.button);
		break;
	case ALLEGRO_EVENT_MOUSE_BUTTON_UP: ReportMouseButtonUp(ev.mouse.button);
		break;
	default:break;
	};
}
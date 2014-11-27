#ifndef CONTROLS_HPP
#define CONTROLS_HPP

union ALLEGRO_EVENT;

#include "PlayerInput.hpp"

namespace gauntlet
{

enum PLAYER_NUMBERING
{
	FIRST_PLAYER = 0,
	PLAYER_ONE = 1,
	PLAYER_TWO = 2,
	PLAYER_THREE = 3,
	PLAYER_FOUR = 4,
	MAX_PLAYERS = PLAYER_FOUR
};


/* Set player input by feeding raw input during Input
and querying the results in Process as game-related controls*/
class Controls
{
public:
	/* Sets up player's default controls */
	Controls();
	/* Does nothing*/
	~Controls();
	/* Currently does nothing, will load controls from file*/
	void Initialize();
	/* Currently does nothing, a future version might save settings*/
	void Destroy();

	void HandlePlayerControls(ALLEGRO_EVENT &ev);
	void ReportKeyDown(int keycode);
	void ReportKeyUp(int keycode);
	void ReportKeyPress(int keycode);
	void ReportMouseButtonDown(int button);
	void ReportMouseButtonUp(int keycode);
	void ReportMousePosition(int x, int y);
	void AdjustMouseZPosition(bool isIncrementing);
	PlayerInput *GetPlayerInput(unsigned int playerNo = 1);

	bool IsMouseButtonClicked(int buttonNo);
	/* return radian angle to mouse position*/
	double GetAngleToMouse( double srcX, double srcY);
private:

	void SetButtonPosition(int player, int button, bool isDown);

	/* Default controls for player 1*/
	static const int DEFAULT_CONTROLS[TOTAL_PLAYER_BUTTONS];

	/* lists input keycodes for each player*/
				 int		controls[MAX_PLAYERS][TOTAL_PLAYER_BUTTONS];

	Button mouseButtons[3];
	int mousePositions[2];
	PlayerInput playerInput[MAX_PLAYERS];


};

};

#endif
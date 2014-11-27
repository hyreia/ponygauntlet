#ifndef GAUNTLET_APP_HPP
#define GAUNTLET_APP_HPP

#include <memory>

#include "Allegro.hpp"
#include "Sound.hpp"
#include "Controls.hpp"
#include "GameDataRepository.hpp"
#include "Players.hpp"

namespace gauntlet
{
	class GameScreen;

	class GauntletApp
	{
	public:

		/* in charge of timer, display and event queue and basic Window setup */
		Allegro allegro;
		/* Container class for music and sfx classes, app->sound.sfx, app->sound.music*/
		Sound sound;

		/* Stores map from raw input to gameplay controls */
		Controls controls;
		/* Stores persistent information about the players */
		Players players;
		GameDataRepository data;


		/* Zeroes out and NULLs members, object members are initialized */
		GauntletApp();
		/* Does nothing */
		~GauntletApp();

		/* Starts all subsystems */
		void Initialize();
		/* Frees information from all subsystems */
		void Destroy();
		/* Sets up initial state and begins gameplay, does not return until app has been quit by user */
		void Run();

		void SetIsTimeToQuit();

		void ChangeToNewScreen(GameScreen *newScreen);

		/* Title displayed in title bar */
		static const char *title;
		 /* logical ticks per second */
		static const int TPS;
		/* delta time; time between logical updates (1.0/TPS) */
		static const double dt;
	private:

		/* time of program where we started tracking time */
		double startTimeOffset; 
		/* time since beginning of program time, in seconds*/
		double gameTime; 

		/* Current GameScreen being updated and drawn */
		GameScreen *currentScreen;

		/* Prepares game for main loop; loads first screen*/
		void StartFirstScreen();

		/* Calls handle events and output until time to quit*/
		void MainLoop();

		/* Handles all allegro events, including timer tick which calls Update. 
	     * Does not return until event queue is empty */
		void HandleEvents();

		/* Draws screen as it currently is, using dt to adjust screens accordingly */
		void Output();

		/* Updates current screen */
		void Update();

		/* When true the main loop ends and Run() returns*/
		bool isTimeToQuit;


	};

};

	//global instance of app class intended to be only copy
	extern std::unique_ptr<gauntlet::GauntletApp> app;

//};


#endif
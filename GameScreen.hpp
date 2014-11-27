#ifndef GAME_SCREEN_HPP
#define GAME_SCREEN_HPP

union ALLEGRO_EVENT;

namespace gauntlet
{
	class GameScreen
	{
		public:
			virtual void HandleInput(ALLEGRO_EVENT &ev) = 0;
			virtual void Render(double alpha) = 0;
			virtual void Update() = 0;
			virtual ~GameScreen(){}
		protected:
			GameScreen(){}

	};
};

#endif
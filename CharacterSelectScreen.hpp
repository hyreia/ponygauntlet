#ifndef CHARACTER_SELECT_SCREEN_HPP
#define CHARACTER_SELECT_SCREEN_HPP

#include "GameScreen.hpp"
#include <memory>
#include "XYPair.hpp"
#include "RectAndCircle.hpp"

namespace gauntlet
{
	class Bitmap;

	class CharacterSelectScreen: public GameScreen
	{
	public:
		CharacterSelectScreen();
		~CharacterSelectScreen();

		void HandleInput(ALLEGRO_EVENT &ev);
		void Render(double alpha);
		void Update();
	private:
		std::shared_ptr<Bitmap> background;
		std::shared_ptr<Bitmap> archerBitmask;
		std::shared_ptr<Bitmap> warriorBitmask;
		std::shared_ptr<Bitmap> wizardBitmask;
		std::shared_ptr<Bitmap> valkyrieBitmask;

		XYPair<double> backdropPos;
		int characterSelected;
		Rect<double> startButton;
		Rect<double> backButton;
		XYPair<double> descriptionPos;
		const XYPair<double> BUTTON_SIZE;
	};

};

#endif
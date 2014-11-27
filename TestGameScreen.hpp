#ifndef TEST_GAME_SCREEN_HPP
#define TEST_GAME_SCREEN_HPP


#include "GameScreen.hpp"
#include "LevelMap.hpp"
#include "EntityBatchRenderer.hpp"
#include "ObjectField.hpp"
#include <memory>

namespace gauntlet
{
	class Bitmap;
	class GameCharacter;
	class GameEntity;

	class TestGameScreen: public GameScreen
	{
	public:
		TestGameScreen();
		~TestGameScreen();

		void HandleInput(ALLEGRO_EVENT &ev) override;

		void Render(double alpha) override;
		
		void Update() override;
		void UpdateCharactersFromPlayerInput();
		void PerformCollisionsForPlayerCharacters();

		void QueueEntityToBeDrawn(GameEntity *entity);
		void DrawQueuedEntities();

	private:
		
		LevelMap map;
		ObjectField objectField;

		GameCharacter *character;
		GameCharacter *entity;

		std::shared_ptr<Bitmap> temp;
		//EntityBatchRenderer entityRenderer;

		//Actionbar
	};
};

#endif
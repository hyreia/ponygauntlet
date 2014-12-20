#ifndef TEST_GAME_SCREEN_HPP
#define TEST_GAME_SCREEN_HPP


#include "PlayerCharacter.hpp"
#include "Monster.hpp"
#include "GameScreen.hpp"
#include "LevelMap.hpp"
#include "EntityBatchRenderer.hpp"
#include "ObjectField.hpp"
#include "RectAndCircle.hpp"
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
		void UpdateMonsterSpawners();
		void CreateSpawners();

		void CalculateTilesInView();
		void FindMonstersAndSpawnersInView();

		void PerformCollisionsForCharacters();
		void UpdateMonstersFromAI();
		void PerformCollisionsForMonsters();
		/* Turns position into closest position of player. If no player
		is loaded (transitioning to next screen?) position stays the same.
		Returns: index of player (from Players vector) or -1 if no player
		is available */
		int GetPositionOfClosestPlayer(XYPair<double> &position);
		bool FoundOpenTileForMonsterSpawner(int &x, int &y);
		void QueueEntityToBeDrawn(GameEntity *entity);
		void DrawQueuedEntities();

	private:
		
		//Init list
		void CreatePlayerCharactersFromPlayers();

		LevelMap map;
		ObjectField objectField;

		Rect<double> cameraOffset;

		XYPair<int> firstTileInView;
		XYPair<int> lastTileInView;

		std::vector<PlayerCharacter*> playerCharacters;
		std::vector<Monster*> monstersInView;
		std::vector<Spawner*> spawnersInView;

		double timeUntilNextSpawnerIsCreated;
		double intervalBetweenMonsterSpawners;
		//GameCharacter *character;
		//GameCharacter *entity;

		//Actionbar

	};
};

#endif
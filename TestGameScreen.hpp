#ifndef TEST_GAME_SCREEN_HPP
#define TEST_GAME_SCREEN_HPP

#include <memory>
#include <tuple>
#include <map>

#include "PlayerCharacter.hpp"
#include "Monster.hpp"
#include "GameScreen.hpp"
#include "LevelMap.hpp"
#include "EntityBatchRenderer.hpp"
#include "ObjectField.hpp"
#include "RectAndCircle.hpp"
#include "AttackType.hpp"

#include "ActionBar.hpp"
#include "SpriteSheetAnimation.hpp"

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

	private:
		
		/* Init list */
			void CreatePlayerCharactersFromPlayers();
		
		/* Update list */
			void RebuildActionBar();
			void UpdateCharactersFromPlayerInput();
			void UpdateMonstersFromAI();
			void PerformCollisionsForCharacters();
			void CalculateTilesInView();
			void FindMonstersAndSpawnersInView();
			void UpdateAnimations();
			void UpdateMonsterSpawners();
			void CreateSpawners();
			void UpdateAttacks();

		/* Render list */
			void RenderHUD();

		/* Update helper functions */
			/* Turns position into closest position of player. If no player
			is loaded (transitioning to next screen?) position stays the same.
			Returns: index of player (from Players vector) or -1 if no player
			is available */
			int GetPositionOfClosestPlayer(XYPair<double> &position);
			bool FoundOpenTileForMonsterSpawner(int &x, int &y);		
			void AddAnimationToScreen(XYPair<double> offset, double angle, SpriteSheetAnimationType *type, GameCharacter *character);
			void CreateNewAttack(GameCharacter *attacker, double attackerPower, AttackType *type);
		



		/* Data members */
		LevelMap map;
		ObjectField objectField;


		Rect<double> cameraOffset;

		XYPair<int> firstTileInView;
		XYPair<int> lastTileInView;

		std::vector<PlayerCharacter*> playerCharacters;
		std::vector<Monster*> monstersInView;
		std::vector<Spawner*> spawnersInView;
		std::vector<Attack*> attacksInView;

		//animations underneath characters
		std::vector< AnimationOnGameCharacter> animationsOnFloor;
		//animations above characters
		std::vector< AnimationOnGameCharacter> animationsInSky;
		//std::vector<SpriteSheetAnimation> looseAnimations;

		double timeUntilNextSpawnerIsCreated;
		double intervalBetweenMonsterSpawners;
		//GameCharacter *character;
		//GameCharacter *entity;

		//Actionbar
		std::shared_ptr<Bitmap> actionHUD;
		ActionBar actionBar;
		


	};
};

#endif
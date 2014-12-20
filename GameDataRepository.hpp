#ifndef GAME_DATA_REPOSITORY_HPP
#define GAME_DATA_REPOSITORY_HPP

#include <map>
#include <vector>

#include "PlayerCharacterType.hpp"
#include "AttackType.hpp"
#include "SpriteSheetAnimation.hpp"
#include "Monster.hpp"
#include "Spawner.hpp"

namespace gauntlet
{

class Bitmap;

/* Stores static game data that has no logic, just data that is used by the rest of the game */
struct GameDataRepository
{
	/* Initializes the data, some data loads bitmaps so do not call this Initialize until allegro has been initialized */
	void Initialize();

	/* Free data */
	void Destroy();

	std::vector<PlayerCharacterType> playerCharacterTypes;
	std::vector<SpriteSheetAnimationType> spriteSheetAnimationTypes;
	std::map<int, AttackType> attackTypes;
	std::map<int, MonsterType>			monsterTypes;
	std::map<int, SpawnerType> spawnerTypes;
private:
	void CreatePlayerCharacterTypes();
	void CreateAnimationTypes();
	void CreateAttackTypes();
	void CreateSpawnerTypes();
	void CreateMonsterTypes();

};

};
#endif
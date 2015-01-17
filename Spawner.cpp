#include "Spawner.hpp"
#include "GauntletApp.hpp"

using namespace gauntlet;

Spawner::Spawner(SpawnerType *_type, int tileX, int tileY):
TileObject(tileX, tileY),
type(_type),
spawnsLeft(_type->spawnLimit),
spawnCooldown(_type->spawnCooldownRate)
{
			spawnsLeft = type->spawnLimit;
			int animationListSize = app->data.spriteSheetAnimationTypes.size();
			if(type->idleAnimationIndex < animationListSize)
			{
				SpriteSheetAnimation idle(&(app->data.spriteSheetAnimationTypes[type->idleAnimationIndex]));
				animations.push_back(idle);
			}
			if(type->spawningAnimationIndex < animationListSize)
			{
				SpriteSheetAnimation spawn(&(app->data.spriteSheetAnimationTypes[type->spawningAnimationIndex]));
				animations.push_back(spawn);
			}
			if(type->damagedAnimationIndex < animationListSize)
			{
				SpriteSheetAnimation damaged(&(app->data.spriteSheetAnimationTypes[type->damagedAnimationIndex]));
				animations.push_back(damaged);
			}


}

/* Returns new spawnCooldown value */
int Spawner::DecrementSpawnCooldown()
{
	if(spawnCooldown > 0)
	{
		spawnCooldown -= app->dt;
		return spawnCooldown;
	}
	else return 0;
}

void Spawner::ResetSpawnCooldown()
{
	spawnCooldown = type->spawnCooldownRate;
}

bool Spawner::WillBlockMovement()
{ 
	return type->isMovementImpedingWall; 
}

void Spawner::DecrementSpawnsLeft()
{
	if(spawnsLeft > 0) --spawnsLeft;
}

int Spawner::GetSpawnsLeft()
{
	return spawnsLeft;
}


XYPair<double> Spawner::GetCenterOfTile(){ return centerPos; }

MonsterType *Spawner::GetMonsterTypeToSpawn()
{
	MonsterType *ret = type->monsterList[0];

	const int NUM_OF_MONSTER_TYPES = type->monsterList.size();
	//common case
	if(NUM_OF_MONSTER_TYPES == 1) ret = type->monsterList[0];


	else if(type->monsterSelectionTechnique == SPAWNER_MONSTER_SELECT_CHANGES_ON_DAMAGE)
	{
		double durabilityPercent = (double)durabilityLeft/(double)type->durability;
		int indexOfNextMonstersType = (int)((double)NUM_OF_MONSTER_TYPES*durabilityPercent);
		ret = type->monsterList[indexOfNextMonstersType];
	}
	else if(type->monsterSelectionTechnique == SPAWNER_MONSTER_SELECT_IS_RANDOM)
	{
		int indexOfNextMonstersType = rand()%NUM_OF_MONSTER_TYPES;
		ret = type->monsterList[indexOfNextMonstersType];
	}


	return ret;
}
#ifndef SPAWNER_HPP
#define SPAWNER_HPP

#include "TileObject.hpp"

namespace gauntlet
{
	struct MonsterType;

	struct SpawnerType
	{
		SpawnerType(){}
		~SpawnerType(){}

		std::vector<MonsterType*> monsterList;
		int monsterSelectionTechnique; 

		int idleAnimationIndex;
		int spawningAnimationIndex;
		int damagedAnimationIndex;

		int spawnLimit;
		int durability;
		double spawnCooldownRate;
		bool isMovementImpedingWall;
	};

	enum MONSTER_SELECTION_STYLE
	{
		 //as damage is taken, selection ascends the list
		SPAWNER_MONSTER_SELECT_CHANGES_ON_DAMAGE,
		//monster spawned is rand() from list
		SPAWNER_MONSTER_SELECT_IS_RANDOM,
		//random, but monsters at position x is twice as common as monster as position x+1
		SPAWNER_MONSTER_SELECT_IS_RANDOM_BUT_WEIGHTED 
							
	};

	/* Monster spawner class. Creates a steady stream of monsters
	 *
	 * TODO: this is a stub
	*/
	class Spawner: public TileObject
	{
	public:
		Spawner(SpawnerType *_type, int tileX, int tileY):
		TileObject(tileX, tileY),
		type(_type)
		{
			spawnCooldown = 0.0;
			spawnsLeft = type->spawnLimit;
		}

		bool AreSpawnsLeft(){ !(spawnsLeft==0); }
	private:
		SpawnerType *type;
		int spawnsLeft;
		int durabilityLeft;
		double spawnCooldown;

	};
};

#endif
#ifndef SPAWNER_HPP
#define SPAWNER_HPP

#include "TileObject.hpp"

namespace gauntlet
{
	enum TEMPORARY_SPAWNER_LIST
	{
		SHALLOW_GRAVE
	};

	struct MonsterType;

	const int NO_SPAWN_LIMIT = -1;

	struct SpawnerType
	{
		SpawnerType(){}
		~SpawnerType(){}

		std::vector<MonsterType*> monsterList;
		int monsterSelectionTechnique; 

		int idleAnimationIndex;
		int spawningAnimationIndex;
		int damagedAnimationIndex;

		/* number of monsters it can spawn until it destroys itself, -1 for unlimited*/
		int spawnLimit;

		/* spawner starting/max ife*/
		int durability;
		
		/* in seconds */
		double spawnCooldownRate;
		
		/* can be walked on top of, 
			side effect: monsters don't spawn on top of but to the side of*/
		bool isMovementImpedingWall;
	};

	enum MONSTER_SELECTION_STYLE
	{
		 //as damage is taken, selection ascends the list
		SPAWNER_MONSTER_SELECT_CHANGES_ON_DAMAGE,
		//monster spawned is rand() from list
		SPAWNER_MONSTER_SELECT_IS_RANDOM
		//SPAWNER_MONSTER_SELECT_IS_RANDOM_BUT_WEIGHTED 
							
	};

	/* Monster spawner class. Creates a steady stream of monsters
	 *
	 * TODO: this is a stub
	*/
	class Spawner: public TileObject
	{
	public:
		Spawner(SpawnerType *type, int tileX, int tileY);

		/* Returns new spawnCooldown value */
		int DecrementSpawnCooldown();
		void ResetSpawnCooldown();
		bool WillBlockMovement();

		void DecrementSpawnsLeft();
		int GetSpawnsLeft();

		double InflictDamage(int durabilityLost);
		void GetDurabilityLeft();

		XYPair<double> GetCenterOfTile();

		MonsterType *GetMonsterTypeToSpawn();

		SpawnerType *type;
		int spawnsLeft;
		int durabilityLeft;
	private:

		double spawnCooldown;


	};
};

#endif
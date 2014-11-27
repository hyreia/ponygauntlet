#ifndef COLLISION_GRID_CELL_HPP
#define COLLISION_GRID_CELL_HPP

#include <vector>

namespace gauntlet
{
	class Monster;
	class PlayerCharacter;
	class Attack;

	struct CollisionGridCell
	{

		CollisionGridCell(){}
		~CollisionGridCell(){}

		std::vector<Monster*> monstersOverlappingCell;
	};

};

#endif
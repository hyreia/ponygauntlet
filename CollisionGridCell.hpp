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
		void Remove(Monster *monster)
		{
			auto monsterIter = monstersOverlappingCell.begin();
			while(true)
			{
				if(monsterIter == monstersOverlappingCell.end()) break;


				if(monster == (*monsterIter))
				{
						monstersOverlappingCell.erase( monsterIter );
						break;
				}
				else monsterIter++;
			};
		}
		void Insert(Monster *monster)
		{
			monstersOverlappingCell.push_back(monster);
		}

		void Insert(PlayerCharacter *player)
		{
			playersOverlappingCell.push_back(player);
		}

		void Remove(PlayerCharacter *player)
		{
			auto playerIter = playersOverlappingCell.begin();
			while(true)
			{
				if(playerIter == playersOverlappingCell.end()) break;


				if(player == (*playerIter))
				{
						playersOverlappingCell.erase( playerIter );
						break;
				}
				else playerIter++;
			};
		}


		CollisionGridCell(){}
		~CollisionGridCell(){}

		std::vector<Monster*> monstersOverlappingCell;
		std::vector<PlayerCharacter*> playersOverlappingCell;
	};

};

#endif
#ifndef BREAKABLE_WALL_HPP
#define BREAKABLE_WALL_HPP

#include "TileObject.hpp"

namespace gauntlet
{
	class BreakableWall: public TileObject
	{
	public:
		BreakableWall(int tileX, int tileY, int durability = 1, int trigger = 0):
		TileObject(tileX, tileY),
		durabilityLeft(durability),
		destroyTrigger(trigger)
		{}
		
		~BreakableWall();
		int GetDurabilityLeft(){ return durabilityLeft; }
		void Damage(int damageAmount){ durabilityLeft -= damageAmount; }
		bool HasTrigger(){ return destroyTrigger!=0; }
		int GetTriggerId(){ return destroyTrigger; }
	private:
		int durabilityLeft;
		int destroyTrigger;
	};
};


#endif
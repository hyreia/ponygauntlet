#ifndef LEVER_HPP
#define LEVER_HPP

#include "TileObject.hpp"

namespace Gauntlet
{
	class Lever: public TileObject
	{
	public:
		Lever(int triggerId, int tileX, int tileY):
		TileObject(tileX, tileY),
		trigger(triggerId)
		{}
		/* returns trigger id*/
		int Flip(){ isFlipped = true; return trigger; }
		bool IsFlipped(){ return isFlipped; }
		int GetTriggerIdWithoutFlipping(){ return trigger; }
	private:
		int trigger;
		bool isFlipped;
	};
};

#endif
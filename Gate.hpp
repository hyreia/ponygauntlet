#ifndef GATE_HPP
#define GATE_HPP

#include "TileObject.hpp"

namespace gauntlet
{
	/* TODO: this is a stub*/
	class Gate: public TileObject
	{
	public:
		Gate(int tileX, int tileY, int trigger = 0):
		TileObject(tileX, tileY),
		openTrigger(trigger)
		{}

		bool willKeyOpenGate(){ return openTrigger==0; }
		int GetTrigger(){ return openTrigger; }

	private:
		/* 0 = key works, >0 needs a lever of that # to trigger */
		int openTrigger;
	};
};

#endif
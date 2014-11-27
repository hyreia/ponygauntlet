#ifndef HAZARD_HPP
#define HAZARD_HPP
#include "TileObject.hpp"


namespace gauntlet
{
	/* Hazards are special walkable tiles that */

	enum HAZARD_EFFECT
	{
		//effectAmount = attackType
		HAZARD_EFFECT_ATTACK, 
		//effectAmount = seconds stunned
		HAZARD_EFFECT_STUN,	
		//effectAmount = trigger causeing changes
		HAZARD_EFFECT_TRAP,	
		//effectAmount N/A, looks forward for a teleport, then sweeps
		//until it becomes random
		HAZARD_EFFECT_TELEPORT_FORWARD, 
		//effectAmount N/A, moves to any nearby teleport
		HAZARD_EFFECT_TELEPORT_RANDOM	  
	};

	struct HazardType
	{
		int effect; //HAZARD_EFFECT
		double effectAmount;
		double onTime;	//time trap is on
		double offTime; //time trap is off, 0 == always on
		bool isGroundBased; //not effect by charaters with altitude
		int cooldown; //time until it can hurt again, N/A for non-damage types
		std::shared_ptr<Bitmap> bitmap;
	};

	/* TODO: this is stub */
	class Hazard: public TileObject
	{
	public:
		Hazard(HazardType *_type, int tileX, int tileY):
		TileObject(tileX, tileY),
		type(_type),
		timeInCycle(0)
		{}
		int GetEffect();
		int GetEffectAmount();
		
		/* Adds time to calculation timeInCycle */
		void AddTime(double time)
		{
			timeInCycle += time;
			if(timeInCycle > type->onTime+type->offTime)
			{
				timeInCycle -= type->onTime+type->offTime;
			}
		}
		bool IsOn()
		{
			return (timeInCycle > type->offTime);
		}
	private:
		HazardType *type;
		/*
			If timeInCycle < offTime, hazard is off
			If timeInCycle < offtime+onTime, hazard is on
			if timeInCycle > offtime+onTime, time is reset to 0
		*/
		double timeInCycle;

	};
};

#endif
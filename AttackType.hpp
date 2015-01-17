#ifndef ATTACK_TYPE_HPP
#define ATTACK_TYPE_HPP

namespace gauntlet
{
	enum ATTACK_TYPE_ATTRIBUTES
	{
		/* if true, it moves when the player moves */
		IS_MELEE							= 1 << 0,
		/* if true, when a character is hit with it, they will get moved back */
		KNOCKS_BACK							= 1 << 1,
		/*  is destroyed when it hits characters and causes them damage */
		DESTROYED_ON_HIT					= 1 << 2,
		/* each logic frame it makes sure it’s facing is the same as the player, 
		 * it also resets it’s velocity to be forward 
		 * (in the direction the player is facing) */
		MOVES_IN_ATTACKERS_FACING_DIRECTION = 1 << 3,
		/* if a characters gets hit with it, it stunts them for a short time */
		WILL_STUN							= 1 << 4,
		/* while active, prevents attacker from moving (Zeroes velocity) */
		PREVENTS_MOVEMENT					= 1 << 5,
		/* while active, prevents attacker from rotating (resets velocity) */
		PREVENTS_ROTATING					= 1 << 6,
	};

	enum ATTACK_TYPE_PATH
	{
		PATH_AROUND_CHARACTER,
		PATH_FORWARD,
		PATH_NON_MOVING_LINE
	};

	struct AttackType
	{
		AttackType(){}

		AttackType(int pathType,
			int lifetime,
			double energyUsed,
			double size,
			double damageMultiplier,
			double speedMultiplier,
			int projectileAnimationIndex,
			int attackerAnimationIndex,
			int attackedAnimationIndex,
			unsigned int attributes):
		PATH_TYPE(pathType),
		LIFETIME(lifetime),
		ENERGY_USED(energyUsed),
		SIZE(size),
		DAMAGE_MULTIPLIER(damageMultiplier),
		SPEED_MULTIPLIER(speedMultiplier),
		ANIMATION_ON_PROJECTILE_INDEX(projectileAnimationIndex),
		ANIMATION_ON_ATTACKER_INDEX(attackerAnimationIndex),
		ANIMATION_ON_ATTACKED_INDEX(attackedAnimationIndex),
		ATTRIBUTES(attributes)
		{}

		bool HasAttribute(int attribute)
		{
			return (ATTRIBUTES	& attribute) != 0;
		}

		unsigned int ATTRIBUTES;
		int PATH_TYPE;
		/* Lifetime in logic frames, -1 for unlimited. Not valid with NON_MOVING_LINE path */
		int LIFETIME;
		double ENERGY_USED;
		double SIZE;
		double DAMAGE_MULTIPLIER;
		double SPEED_MULTIPLIER;
		int ANIMATION_ON_PROJECTILE_INDEX;
		int ANIMATION_ON_ATTACKER_INDEX;
		int ANIMATION_ON_ATTACKED_INDEX;
	};



};

#endif
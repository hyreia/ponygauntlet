#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "GameCharacter.hpp"

namespace gauntlet
{
	enum TEMPORARY_MONSTER_LIST
	{
		SPOOKY_BONES
	};

	enum MONSTER_AI_TYPE
	{
		BASIC_MONSTER_AI
	};
	
	struct MonsterType
	{
		double maxHealth;
		double maxEnergy;
		int defaultAttackIndex;
		double radius;
		double height;
		double startAltitude;
		std::shared_ptr<Bitmap> bitmap;
		double imageWidth, imageHeight;
		double moveSpeed;
		int aiTypeIndex, thinkCooldown;
	};

	class Monster: public GameCharacter
	{
	public:
		Monster(XYPair<double> position, int startFacing, MonsterType &type);

		int GetAiType();
		void DecrementThinkCooldown();
		bool IsAbleToThink();
		void ResetThinkCooldown();

		void LookAt(XYPair<double> position);
		void LookAwayFrom(XYPair<double> position);
		void SetVelocityToForward();
		void SetVelocityToDirection(double radian);

		void AddStepSpeedToAnimation();
	private:
		MonsterType *type;
		int health, energy;

		int thinkFrameCooldown;
	};


};

#endif
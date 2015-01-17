#ifndef ATTACK_HPP
#define ATTACK_HPP

#include "GameEntity.hpp"
#include "RectAndCircle.hpp"
#include "SpriteSheetAnimation.hpp"

namespace gauntlet
{
	struct AttackType;
	class GameCharacter;
	struct AttackType;

	class Attack
	{
	public:
		Attack(unsigned int attackerId, XYPair<double> position, double size, 
			double angle,
			AttackType *type,
			double damage, double speed,
			std::shared_ptr<Bitmap> bitmap,
			SpriteSheetAnimation *animation);
		
		/* Moved appropriately for its path*/
		void UpdatePosition();
		void DecrementTimeLeft();
		bool IsExpired();
		void ZeroOutTimeLeft();

		XYPair<double> GetVelocity();

		double GetFacing();
		void SetFacing(double facing);
		unsigned int GetAttributes();

		Rect<double> GetHitboxAtDeltaTime(double alpha);
		Rect<double> GetImageBoxAtDeltaTime(double alpha);

		/* Used to interpolate drawing position when drawing between logic frames*/
		XYPair<double> lastPosition;

		double facingAngle;

		int timeLeft;
		double speed;
		double damage;
		unsigned int attackerId;
		SpriteSheetAnimation *animation;

		/* If attack is a line, it's position is its ending position*/
		Rect<double> hitbox;
		std::shared_ptr<Bitmap> imageBitmap;
		AttackType *type;

	};
};

#endif
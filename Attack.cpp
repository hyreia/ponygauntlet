#include "Attack.hpp"
#include "AttackType.hpp"
#include "GameCharacter.hpp"
#include "GauntletApp.hpp"

using namespace gauntlet;

Attack::Attack(unsigned int id, XYPair<double> position, double size, 
	double angle,
	AttackType *_type,
	double _damage, double _speed,
	std::shared_ptr<Bitmap> _bitmap, 
	SpriteSheetAnimation *_animation):
	type(_type),	
	hitbox(position.x - size/2, position.y - size/2, size, size),
	imageBitmap(_bitmap),
	damage(_damage),
	speed(_speed),
	attackerId(id),
	animation(_animation),
	timeLeft(_type->LIFETIME)	
{
	lastPosition = position;
	facingAngle = angle;
}

double Attack::GetFacing()
{
	return facingAngle;
}

void Attack::SetFacing(double facing)
{
	facingAngle = facing;
}

Rect<double> Attack::GetHitboxAtDeltaTime(double alpha)
{
	return Rect<double>(
			(lastPosition.x + ((hitbox.x)-lastPosition.x) * alpha),
			(lastPosition.y + ((hitbox.y)-lastPosition.y) * alpha),
			hitbox.w, hitbox.h);
}

XYPair<double> Attack::GetVelocity()
{
	return XYPair<double>(lastPosition.x - hitbox.x,
		lastPosition.y - hitbox.y);
}

void Attack::UpdatePosition()
{
	//TODO: move things that need moving
	if(type->PATH_TYPE == PATH_FORWARD)
	{
		//move forward
		double distance = (type->SPEED_MULTIPLIER * speed/ app->TPS);
		XYPair<double> velocity;
		velocity.x = distance * cos(facingAngle);
		velocity.y = 0 - distance * sin(facingAngle);

		lastPosition.x = hitbox.x;
		lastPosition.y = hitbox.y;

		hitbox.x += velocity.x;
		hitbox.y += velocity.y;
	}
	else if(type->PATH_TYPE == PATH_AROUND_CHARACTER)
	{
		
		//add a calculation derived from speed to rotation that is around character
		
		//angle from attacker to attack

	}
	else// if(type->PATH_TYPE == PATH_NON_MOVING_LINE)
	{
		//position doesn't move, either because it represents a line's endpoint or has no path
	}
}

void Attack::DecrementTimeLeft()
{
	if(timeLeft > 0) timeLeft--;
}

bool Attack::IsExpired()
{
	return (timeLeft==0);
}

void Attack::ZeroOutTimeLeft()
{
	timeLeft = 0;
}
#include "GameCharacter.hpp"
#include <allegro5/allegro.h>

using namespace gauntlet;

GameCharacter::GameCharacter(XYPair<double> _position, double _radius,
	std::shared_ptr<Bitmap> _bitmap,
	Rect<double> _imageSource,
	int _startFacing,
	double _height,
	double _altitude):
GameEntity(Circle<double>(_position.x, _position.y, _radius), _bitmap,_imageSource),
	altitude(_altitude),
	height(_height),
	animationFacing(_startFacing),
	animation(IDLE_ANIMATION),
	stepsInAnimation(0.0),
	animationTime(0),
	facingAngle(0.0),
	numOfFacingAngleLocks(0),
	numOfMovementLocks(0),
	lastPosition(_position.x, _position.y)
{
	//Set facingAngle from startFacing
	facingAngle = ((double)animationFacing/(double)TOTAL_DIRECTIONS) * 2*ALLEGRO_PI;
}
GameCharacter::~GameCharacter(){}

Rect<double> GameCharacter::GetImageBoxAtDeltaTime(double alpha)
{
	return Rect<double>(
		
		(lastPosition.x + ((hitbox.cx)-lastPosition.x) * alpha)+imageToHitboxOffset.x - hitbox.radius,
			(lastPosition.y + ((hitbox.cy)-lastPosition.y) * alpha)+imageToHitboxOffset.y - hitbox.radius,
			imageSize.x, imageSize.y);
}

void GameCharacter::SetVelocity(double x, double y)
{ 
	velocity.x = x; velocity.y = y;
}
	
XYPair<double> GameCharacter::GetVelocity()
{ 
	return velocity; 
}
	
void GameCharacter::SetLastPosition(XYPair<double> previousPosition)
{
	lastPosition.x = previousPosition.x; lastPosition.y = previousPosition.y;
}

Circle<double> GameCharacter::GetNextHitbox()
{
	return Circle<double>(hitbox.cx+velocity.x,hitbox.cy+velocity.y, hitbox.radius);
}

void GameCharacter::SetFacingAngle(double newFacing)
{
	facingAngle = newFacing;
}

double GameCharacter::GetFacing()
{ 
	return facingAngle; 
}

void GameCharacter::SetFacingFromRadian(double angle)
{
	facingAngle = angle;
		const float PI = acos(-1.0f);
		if(angle < 0)
			angle += 2*PI;

		
		/* Facing is determined by dividing the directions around them in 8 pie
		slices. If they're within a 45 degrees of that direction, they face that one. */
		/*To make finding angles easier, we add half of 45 degrees so our
		transitions between different angles is at 45 intervals starting at 0 
		instead of 22.5 degrees (PI/8)*/
		angle += (PI/8.0f);

		if(angle < PI/4.0)
			SetFacingDirection(DIR_RIGHT);
		else if(angle < PI/2.0)
			SetFacingDirection(DIR_UPRIGHT);
		else if(angle < 3*PI/4)
			SetFacingDirection(DIR_UP);
		else if(angle < PI)
			SetFacingDirection(DIR_UPLEFT);
		else if(angle < 5*PI/4)
			SetFacingDirection(DIR_LEFT);
		else if(angle < 3*PI/2)
			SetFacingDirection(DIR_DOWNLEFT);
		else if(angle < 7*PI/4)
			SetFacingDirection(DIR_DOWN);
		else if(angle < 2*PI)
			SetFacingDirection(DIR_DOWNRIGHT);
}

void GameCharacter::SetFacingDirection(int direction)
{
	if(direction >= FIRST_DIRECTION && direction < TOTAL_DIRECTIONS)
	{
		animationFacing = direction;
		//Keep the two consistent
		facingAngle = ((double)animationFacing/(double)TOTAL_DIRECTIONS) * 2*ALLEGRO_PI;
	}
	//else don't set facing to the invalid direction
}

void GameCharacter::SetAnimation(int newAnimation)
{
	animation = newAnimation;
}
	
void GameCharacter::UpdateAnimation()
{
	const double MOVING_ANIMATION_STARTING_COLUMN = 1.0;

	SetFacingFromRadian(facingAngle);
	imageDrawingFlags = DIRECTION_FLAGS[animationFacing];
	imageSource.y = imageSize.y*DRAW_ROW_BY_CHAR_DIRECTION[animationFacing];
	if(animation == MOVING_ANIMATION)
	{
		const double STEPS_IN_MOVING_ANIMATION = 6.0;
		if(stepsInAnimation >= STEPS_IN_MOVING_ANIMATION)
		{
			stepsInAnimation = stepsInAnimation - STEPS_IN_MOVING_ANIMATION;
		}

		imageSource.x = (MOVING_ANIMATION_STARTING_COLUMN + 
								floor(stepsInAnimation)) * imageSize.x;
	}
	else
	{
		//IDLE ANIMATION
		imageSource.x = 0;
	}
}

void GameCharacter::AddStepsToAnimation(double stepsAdded)
{
	if(stepsAdded < 0) stepsAdded = 0-stepsAdded;
	stepsInAnimation += stepsAdded;
}

double GameCharacter::GetHeight()
{ 
	return height;
}

double GameCharacter::GetAltitude()
{ 
	return altitude; 
}

void GameCharacter::SetAltitude(double newAltitude)
{ 
	altitude = newAltitude;
}

void GameCharacter::AddLockToFacingAngle()
{
	numOfFacingAngleLocks++;
}

void GameCharacter::RemoveLockFromFacingAngle()
{
	numOfFacingAngleLocks--;
}

bool GameCharacter::IsFacingAngleLocked()
{
	return (numOfFacingAngleLocks>0);
}

void GameCharacter::AddLockToMovement()
{
	numOfMovementLocks++;
}

void GameCharacter::RemoveLockFromMovement()
{
	numOfMovementLocks--;
}

bool GameCharacter::isMovementLocked()
{
	return (numOfMovementLocks>0);
}
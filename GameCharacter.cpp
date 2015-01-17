#include "GameCharacter.hpp"
#include <allegro5/allegro.h>
#include "Bitmap.hpp"

using namespace gauntlet;

double GameCharacter::drawAlpha = 0.0;

GameCharacter::GameCharacter(unsigned int _ID, XYPair<double> _position, XYPair<double> _size,
	std::shared_ptr<Bitmap> _bitmap,
	Rect<double> _imageSource,
	int _startFacing,
	double _height,
	double _altitude):
GameEntity( Rect<double>(_position.x, _position.y, _size.x, _size.y), _bitmap,_imageSource),
	altitude(_altitude),
	height(_height),
	animationFacing(_startFacing),
	animation(IDLE_ANIMATION),
	stepsInAnimation(0.0),
	animationTime(0),
	facingAngle(0.0),
	lastPosition(_position.x, _position.y),
	ID(_ID),
	timeLeftStunned(0.0),
	movementLocksFromAnAttack(0),
	rotationLocksFromAnAttack(0)
{
	//Set facingAngle from startFacing
	facingAngle = ((double)animationFacing/(double)TOTAL_DIRECTIONS) * 2*ALLEGRO_PI;
}
GameCharacter::~GameCharacter(){}

Rect<double> GameCharacter::GetImageBoxAtDeltaTime(double alpha)
{
	return Rect<double>(
		
		(lastPosition.x + ((hitbox.x)-lastPosition.x) * alpha)+imageToHitboxOffset.x,
			(lastPosition.y + ((hitbox.y)-lastPosition.y) * alpha)+imageToHitboxOffset.y + altitude,
			imageSize.x, imageSize.y);
}

unsigned int GameCharacter::GetID()
{
	return ID;
}

Rect<double> GameCharacter::GetHitboxAtDeltaTime(double alpha)
{
	return Rect<double>(
			(lastPosition.x + ((hitbox.x)-lastPosition.x) * alpha),
			(lastPosition.y + ((hitbox.y)-lastPosition.y) * alpha),
			hitbox.w, hitbox.h);
}

void GameCharacter::SetVelocity(double x, double y)
{ 
	velocity.x = x; velocity.y = y;
}
	
XYPair<double> GameCharacter::GetVelocity()
{ 
	return velocity; 
}

void GameCharacter::AddVelocityToPosition()
{
	hitbox.x += velocity.x;
	hitbox.y += velocity.y;
}

void GameCharacter::SetForcedVelocity(double x, double y){ forcedVelocity.x = x; forcedVelocity.y = y; }
XYPair<double> GameCharacter::GetForcedVelocity(){ return forcedVelocity; }

	
void GameCharacter::SetLastPosition(XYPair<double> previousPosition)
{
	lastPosition.x = previousPosition.x; lastPosition.y = previousPosition.y;
}

void GameCharacter::SetLastPositionToCurrentPosition()
{
	lastPosition.x = hitbox.x; lastPosition.y = hitbox.y;
}

void GameCharacter::SetYVelocity(double y){ velocity.y = y; }
void GameCharacter::SetXVelocity(double x){ velocity.x = x; }

Rect<double> GameCharacter::GetNextHitbox()
{
	return Rect<double>(hitbox.x+velocity.x,hitbox.y+velocity.y, hitbox.w, hitbox.h);
}

XYPair<double> GameCharacter::ProjectedNextPosition()
{
	return XYPair<double>(hitbox.x+velocity.x, hitbox.y+velocity.y);
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
		else
			SetFacingDirection(DIR_DOWNRIGHT);
}

void GameCharacter::SetFacingDirection(int direction)
{
	if(direction >= FIRST_DIRECTION && direction < TOTAL_DIRECTIONS)
	{
		animationFacing = direction;
		//Keep the two consistent
		//facingAngle = ((double)animationFacing/(double)TOTAL_DIRECTIONS) * 2*ALLEGRO_PI;
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

	if( (velocity.x != 0.0 || velocity.y != 0.0) && animation == IDLE_ANIMATION)
	{
		animation = MOVING_ANIMATION; stepsInAnimation = 0;
	}
	else if(velocity.x == 0.0 && velocity.y == 0.0 && animation == MOVING_ANIMATION)
	{
		animation = IDLE_ANIMATION; stepsInAnimation = 0;
	}

	SetFacingFromRadian(facingAngle);
	imageDrawingFlags = DIRECTION_FLAGS[animationFacing];
	imageSource.y = imageSize.y*DRAW_ROW_BY_CHAR_DIRECTION[animationFacing];
	if(animation == MOVING_ANIMATION)
	{
		
		const double STEPS_IN_MOVING_ANIMATION = imageBitmap->Width()/imageSize.x - 1.0;
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

double GameCharacter::GetRenderSortY()
{
	return (lastPosition.y + (hitbox.y-lastPosition.y) * drawAlpha) + hitbox.h;
}

void GameCharacter::AddForcedVelocityToVelocity()
{

}


void GameCharacter::LockMovementFromAnAttack()
{
	movementLocksFromAnAttack++;
}

void GameCharacter::UnlockMovementFromAnAttack()
{
	if(movementLocksFromAnAttack>0) movementLocksFromAnAttack--;
}

bool GameCharacter::IsMovementLockedFromAnAttack()
{
	return (movementLocksFromAnAttack>0);
}

void GameCharacter::LockRotationFromAnAttack()
{
	rotationLocksFromAnAttack++;
}

void GameCharacter::UnlockRotationFromAnAttack()
{
	if(rotationLocksFromAnAttack>0) rotationLocksFromAnAttack--;
}

bool GameCharacter::IsRotationLockedFromAnAttack()
{
	return (rotationLocksFromAnAttack>0);
}


bool GameCharacter::IsStunTimeLeft()
{
	return timeLeftStunned>0;
}

void GameCharacter::SetStunTime(double timeToStun)
{
	timeLeftStunned = timeToStun;
}

void GameCharacter::DecreaseStunTime(double timeToSubtract)
{
	timeLeftStunned -= timeToSubtract;
	if(timeLeftStunned < 0) timeLeftStunned = 0;
}


bool gauntlet::CompareCharactersByYAxis(GameCharacter *a, GameCharacter *b)
{
	return (a->GetRenderSortY() < b->GetRenderSortY());
}
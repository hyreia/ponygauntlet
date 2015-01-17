#include "Monster.hpp"
#include <allegro5/allegro.h>
#include "GauntletApp.hpp"

using namespace gauntlet;

Monster::Monster(unsigned int ID, XYPair<double> _centerStartPos, int startFacing, MonsterType &type):
type(&type),
GameCharacter(ID, _centerStartPos, 
				XYPair<double>(type.radius, type.radius), type.bitmap,
Rect<double>(0, 0, type.imageWidth, type.imageHeight), startFacing,
type.height, type.startAltitude),
energy(type.maxEnergy),
health(type.maxHealth),
thinkFrameCooldown(0)
{}

int Monster::GetAiType()
{
	return type->aiTypeIndex;
}

void Monster::DecrementThinkCooldown()
{
	if(thinkFrameCooldown > 0) thinkFrameCooldown--;
}

bool Monster::IsAbleToThink()
{
	return (thinkFrameCooldown == 0);
}

void Monster::ResetThinkCooldown()
{
	thinkFrameCooldown = type->thinkCooldown;
}

void Monster::LookAt(XYPair<double> position)
{
	auto center = hitbox.Center();
	double radian = atan2(center.y - position.y, position.x - center.x);
	SetFacingAngle(radian);				
}

void Monster::LookAwayFrom(XYPair<double> position)
{
	auto center = hitbox.Center();
	double radian = atan2(center.y - position.y, center.x - position.x);
	radian += ALLEGRO_PI;
	SetFacingAngle(radian);
}

void Monster::SetVelocityToForward()
{
	SetVelocityToDirection(facingAngle);
}

void Monster::SetVelocityToDirection(double radian)
{
	const double MULTIPLIER = 1.4;
	auto moveSpeed = (type->moveSpeed * MULTIPLIER)  / app->TPS;

	velocity.x = moveSpeed * cos(radian);
	//Cartesian plane has Y increase up, in-game Y increases DOWN
	velocity.y = 0 - moveSpeed * sin(radian); 
}

void Monster::AddStepSpeedToAnimation()
{
	const double MOVE_BASE_SPEED = 12.0;
	AddStepsToAnimation((type->moveSpeed * app->dt + MOVE_BASE_SPEED) / app->TPS);
}

void Monster::DecreaseHealth(double damage)
{
	health -= damage;
	if(health <0) health = 0;
}
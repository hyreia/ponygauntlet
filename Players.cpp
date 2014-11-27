#include "PlayerCharacterType.hpp"
#include "Players.hpp"

using namespace gauntlet;

Player::Player():
characterType(NULL),
health(100),
energy(100),
maxHealth(health),
maxEnergy(energy),
strength(0),
magic(0),
speed(0),
armor(0),
name("Player"),
score(0),
credit(0),
strengthMod(0),
magicMod(0),
armorMod(0),
speedMod(0),
isPlaying(false)
{}

Player::~Player(){}

void Player::SetStatsToDefaultForCharacterType()
{
	if(characterType)
	{
		strength = characterType->strength;
		magic = characterType->speed;
		speed = characterType->speed;
		armor = characterType->armor;
		maxHealth = strength+magic+speed+armor;
		health = maxHealth;
	}
}

void Player::SetNameToCharacterName()
{
	if(characterType)
	{
		name = characterType->defaultName;
	}
}


int Player::STR(){ return strength+strengthMod; }

int Player::MAG(){ return magic+magicMod; }

int Player::ARM(){ return armor+armorMod; }

int Player::SPD(){ return speed+speedMod; }
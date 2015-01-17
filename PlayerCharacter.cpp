#include "PlayerCharacter.hpp"

using namespace gauntlet;

PlayerCharacter::PlayerCharacter(unsigned int ID, XYPair<double> position, Player &player):
GameCharacter(ID, position, XYPair<double>(player.characterType->radius,player.characterType->radius), player.characterType->bitmap,
	Rect<double>(XYPair<double>(), player.characterType->imageSize),
	DIR_DOWN,
	player.characterType->height),
player(&player)
{}

Player &PlayerCharacter::GetPlayer()
{
	return *player;
}
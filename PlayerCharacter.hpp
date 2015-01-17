#ifndef PLAYER_CHARACTER_HPP
#define PLAYER_CHARACTER_HPP

#include "GameCharacter.hpp"
#include "Players.hpp"

namespace gauntlet
{
	class PlayerCharacter: public GameCharacter
	{
	public:
		PlayerCharacter(unsigned int ID, XYPair<double> position, Player &player);

		Player &GetPlayer();
	private:
		Player *player;
	};


};

#endif
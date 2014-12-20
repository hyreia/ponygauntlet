#ifndef PLAYERS_HPP
#define PLAYERS_HPP

#include <string>
#include "PlayerCharacterType.hpp"

namespace gauntlet
{
	struct PlayerCharacterType;

	struct Player
	{
	public:
		Player();
		~Player();

		bool isPlaying;

		PlayerCharacterType *characterType;

		double health, energy, maxHealth, maxEnergy;
		int strength, magic, speed, armor;
		int strengthMod, magicMod, speedMod, armorMod;
		/* Returns base + mod */
		int STR();
		/* Returns base + mod */
		int MAG();
		/* Returns base + mod */
		int ARM();
		/* Returns base + mod */
		int SPD();

		std::string name;
		int score;
		int credit;
		const size_t ID;

		/* Items that can be used as a secondary action from action bar*/
		std::vector<int> inventory;

		void SetStatsToDefaultForCharacterType();
		void SetNameToCharacterName();

		static size_t nextPlayerIDAvailable;
	};

	/* A place to keep players in the app */
	class Players
	{
	public:
		static const int MAX_PLAYERS = 4;
		Player &GetPlayer(int playerId)
		{
			if(playerId > 0 && playerId <= MAX_PLAYERS)
			{
				return player[playerId-1];
			}
			else return player[0];
		}
	private:
		Player player[MAX_PLAYERS];
	};
};

#endif
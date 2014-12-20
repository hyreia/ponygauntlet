#ifndef PLAYER_CHARACTER_TYPE_HPP
#define PLAYER_CHARACTER_TYPE_HPP

#include <string>
#include <memory>
#include "XYPair.hpp"
#include "Bitmap.hpp"

namespace gauntlet
{
	enum PLAYER_CHARACTER_TYPES
	{
		PC_WARRIOR,
		PC_WIZARD,
		PC_VALKYRIE,
		PC_ARCHER,
		NUM_OF_PLAYER_CHARACTER_TYPES
	};

	struct PlayerCharacterType
	{
		PlayerCharacterType(){}
		PlayerCharacterType(std::string _defaultName,
			std::string _typeName, std::shared_ptr<Bitmap> _bitmap,
			std::string _personalityDescription,
			std::string _abilitiesDescription,
			int _strength, int _magic, int _speed, int _armor,
			int _mainAction,
			XYPair<double> _imageSize,
			double _height,
			double _radius):
		strength(_strength),
		magic(_magic),
		speed(_speed),
		armor(_armor),
		defaultName(_defaultName),
		typeName(_typeName),
		bitmap(_bitmap),
		personalityDescription(_personalityDescription),
		abilitiesDescription(_abilitiesDescription),
		mainAction(_mainAction),
		radius(_radius),
		imageSize(_imageSize),
		height(_height)
		{}
		int strength, magic, speed, armor;

		unsigned int mainAction;
		std::vector<unsigned int> secondaryInherentActions;

		std::shared_ptr<Bitmap> bitmap;
		std::string defaultName;
		std::string typeName;
		std::string personalityDescription;
		std::string abilitiesDescription;
		XYPair<double> imageSize;
		double height;
		double radius;
	};
};

#endif
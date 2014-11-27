#ifndef EXIT_HPP
#define EXIT_HPP

#include "GameCharacter.hpp"
#include "SpriteSheetAnimation.hpp"

namespace gauntlet
{
	/* TODO: stub */
	class Exit: public GameEntity
	{
	public:
		Exit(XYPair<double> centerPos, std::string _destination,
			std::shared_ptr<Bitmap> bitmap,
			SpriteSheetAnimationType *animation):
		GameEntity(Circle<double>(centerPos.x, centerPos.y, 16), bitmap, Rect<double>(0,0,32,32)),
		exitAnimation(animation),
		destination(_destination)
		{}
		std::string GetDestination(){ return destination; }

	private:
		SpriteSheetAnimation exitAnimation;
		std::string destination;
	};
};

#endif
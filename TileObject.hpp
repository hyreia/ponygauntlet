#ifndef TILE_OBJECT_HPP
#define TILE_OBJECT_HPP

#include "SpriteSheetAnimation.hpp"

namespace gauntlet
{
	/* TileObjects are objects that are centered on tiles.
	In common they have animations, current animation and a time in current animation
	that helps figure out how they're drawn */
	class TileObject
	{
	public:
		std::shared_ptr<Bitmap> GetSourceBitmap();
		XYPair<double> ImageSource();
		Rect<double> GetImageBox();
		Rect<double> GetTileRect();

		void AddTimeToAnimation(double time);
	protected:
		TileObject(int tileX, int tileY);
		~TileObject();
		int currentAnimationIndex;
		std::vector<SpriteSheetAnimation> animations;
		XYPair<double> centerPos;
	};
};

#endif
#ifndef TILE_HPP
#define TILE_HPP

#include "RectAndCircle.hpp"

namespace gauntlet
{
	struct Tile
	{
	public:
		static const int TILE_SIZE = 32;

		/* Returns tile that would be at given pixel position*/
		static XYPair<int> PixelToTile(double pixelX, double pixelY)
		{
			return XYPair<int>(
			(int)floor(pixelX/TILE_SIZE), (int)floor(pixelY/TILE_SIZE));
		}

		/* Returns pixel position where tile would start */
		static XYPair<double> TileToPixel(int tileX, int tileY)
		{
			return XYPair<double>(tileX*TILE_SIZE, tileY*TILE_SIZE);
		}

		static Rect<double> TileToRect(int tileX, int tileY)
		{
			return Rect<double>(XYPair<double>(tileX*TILE_SIZE, tileY*TILE_SIZE),
				XYPair<double>(TILE_SIZE, TILE_SIZE));
		}

	private:
		Tile(){}
	};
};

#endif
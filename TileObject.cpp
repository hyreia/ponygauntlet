#include "TileObject.hpp"
#include "Tile.hpp"

using namespace gauntlet;

TileObject::TileObject(int tileX, int tileY):
currentAnimationIndex(0)
{
	centerPos = XYPair<double>(tileX*Tile::TILE_SIZE + Tile::TILE_SIZE/2, 
							   tileY*Tile::TILE_SIZE + Tile::TILE_SIZE/2);
}

TileObject::~TileObject(){}

std::shared_ptr<Bitmap> TileObject::GetSourceBitmap()
{
	return animations[currentAnimationIndex].type->bitmap;
}

XYPair<double> TileObject::ImageSource()
{
	return animations[currentAnimationIndex].ImageSource();
}

Rect<double> TileObject::GetImageBox()
{
	//We want the bottom of the image to rest at the bottom of the tile

	double bottomOfTile = (floor( centerPos.y / Tile::TILE_SIZE)+1) * Tile::TILE_SIZE;

	Rect<double> ret = animations[currentAnimationIndex].GetImageBox(centerPos.x, centerPos.y);
	ret.y = bottomOfTile - ret.h;
	return ret;
}

void TileObject::AddTimeToAnimation(double time)
{
	animations[currentAnimationIndex].AddTimeToAnimation(time);
}

Rect<double> TileObject::GetTileRect()
{
	auto tile = Tile::PixelToTile(centerPos.x, centerPos.y);
	return Tile::TileToRect(tile.x, tile.y);
}
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
	return animations[currentAnimationIndex].GetImageBox(centerPos.x, centerPos.y);
}

void TileObject::AddTimeToAnimation(double time)
{
	animations[currentAnimationIndex].AddTimeToAnimation(time);
}
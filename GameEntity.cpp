#include "GameEntity.hpp"
#include "Tile.hpp"

using namespace gauntlet;

GameEntity::GameEntity(Rect<double> _hitbox, std::shared_ptr<Bitmap> _bitmap,
Rect<double> _imageSource, int _imageDrawingFlags):
hitbox(_hitbox), imageBitmap(_bitmap), imageSource(_imageSource.x, _imageSource.y),
imageSize(_imageSource.w, _imageSource.h), imageDrawingFlags(_imageDrawingFlags)
{
	CalculateImageToHitboxOffset();
	CalculateNumOfTileIntersectionsToReserve();
}

GameEntity::~GameEntity(){}


void GameEntity::CalculateImageToHitboxOffset()
{
	//const double DIAMETER = hitbox.radius*2;
	imageToHitboxOffset = XYPair<double>( ( (hitbox.w)-imageSize.x)/2, (hitbox.h)-imageSize.y);
}

void GameEntity::CalculateNumOfTileIntersectionsToReserve()
{
	int tilesToReserve = 0;

	/* An entity smaller than or equal size of a tile sitting on the 
	 * intersection of four tiles takes 4 (+1 for each direction), 
	 if it's size is not easily divisble by tile size we need to round up to take into account the bit
	 hanging over into the next tile over (ceil)
	 */

	tilesToReserve = int( ceil(hitbox.w/double(Tile::TILE_SIZE)) *
						  ceil(hitbox.h/double(Tile::TILE_SIZE)));


	tilesIntersected.reserve(tilesToReserve);
}

Rect<double> GameEntity::GetHitbox(){ return hitbox; }
XYPair<double> GameEntity::GetPosition(){ return XYPair<double>(hitbox.x, hitbox.y); }
void GameEntity::SetPosition(double x, double y)
{
	hitbox.x = x; hitbox.y = y;
}
void GameEntity::AddToPosition(double x, double y)
{
	hitbox.x += x; hitbox.y += y;
}
		
Rect<double> GameEntity::GetImageBox()
{
	return Rect<double>(	(hitbox.x - hitbox.w) +imageToHitboxOffset.x,
					(hitbox.y - hitbox.h) + imageToHitboxOffset.y,
					imageSize.x, imageSize.y);
}

XYPair<double> GameEntity::ImageSource(){ return imageSource; }

int GameEntity::ImageDrawingFlags(){ return imageDrawingFlags; }

std::shared_ptr<Bitmap> GameEntity::GetSourceBitmap(){ return imageBitmap; }

void GameEntity::SetNewBitmap(std::shared_ptr<Bitmap> bitmap)
{

	CalculateImageToHitboxOffset();
}
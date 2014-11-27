#include "GameEntity.hpp"
#include "Tile.hpp"

using namespace gauntlet;

GameEntity::GameEntity(Circle<double> _hitbox, std::shared_ptr<Bitmap> _bitmap,
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
	imageToHitboxOffset = XYPair<double>( ( (hitbox.radius*2)-imageSize.x)/2, (hitbox.radius*2)-imageSize.y);
}

void GameEntity::CalculateNumOfTileIntersectionsToReserve()
{
	int tilesToReserve = 0;

	double diameter = hitbox.radius*2;


	/* An entity smaller than or equal size of a tile sitting on the 
	 * intersection of four tiles takes 4 (+1 for each direction), 
	 if it's size is not easily divisble by tile size we need to round up to take into account the bit
	 hanging over into the next tile over (ceil)
	 */
	tilesToReserve = int( ceil(diameter/double(Tile::TILE_SIZE)+1) *
						  ceil(diameter/double(Tile::TILE_SIZE)+1));


	tilesIntersected.reserve(tilesToReserve);
}

Circle<double> GameEntity::GetHitbox(){ return hitbox; }
XYPair<double> GameEntity::GetPosition(){ return XYPair<double>(hitbox.cx, hitbox.cy); }
void GameEntity::SetPosition(double x, double y)
{
	hitbox.cx = x; hitbox.cy = y;
}
void GameEntity::AddToPosition(double x, double y)
{
	hitbox.cx += x; hitbox.cy += y;
}
		
Rect<double> GameEntity::GetImageBox()
{
	return Rect<double>(	(hitbox.cx - hitbox.radius) +imageToHitboxOffset.x,
					(hitbox.cy - hitbox.radius) + imageToHitboxOffset.y,
					imageSize.x, imageSize.y);
}

XYPair<double> GameEntity::ImageSource(){ return imageSource; }

int GameEntity::ImageDrawingFlags(){ return imageDrawingFlags; }

std::shared_ptr<Bitmap> GameEntity::GetSourceBitmap(){ return imageBitmap; }
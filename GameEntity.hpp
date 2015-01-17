#ifndef GAME_ENTITY_HPP
#define GAME_ENTITY_HPP
#include <memory>
#include <vector>
#include "RectAndCircle.hpp"

/*
A GameEntity is defined above as an object with a position, 
what to draw and the image it’s being drawn from

If we want to modify how any subclass of GameEntity is drawn, 
we need to handle it in a way that still allows all objects to 
be correctly drawn through the GameEntity base class interface.
I.e. set GameEntity's image members to hide any further complexity
*/

namespace gauntlet
{
class Bitmap;
	
class GameEntity
{
	public:

		Rect<double> GetHitbox();
		XYPair<double> GetPosition();
		double GetRadius();
		void SetPosition(double x, double y);
		void AddToPosition(double x, double y);

		/* Where entity's center point is on the map, returns pos+size/2*/
		//XYPair<double> GetCenter();
		
		/* Analogous to 'hitbox', imageBox is where it's drawn and how
		 * big it is when it's drawn there. Adds hitbox and imageOffset in x and y,
		 w and h returns the size the drawing will be when done. 
		 In addition to these you need the source of the drawing (sx, sy) and
		 drawing flags for optional horizontal/vertical flipping, found below */
		Rect<double> GetImageBox();

		/* (sx, sy) */
		XYPair<double> ImageSource();
		/* Value to pass to allegro_bitmap for optional vert/horiz flipping */
		int ImageDrawingFlags();

		std::shared_ptr<Bitmap> GetSourceBitmap();
		

		/* Used to immediately see what tiles a GameEntity is intersecting,
		 * so that when we need to remove or move it we have that calculated already.
		 * Needs to be recalculated every time a moving GameEntity subclass is moved to keep it up to date
		 * with the value returned from passing GetTileRange GameEntity's hitbox. 
		 Notice it's public, an entity's position is public knowledge, this could be easily calculated
		 with its hitbox as well, this just saves us calculating it anytime we need it*/
		std::vector< XYPair<int>  > tilesIntersected;

		/* Public instead protected for debugging purposes*/
		GameEntity(Rect<double> hitbox, std::shared_ptr<Bitmap> bitmap,
			Rect<double> imageSource, int imageDrawingFlags = 0);
		/* Does nothing */
		virtual ~GameEntity();

	protected:
		/* Where entity actually is*/
		Rect<double> hitbox;

		/* Offset from hitbox where entity is drawn (add to hitbox to get draw pos)*/
		XYPair<double> imageToHitboxOffset;
		/* Size of sprite drawn for image*/
		XYPair<double> imageSize;

		/* Bitmap entity is drawn from*/
		std::shared_ptr<Bitmap> imageBitmap;

		/* Position on bitmap that the entity is drawn from*/
		XYPair<double> imageSource;

		/* Flags used to draw entity (1|2 = vertical/horizontal flip respectively) */
		int imageDrawingFlags;

		void SetNewBitmap(std::shared_ptr<Bitmap> bitmap);
	private:
		/* Called in constructor */
		void CalculateImageToHitboxOffset();
		/* Called in constructor */
		void CalculateNumOfTileIntersectionsToReserve();
};

};

#endif
#ifndef RECT_AND_CIRCLE_HPP
#define RECT_AND_CIRCLE_HPP

#include "XYPair.hpp"

template <typename T>
struct Rect
{
		T x, y, w, h;

		Rect(T _x = 0, T _y = 0, T _w = 0, T _h = 0):
		x(_x), y(_y), w(_w), h(_h){}

		Rect(XYPair<T> pos, XYPair<T> size):
		x(pos.x), y(pos.y), w(size.x), h(size.y)
		{}

		//Get lower edge
		inline T Y2(){ return y+h-1; }

		//Get right edge x
		inline T X2(){ return x+w-1; }

		//Get top left corner
		inline XYPair<T> TL(){ return XYPair<T>(x, y); }
		
		//Get top right corner
		inline XYPair<T> TR(){ return XYPair<T>(x+w-1, y); }

		//Get bottom left corner
		inline XYPair<T> BL(){ return XYPair<T>(x, y+h-1); }

		//Get bottom right corner
		inline XYPair<T> BR(){ return XYPair<T>(x+w-1, y+h-1); }

		inline XYPair<T> Center(){ return XYPair<T>(x+(w/2), y+(h/2)); }

		void SetCenterTo(T centerX, T centerY)
		{
			x = centerX - w/2;
			y = centerY - h/2;
		}

		T Bottom(){ return y+h; }
		T Top(){ return y; }
		T Left(){ return x; }
		T Right(){ return x+w; }

		bool IsOverlapping(Rect other)
		{
			if ((x > other.x + other.w - 1) || // is b1 on the right side of b2?
				(y > other.y + other.h - 1) || // is b1 under b2?
				(other.x > x + w - 1) || // is b2 on the right side of b1?
				(other.y > y + h - 1))   // is b2 under b1?
			{
				// no collision
				return false;
			}
 
			// collision
			return true;
		}

		bool IsCompletelyWithin(Rect<T> other)
		{
			if(x >= other.x && x+w <= other.x+other.w &&
				y >= other.y && y+h <= other.y+other.h)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool operator<(const Rect<T> &other) const
		{
			if(y < other.y) return true;
			if(y == other.y) return x < other.x;

			return false;
		}

		void operator=(const Rect<T> &other)
		{
			x = other.x;
			y = other.y;
			w = other.w;
			h = other.h;
		}

		bool IsVerticalLineIntersecting(T lineX, T lineY1, T lineY2)
		{
			if(lineX < x || lineX > x+w) return false;

			//if the line is shorter than the rectangle
			if(lineY1 > y && lineY1 < y+w) return true;
			if(lineY2 > y && lineY2 < y+w) return true;

			//if the line is longer than the rectangle
			if(y > lineY1 && y < lineY2) return true;
			if(y+w > lineY1 && y+w < lineY2) return true;
		
			return false;
		}
		bool IsHorizontalLineIntersecting(T lineX1, T lineX2, T lineY)
		{
			if(lineY < y || lineY > y+h) return false;

			//if the line is shorter than the rectangle
			if(lineX1 > x && lineX1 < x+h) return true;
			if(lineX2 > x && lineX2 < x+h) return true;

			//if the line is longer than the rectangle
			if(x > lineX1 && x < lineX2) return true;
			if(x+h > lineX1 && x+h < lineX2) return true;
		

			return false;
		}

		bool GetCollisionResolution(Rect<T> &b, XYPair<T> &normal)
		{
			/* Since the vector is a pointer then it could have been
			intialized to any value so we need to make sure we initalize
			the vectors X and Y to zero*/
			normal.x = 0;
			normal.y = 0;

			// The distance between the two objects
			XYPair<> Distance;
			// The absDistance between the objects
			XYPair<> absDistance;

			double XMagnitute;
			double YMagnitute;

			// Calculate the distance between A and B
			Distance.x = ( ( b.x ) - ( this->x ) );
			Distance.y = ( ( b.y ) - ( this->y /*- (this->h / 2)*/ ) );
			//Y has to account that this original algorithm assumes Y was in the center

			// Combine both rectangles and half the returned value
			double XAdd = ( ( b.w ) + ( this->w ) ) / 2.0f;
			double YAdd = ( ( b.h ) + ( this->h ) ) / 2.0f;
	


			// Check if the Distance vector is below 0.0f
			absDistance.x = ( Distance.x < 0.0f ) ? -Distance.x : Distance.x;
			absDistance.y = ( Distance.y < 0.0f ) ? -Distance.y : Distance.y;

			/*If the absDistance X is less than X add and the absDistance is less than YAdd
			then it dosen't take a genius to figure out they aren't colliding so return false*/
			if( ! ( ( absDistance.x < XAdd ) && ( absDistance.y < YAdd ) ) ) {   
			return false;
			}

			/*Get the magnitute by the overlap of the two rectangles*/
			XMagnitute = XAdd - absDistance.x;
			YMagnitute = YAdd - absDistance.y;

			/*Determin what axis we need to act on based on the overlap*/
			if( XMagnitute < YMagnitute ) {
				normal.x = ( Distance.x >= 0) ? -XMagnitute : XMagnitute;
			}
			else if ( XMagnitute > YMagnitute ) {
				normal.y = ( Distance.y >= 0) ? -YMagnitute : YMagnitute;
			}
			// If we reached this point then we now know the was a collision
			return true;	
		}
};

/* Returns true if rect a and b are intersecting. Sets normal to 
the values needed to add to a's position to move it out of b. Subtract
it from b to move b out of the way. Or mix and match.*
inline bool GetCollisionResolution(const Rect<double> &a, const Rect<double> &b, XYPair<double> &normal)
{
	///* Since the vector is a pointer then it could have been
   //intialized to any value so we need to make sure we initalize
   //the vectors X and Y to zero
   normal.x = 0;
   normal.y = 0;

   // The distance between the two objects
   XYPair<double> Distance;
   // The absDistance between the objects
   XYPair<double> absDistance;

   double XMagnitute;
   double YMagnitute;

   // Calculate the distance between A and B
   Distance.x = ( ( b.x ) - ( a.x ) );
   Distance.y = ( ( b.y ) - ( a.y /*- (a.h / 2) ) );
   //Y has to account that this original algorithm assumes Y was in the center

   // Combine both rectangles and half the returned value
   double XAdd = ( ( b.w ) + ( a.w ) ) / 2.0f;
   double YAdd = ( ( b.h ) + ( a.h ) ) / 2.0f;
	


   // Check if the Distance vector is below 0.0f
   absDistance.x = ( Distance.x < 0.0f ) ? -Distance.x : Distance.x;
   absDistance.y = ( Distance.y < 0.0f ) ? -Distance.y : Distance.y;

    /*If the absDistance X is less than X add and the absDistance is less than YAdd
    then it dosen't take a genius to figure out they aren't colliding so return false
   //if( ! ( ( absDistance.x < XAdd ) && ( absDistance.y < YAdd ) ) ) {   
   return false;
   }

   /*Get the magnitute by the overlap of the two rectangles
    XMagnitute = XAdd - absDistance.x;
    YMagnitute = YAdd - absDistance.y;

   /*Determin what axis we need to act on based on the overlap
    if( XMagnitute < YMagnitute ) {
      normal.x = ( Distance.x >= 0) ? -XMagnitute : XMagnitute;
    }
    else if ( XMagnitute > YMagnitute ) {
      normal.y = ( Distance.y >= 0) ? -YMagnitute : YMagnitute;
   }
   // If we reached this point then we now know the was a collision
   return true;	
}
*/

template <typename T>
struct Circle
{
	Circle(T x, T y, T r): cx(x), cy(y), radius(r)
	{}
	T cx, cy, radius;
	Rect<T> Squared()
	{
		return Rect<T>(cx-radius, cy-radius, 2*radius + 1, 2*radius + 1);
	}	

};

inline bool IsRectIntersectingCircle(double cx, double cy, double radius, Rect<double> rect)
{
		XYPair<double> point(cx, cy);

		//if center of circle is within rect return true
		if(point.x > rect.Left() && point.x < rect.Right() &&
			point.y > rect.Top() && point.y < rect.Bottom())
		{
			return true;
		}
		else
		{
			//Snap point to rect to find closest point on rect
			if(point.x > rect.Right()) point.x = rect.Right();
			if(point.x < rect.Left()) point.x = rect.Left();
			if(point.y > rect.Bottom()) point.y = rect.Bottom();
			if(point.y < rect.Top()) point.y = rect.Top();

			//If snapped point is still within circle, rect is within circle
			double distanceSquared =	(point.x - cx)*(point.x - cx) + 
										(point.y - cy)*(point.y - cy);
			
			//return radius*radius > distanceSquared; //can return true even if difference appears to be 0 because floating points
			return ( radius*radius - distanceSquared > 0.00001); 
		}
}

inline bool IsPointInsideCircle(double cx, double cy, double r, double px, double py)
{
		double distanceSquared =	(px - cx)*(px - cx) + 
									(py - cy)*(py - cy);
		return r*r > distanceSquared;
}

inline bool IsCircleIntersectingCircle(double cx1, double cy1, double r1, double cx2, double cy2, double r2)
{
	const double EPSILON = 0.00001;

	double distanceSquared =	(cx2 - cx1)*(cx2 - cx1) + 
								(cy2 - cy1)*(cy2 - cy1);
	
	return ( (r1+r2)*(r1+r2) - distanceSquared > EPSILON);
}

#endif
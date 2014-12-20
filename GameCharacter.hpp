#ifndef GAME_CHARACTER_HPP
#define GAME_CHARACTER_HPP


#include "GameEntity.hpp"
#include "Directions.hpp"

namespace gauntlet
{

enum GameCharacterAnimations
{
	IDLE_ANIMATION, //A game character's default animation
	MOVING_ANIMATION
};

/* The main difference between an entity and a game character is that
a game character is expected to be moving around: so it has velocity,
a facing angle, and a previous position where it was before it was moved last. */

class GameCharacter: public GameEntity
{
public:
	GameCharacter(XYPair<double> position, XYPair<double> size,
		std::shared_ptr<Bitmap> bitmap,
		Rect<double> imageSource,
		int startFacing,
		double height = 50,
		double altitude = 0);
	
	/* Does nothing */
	virtual ~GameCharacter();

	/* Returns imageBox describing where to draw character at the moment for smooth 
	movement. */
	Rect<double> GetImageBoxAtDeltaTime(double alpha);

	double GetRenderSortY();

	void SetVelocity(double x, double y);
	XYPair<double> GetVelocity();
	void SetYVelocity(double y);
	void SetXVelocity(double x);
	void AddVelocityToPosition();

	/* Used to determine where to calculate imageBox for */
	void SetLastPosition(XYPair<double> previousPosition);
	void SetLastPositionToCurrentPosition();

	/* Used to project where player is trying to move to*/
	Rect<double> GetNextHitbox();
	Rect<double> GetHitboxAtDeltaTime(double alpha);
	XYPair<double> ProjectedNextPosition();

	void SetFacingAngle(double newFacing);
	double GetFacing();
	void SetFacingFromRadian(double radian);
	void SetFacingDirection(int direction);

	void SetAnimation(int newAnimation);
	void UpdateAnimation();
	void AddStepsToAnimation(double stepsAdded);

	double GetHeight();

	double GetAltitude();
	void SetAltitude(double newAltitude);

	void AddLockToFacingAngle();
	void RemoveLockFromFacingAngle();
	bool IsFacingAngleLocked();

	void AddLockToMovement();
	void RemoveLockFromMovement();
	bool isMovementLocked();

	void SetWasTileBlocked(bool isBlockedByTile);
	bool GetWasTileBlocked();

	static double drawAlpha;
protected:
		
	/* How far it is from the ground, applicable for projectiles, characters and tile collisions */
	double altitude;

	/* How tall (and thus how far their vertical reach) is in pixels, default=50 */
	double height;

	/* Direction character is facing in radians*/
	double facingAngle;

	/* Locks on facing, if there are more than 0, the game character shouldn't be changing their facing angle, useful for attacks that lock the facing (long, ranged attacks) */
	int numOfFacingAngleLocks;

	/* Locks on movement. If there are more than 0, the game character shouldn't be moving (velocity is locked to zero0, useful for attacks that require holding still */
	int numOfMovementLocks;

	/* Velocity attempted to be added to position when it moves*/
	XYPair<double> velocity;

	/* Used to interpolate drawing position when drawing between logic frames*/
	XYPair<double> lastPosition;

	/* current animation */
	int animation;
	/* time that has passed while in animation */
	int animationTime;
	/* distance moved while in animation, affects movement animation speed */
	double stepsInAnimation;

	/* The closest to eight-directions the character's angle is facing */
	int animationFacing;

	/* If true, it was blocked from moving fully by a tile last frame,
	 *  used for making movement between two tiles with one tile space easier
	 * for player characters */
	bool wasTileBlocked;
};

/* Ugly work around */
 bool CompareCharactersByYAxis(GameCharacter *a, GameCharacter *b);

};

#endif
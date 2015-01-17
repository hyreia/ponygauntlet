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

/* The main difference between an moving entity and a game character is that
a game character has a predictable set of animations that are all the same, just scaled.
While a moving game entity has no pre-determined one. */

class GameCharacter: public GameEntity
{
public:
	GameCharacter(unsigned int ID, XYPair<double> position, XYPair<double> size,
		std::shared_ptr<Bitmap> bitmap,
		Rect<double> imageSource,
		int startFacing,
		double height = 50,
		double altitude = 0);
	
	unsigned int GetID();

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

	void SetForcedVelocity(double x, double y);
	XYPair<double> GetForcedVelocity();
	void AddForcedVelocityToVelocity();

	/* Used to determine where to calculate imageBox for */
	void SetLastPosition(XYPair<double> previousPosition);
	void SetLastPositionToCurrentPosition();

	/* Used to project where player is trying to move to*/
	Rect<double> GetNextHitbox();
	Rect<double> GetHitboxAtDeltaTime(double alpha);
	XYPair<double> ProjectedNextPosition();

	void SetFacingAngle(double newFacing);
	double GetFacing();


	void SetAnimation(int newAnimation);
	void UpdateAnimation();
	void AddStepsToAnimation(double stepsAdded);

	double GetHeight();

	double GetAltitude();
	void SetAltitude(double newAltitude);


	void LockMovementFromAnAttack();
	void UnlockMovementFromAnAttack();
	bool IsMovementLockedFromAnAttack();

	void LockRotationFromAnAttack();
	void UnlockRotationFromAnAttack();
	bool IsRotationLockedFromAnAttack();


	bool IsStunTimeLeft();
	/* Overrides any current value */
	void SetStunTime(double timeToStun);
	/* Does not let stun time left go below zero*/
	void DecreaseStunTime(double timeToSubtract);

	static double drawAlpha;
protected:
	const unsigned int ID;

	void SetFacingFromRadian(double radian);
	void SetFacingDirection(int direction);

	/* How far it is from the ground, applicable for projectiles, characters and tile collisions */
	double altitude;

	/* How tall (and thus how far their vertical reach) is in pixels, default=50 */
	double height;

	/* Direction character is facing in radians*/
	double facingAngle;

	/* Velocity attempted to be added to position when it moves*/
	XYPair<double> velocity;
	XYPair<double> forcedVelocity;

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

	double timeLeftStunned;

	/* Unless 0, shouldn't be moving */
	int movementLocksFromAnAttack;

	/* Unless 0, shouldn't be rotating*/
	int rotationLocksFromAnAttack;
};

/* Ugly work around */
 bool CompareCharactersByYAxis(GameCharacter *a, GameCharacter *b);

};

#endif
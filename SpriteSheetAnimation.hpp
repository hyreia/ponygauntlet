#ifndef SPRITE_SHEET_ANIMATION_HPP
#define SPRITE_SHEET_ANIMATION_HPP

#include <memory>
#include <string>
#include "RectAndCircle.hpp"
#include "Bitmap.hpp"

enum SPRITE_SHEET_ANIMATIONS
{
	NO_ANIMATION,
	ANIM_HAMMER,
	ANIM_GROUND_POUND,
	ANIM_CRACKLE,
	ANIM_FLASH,
	ANIM_HAMMER_SMASH,
	ANIM_BALL_LIGHTNING,
	ANIM_LASER,
	ANIM_HALBERD,
	ANIM_SLASH,
	ANIM_ARROW,
	ANIM_GLOWING_ARROW,
	ANIM_SMALL_FLASH,
	ANIM_SWING_ARC,
	ANIM_WALLOP,
	ANIM_GRAVE,
	NUM_OF_ANIMATIONS
};

namespace gauntlet
{
class Bitmap;

/* A struct storing information about a non-character animation.

Animation frames are all square; their width is determined by their height, the number of frames is determined by width/height. 
Animations either loop, or end after the last frame (loops or doesn't loop)

Animations are typically rotated to match the angle the attack is facing. They are typically centered on whatever their target is. Some attacks have an animation that loops to represent their presence, others have a single animation that plays when it's started or when it hits something. 
Attacks that are a non-moving line have their animation 'stretched' to the calculated target. 
*/
struct SpriteSheetAnimationType
{
	SpriteSheetAnimationType(std::shared_ptr<Bitmap> _bitmap,
		bool _willLoop,
		double _secondsPerFrame,
		double _dimension,
		int _num_of_frames,
		bool _isDirectional = false,
		bool _isOnFloorLayer = false):
	bitmap(_bitmap),
	WILL_LOOP(_willLoop),
	SECONDS_PER_FRAME(_secondsPerFrame),
	HALF_DIMENSION(_dimension/2),
	NUM_OF_FRAMES(_num_of_frames),
	IS_DIRECTIONAL(_isDirectional),
	IS_ON_FLOOR_LAYER(_isOnFloorLayer)
	{}

	std::shared_ptr<Bitmap> bitmap;
	bool WILL_LOOP;
	double SECONDS_PER_FRAME;
	double HALF_DIMENSION; //width == height
	int NUM_OF_FRAMES;

	/* True if animation is dependent on target's direction (is rotated),
	value is ignored and always true for the animation representing an attack */
	bool IS_DIRECTIONAL;

	/* True if drawn on layer below game characters, ignored when representing an attack */
	bool IS_ON_FLOOR_LAYER;

	SpriteSheetAnimationType():
	WILL_LOOP(false),
	SECONDS_PER_FRAME(77777.0),
	HALF_DIMENSION(777),
	bitmap(NULL),
	NUM_OF_FRAMES(777){}
};

struct SpriteSheetAnimation
{
	SpriteSheetAnimationType *type;
	int currentFrame;
	double timeInCurrentFrame;
	bool isDone;
	double rotation;

	SpriteSheetAnimation(SpriteSheetAnimationType *_type, double _rotation = 0.0):
	currentFrame(0),
	timeInCurrentFrame(0),
	type(_type),
	isDone(false),
	rotation(0.0)
	{
		if(type->IS_DIRECTIONAL) rotation = _rotation;
	}

	Rect<double> GetImageBox(double centerX, double centerY)
	{
		auto half = type->HALF_DIMENSION;

		Rect<double> ret(centerX - half,
			centerY - half,
			2*half, 2*half);

		return ret;
	}

	void AddTimeToAnimation(double delta)
	{
		timeInCurrentFrame += delta;
		if(timeInCurrentFrame >= type->SECONDS_PER_FRAME)
		{
			timeInCurrentFrame -= type->SECONDS_PER_FRAME;
			
			//Note: frame counting starts at 0, if there are 3 frames, frame 2 is the last one
			//"Is there a frame after this one?"
			if(currentFrame < type->NUM_OF_FRAMES-1)
			{
				currentFrame++;
				timeInCurrentFrame = 0;
			}
			//we're at the last frame, does the animation loop?
			else if(type->WILL_LOOP) 
			{
				currentFrame = 0;
			}
			// else it doesn't loop
			else
			{
				isDone = true;
			}
		}
	}

	XYPair<double> ImageSource()
	{
		XYPair<double> ret;
		auto width = type->HALF_DIMENSION*2;
		ret.x = currentFrame*width;
		ret.y = 0;

		return ret;
	}

};

struct AnimationOnGameCharacter
{
	AnimationOnGameCharacter(SpriteSheetAnimationType *type, double rotation,
		int indexOfCharacter,
		XYPair<double> animationOffset):
	animation(type, rotation),
	characterIndexAnimationIsCenteredOn(indexOfCharacter),
	offset(animationOffset)
	{}
	SpriteSheetAnimation animation;
	int characterIndexAnimationIsCenteredOn;
	XYPair<double> offset;

};

};
#endif
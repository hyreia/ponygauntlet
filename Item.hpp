#ifndef ITEM_HPP
#define ITEM_HPP

#include "GameEntity.hpp"

namespace gauntlet
{
	enum ITEM_TYPES
	{
		KEY_ITEM_TYPE,
		BOMB_ITEM_TYPE,
		MONEY_ITEM_TYPE,
		HEAL_HEALTH_ITEM_TYPE,

		TEMPORARILY_BOOST_STR_STAT_ITEM_TYPE,
		PERMANENT_INCREASE_STR_STAT_ITEM_TYPE,
		TEMPORARILY_BOOST_SPD_STAT_ITEM_TYPE,
		PERMANENT_INCREASE_SPD_STAT_ITEM_TYPE,
		TEMPORARILY_BOOST_ARM_STAT_ITEM_TYPE,
		PERMANENT_INCREASE_ARM_STAT_ITEM_TYPE,
		TEMPORARILY_BOOST_MAG_STAT_ITEM_TYPE,
		PERMANENT_INCREASE_MAG_STAT_ITEM_TYPE,
		
		INVISIBILITY_EFFECT_TYPE,
		RAPID_FIRE_EFFECT_TYPE,
		XRAY_GLASSES_EFFECT_TYPE,
		MULTIWAY_SHOT_EFFECT_TYPE,
		REPULSION_EFFECT_TYPE
	};

	enum 
	{

	};

	struct ItemType
	{
		int type;

		/*	KEY_ITEM_TYPE = number of keys,
			BOMB_ITEM_TYPE = number of bombs,
			MONEY_ITEM_TYPE = amount of money,
			HEAL_HEALTH_ITEM_TYPE = amount healed,

			TEMPORARILY_BOOST_STR_STAT_ITEM_TYPE = amount,
			PERMANENT_INCREASE_STR_STAT_ITEM_TYPE = amount,
			TEMPORARILY_BOOST_SPD_STAT_ITEM_TYPE = amount,
			PERMANENT_INCREASE_SPD_STAT_ITEM_TYPE = amount,
			TEMPORARILY_BOOST_ARM_STAT_ITEM_TYPE = amount,
			PERMANENT_INCREASE_ARM_STAT_ITEM_TYPE = amount,
			TEMPORARILY_BOOST_MAG_STAT_ITEM_TYPE = amount,
			PERMANENT_INCREASE_MAG_STAT_ITEM_TYPE = amount,
		
			INVISIBILITY_EFFECT_TYPE = time,
			RAPID_FIRE_EFFECT_TYPE = time,
			XRAY_GLASSES_EFFECT_TYPE = time,
			MULTIWAY_SHOT_EFFECT_TYPE = time,
			REPULSION_EFFECT_TYPE = time */
		double typeEffect;
		//points added to score from item use
		int pointValue;
		//price to buy
		int buyCost;
		bool usedImmediately;

		std::shared_ptr<Bitmap> bitmap;
	};

	class Item: public GameEntity
	{
	public:
		Item(XYPair<double> center, ItemType *_type):
		type(_type),
		GameEntity(Circle<double>(center.x, center.y, 8), _type->bitmap, 
		Rect<double>(0,0,16,16))
		{}
		~Item(){}
	private:
		ItemType *type;
	};
};

#endif
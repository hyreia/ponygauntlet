#ifndef ACTION_LIST_HPP
#define ACTION_LIST_HPP

/* hardcoded instead of loaded from file for now(?)*/
enum ATTACK_INDEXES
{
	HAMMER_SWING,
	GROUND_POUND,
	HAMMER_SMASH,
	BALL_LIGHTNING,
	LASER_BLAST,
	GUIDED_LIGHTNING,
	HALBERD_SWING,
	HALBERD_PIERCE,
	FIRED_ARROW,
	GUIDED_ARROW,
	SKULL_CHOMP,
	NUM_OF_ATTACKS
};

enum NONATTACK_ACTIONS
{
	FLIGHT,
	KEY_ON_CHEST,
	KEY_ON_GATE,
	REVIVE_ALLY,
	FLIP_SWITCH,
	NUM_OF_NONATTACK_ACTIONS
};

enum ACTION_TYPE
{
	ATTACK_ACTION_TYPE,
	TRIPLE_SHOT_ACTION,
	ITEM_ACTION_TYPE,
	OTHER_ACTION_TYPE
};

struct Action
{
	Action(){}
	Action(int _actionType, int _typeIndex, int _iconIndex=-1):
	actionType(_actionType),
	typeIndex(_typeIndex),
	iconIndex(_iconIndex)
	{}
	/* Attack, Item, Non-Attack Other action*/
	unsigned int actionType;

	/* Index in list for specified action type for more information on this action,
	 * or just more information if only a single integer is needed*/
	unsigned int typeIndex;

	/* action image index from action icons, to be displayed in ActionBar, -1 is for no icon*/
	int iconIndex;
};

#endif
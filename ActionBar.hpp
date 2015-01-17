#ifndef ACTION_BAR_HPP
#define ACTION_BAR_HPP

namespace gauntlet
{

	struct ActionBarNode
	{
		Action action;

	};

	struct ActionBar
	{
		ActionBar():
		selectedNode(0)
		{}
		std::vector<ActionBarNode> nodes;
		int selectedNode;
	};

};

#endif
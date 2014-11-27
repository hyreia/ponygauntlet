#ifndef TREASURE_CHEST_HPP
#define TREASURE_CHEST_HPP

#include "TileObject.hpp"

namespace gauntlet
{
	/* TODO: this is a stub*/
	class TreasureChest: public TileObject
	{
	public:
		TreasureChest(std::string _contents, int tileX, int tileY):
		TileObject(tileX, tileY),
		contents(_contents)
		{}
		void SetIsContentsVisible(bool isVisible)
		{
			isContentsVisible = isVisible;
		}
		bool IsContentsVisible()
		{
			return isContentsVisible;
		}

		std::string GetNameOfContents()
		{
			return contents;
		}
	private:
		std::string contents;
		bool isContentsVisible;
	};
};

#endif
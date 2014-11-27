#ifndef LEVEL_MAP_HPP
#define LEVEL_MAP_HPP

#include "RectAndCircle.hpp"
#include <vector>
#include <string>
#include "XYPair.hpp"

typedef struct _ALLEGRO_MAP ALLEGRO_MAP;

namespace gauntlet
{
/* Initialize() loads an allegro map from the given tmx file,
 * and stores a list of starting locations
 *
 *
*/
class LevelMap
{

	public:
		LevelMap();
		~LevelMap();
		bool Initialize(const char *filename);
		void Destroy();
		const char *GetErrorMessage();
		std::string GetNameOfBackgroundMusic();
		std::vector< XYPair<double> > GetPlayerStartPositions();

		/* Order of Drawing When Drawing a Dungeon Map:
			Draw the void below the floor
			Draw the floor
			Draw walls
			Draw all the dungeon entities on screen
			Draw the ceiling
			Draw the HUD
		*/
		void DrawVoidLayerRegion(float sx, float sy, float sw, float sh, float dx, float dy, int flags);
		void DrawGroundLayerRegion(float sx, float sy, float sw, float sh, float dx, float dy, int flags);
		void DrawWallLayerRegion(float sx, float sy, float sw, float sh, float dx, float dy, int flags);
		void DrawCeilingLayerRegion(float sx, float sy, float sw, float sh, float dx, float dy, int flags);

		bool IsThereAFloorAt(int tileX, int tileY);
		void RemoveFloorAt(int tileX, int tileY);

		bool IsThereAWallAt(int tileX, int tileY);
		void RemoveWallAt(int tileX, int tileY);
		int GetHeightOfWall(int tileX, int tileY);

		/* returns true if there is a floor and no wall*/
		bool CanBeWalkedOn(int tileX, int tileY);

		int GetTileWidthInTiles();
		int GetTileHeightInTiles();
		double GetMapWidthInPixels();
		double GetMapHeightInPixels();


	private:
		ALLEGRO_MAP *allegroMap;
		const char *error;

		//precalculated because it's used frequently
		XYPair<double> levelSizeInPixels;
		XYPair<int> levelSizeInTiles;

		void CollectAllStartPositions();
		std::vector< XYPair< double> > playerStartPositions;

		static char *VOID_LAYER, *GROUND_LAYER, *WALL_LAYER, *CEILING_LAYER;
		static const char *mapDirectory;
		inline void DrawLayerRegion(char *name, float sx, float sy, float sw, float sh, float dx, float dy, int flags);
};

};

#endif
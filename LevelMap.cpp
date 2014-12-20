#include "LevelMap.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_tiled.h>
#include "GauntletApp.hpp"

using namespace gauntlet;

char *LevelMap::VOID_LAYER = "Void";
char *LevelMap::GROUND_LAYER = "Ground";
char *LevelMap::WALL_LAYER = "Wall";
char *LevelMap::CEILING_LAYER = "Ceiling";

const char *LevelMap::mapDirectory = "../data/maps";

LevelMap::LevelMap():
allegroMap(NULL),
error(NULL)
{}

LevelMap::~LevelMap()
{
	if(allegroMap) al_free_map(allegroMap);
}

bool LevelMap::Initialize(const char *filename)
{
	if(allegroMap) Destroy();

	allegroMap = al_open_map(mapDirectory, filename);
	if(allegroMap)
	{
		levelSizeInTiles.x = al_get_map_width(allegroMap);
		levelSizeInTiles.y = al_get_map_height(allegroMap);
		
		levelSizeInPixels.x = al_get_tile_width(allegroMap) * levelSizeInTiles.x;
		levelSizeInPixels.y = al_get_tile_height(allegroMap)* levelSizeInTiles.y;
		
		al_convert_tileset_mask_to_alpha(allegroMap, al_map_rgb(255,0,255));

		CollectAllStartPositions();
		if(playerStartPositions.empty())
		{
			app->allegro.ShowError("No starting positions on map for players!");
			return false;
		}

		return true;

	}
	else
	{
		app->allegro.ShowError("Couldn't open the map file");
		return false;
	}
}

void LevelMap::CollectAllStartPositions()
{
	if(!playerStartPositions.empty()) playerStartPositions.clear();

	ALLEGRO_MAP_LAYER *objectLayer = al_get_map_layer(allegroMap, "Objects");
	if(objectLayer) 
	{
		int numOfObjects = 0;
		int nextStartPos = 0;
		auto objectList = al_get_objects(objectLayer, &numOfObjects);
		for(int o = 0; o < numOfObjects; o++)
		{
			if( strcmp( al_get_object_type(objectList[o]), "Start") == 0)
			{
				double xPos = al_get_object_x(objectList[o]);
				double yPos = al_get_object_y(objectList[o]);
				playerStartPositions.push_back( XYPair<double>(xPos, yPos));
				
			}
			//else do nothing

			al_free(objectList[o]);
			
		}
	}
	//else do nothing


	return;
}

void LevelMap::Destroy()
{
	if(allegroMap) al_free_map(allegroMap);
	allegroMap = NULL;
}

const char *LevelMap::GetErrorMessage()
{
	return error;
}

std::string LevelMap::GetNameOfBackgroundMusic()
{
	std::string ret;

	auto musicFile = al_get_map_property(allegroMap, "music", NULL);
	if(musicFile)
	{
		ret = musicFile;
	}
	//else do nothing

	return ret;
}

std::vector< XYPair<double> > LevelMap::GetPlayerStartPositions()
{
	return playerStartPositions;
}

void LevelMap::DrawVoidLayerRegion(float sx, float sy, float sw, float sh, float dx, float dy, int flags)
{
	if(al_get_map_layer(allegroMap, VOID_LAYER))
	{
		DrawLayerRegion(VOID_LAYER, sx, sy, sw, sh, dx, dy, flags);
	}
}
void LevelMap::DrawGroundLayerRegion(float sx, float sy, float sw, float sh, float dx, float dy, int flags)
{
	if(al_get_map_layer(allegroMap, GROUND_LAYER))
	{
		DrawLayerRegion(GROUND_LAYER, sx, sy, sw, sh, dx, dy, flags);
	}
}
void LevelMap::DrawWallLayerRegion(float sx, float sy, float sw, float sh, float dx, float dy, int flags)
{
	if(al_get_map_layer(allegroMap, WALL_LAYER))
	{
		DrawLayerRegion(WALL_LAYER, sx, sy, sw, sh, dx, dy, flags);
	}
}
void LevelMap::DrawCeilingLayerRegion(float sx, float sy, float sw, float sh, float dx, float dy, int flags)
{
	if(al_get_map_layer(allegroMap, CEILING_LAYER))
	{
		DrawLayerRegion(CEILING_LAYER, sx, sy, sw, sh, dx, dy, flags);
	}
}

void LevelMap::DrawLayerRegion(char *name, float sx, float sy, float sw, float sh, float dx, float dy, int flags)
{
	al_draw_layer_region_for_name(allegroMap, name, sx, sy, sw, sh, dx, dy, flags);
}

bool LevelMap::IsThereAFloorAt(int tileX, int tileY)
{
	ALLEGRO_MAP_LAYER *Floor = al_get_map_layer(allegroMap, GROUND_LAYER);
	if(Floor) return (al_get_single_tile_id(Floor, tileX, tileY) != 0);
	else return false;
}

bool LevelMap::IsThereAWallAt(int tileX, int tileY)
{
	ALLEGRO_MAP_LAYER *Wall = al_get_map_layer(allegroMap, WALL_LAYER);
	if(Wall)
	{
		int tileid = al_get_single_tile_id(Wall, tileX, tileY);
		return (tileid != 0);
	}
	else return false;
}

int LevelMap::GetHeightOfWall(int tileX, int tileY)
{
	if(!IsThereAWallAt(tileX, tileY)) return 0;
	else
	{
		ALLEGRO_MAP_LAYER *Wall = al_get_map_layer(allegroMap, WALL_LAYER);
		auto tile = al_get_single_tile(allegroMap, Wall, tileX, tileY);
		
		return atoi(al_get_tile_property(tile, "height", "50"));
	}
}

void LevelMap::RemoveWallAt(int tileX, int tileY)
{
	ALLEGRO_MAP_LAYER *layer = al_get_map_layer(allegroMap, WALL_LAYER);
	if(layer){	al_remove_tile(layer, tileX, tileY); }
}

void LevelMap::RemoveFloorAt(int tileX, int tileY)
{
	ALLEGRO_MAP_LAYER *layer = al_get_map_layer(allegroMap, GROUND_LAYER);
	if(layer){	al_remove_tile(layer, tileX, tileY); }
}

bool LevelMap::CanBeWalkedOn(int tileX, int tileY)
{
	ALLEGRO_MAP_LAYER *Floor = al_get_map_layer(allegroMap, GROUND_LAYER);
	if(Floor)
	{
		if(al_get_single_tile_id(Floor, tileX, tileY) != 0)
		{
			ALLEGRO_MAP_LAYER *Wall = al_get_map_layer(allegroMap, WALL_LAYER);
			if(!Wall) return true; //no walls, can be walked on
			if(al_get_single_tile_id(Wall, tileX, tileY) != 0)
			{
				return false;
			}
			else return true;
		}
		else return false; //there's no floor here

	}	
	else return false; //no floor

}

double LevelMap::GetMapWidthInPixels(){ return levelSizeInPixels.x; }
double LevelMap::GetMapHeightInPixels(){ return levelSizeInPixels.y; }

int LevelMap::GetTileWidthInTiles(){ return levelSizeInTiles.x; }
int LevelMap::GetTileHeightInTiles(){ return levelSizeInTiles.y; }
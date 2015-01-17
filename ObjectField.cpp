#include "ObjectField.hpp"
#include "Tile.hpp"
#include "Monster.hpp"
#include "PlayerCharacter.hpp"
#include "Spawner.hpp"

using namespace gauntlet;

ObjectField::ObjectField():
nextCharacterIdAvailable(0)
{}
ObjectField::~ObjectField(){}

void ObjectField::InitializeToSize(int width, int height)
{
	std::vector<ObjectFieldCell> objectColumn;
	while(height > (int)objectColumn.size())
	{
		ObjectFieldCell newCell;
		objectColumn.push_back(newCell);
	}
	while(width > (int)objectField.size())
	{
		objectField.push_back(objectColumn);
	}

	//Collision grid
	std::vector<CollisionGridCell> collisionColumn;
	while(height > (int)collisionColumn.size())
	{
		CollisionGridCell newCell;
		collisionColumn.push_back(newCell);
	}
	while(width > (int)collisionGrid.size())
	{
		collisionGrid.push_back(collisionColumn);
	}

}

bool ObjectField::TileHasObjectThatBlocksCharacterFromMoving(GameCharacter *character, int tileX, int tileY)
{
	double characterAltitude = character->GetAltitude();
	auto cell = objectField[tileX][tileY];
	double CHEST_LEVER_GATE_HEIGHT = 30.0;
	double SPAWNER_HEIGHT = 50.0;

	if(characterAltitude < CHEST_LEVER_GATE_HEIGHT && (cell.chest || cell.lever || cell.gate)) return true;
	else if(characterAltitude < SPAWNER_HEIGHT && (cell.spawner && cell.spawner->WillBlockMovement())) return true;
	else return false;
}

void ObjectField::Clear(){}

std::vector< XYPair<int> > ObjectField::GetTilesInRange(Rect<double> range)
{
	std::vector< XYPair<int> > tilesInRange;

	//Keeping it on objectField
	if(range.x < 0)
	{
		range.w += range.x; //shorten width
		range.x = 0; 
	}
	if(range.y < 0)
	{
		range.h += range.y; //shorten height
		range.y = 0;
	}
	if( (range.y+range.h)/Tile::TILE_SIZE > (int)objectField[0].size())
	{
		range.h = objectField[0].size()*Tile::TILE_SIZE - range.y;
	}
	//if range extends beyond the width of the tiles, shorten the range
	if( (range.x+range.w)/Tile::TILE_SIZE > (int)objectField.size())
	{
		range.w = objectField.size()*Tile::TILE_SIZE - range.x;
	}
			

	for(int x = int(range.x/Tile::TILE_SIZE); x <= int( (range.x+range.w)/Tile::TILE_SIZE); x++)
	{	
		for(int y = int(range.y/Tile::TILE_SIZE); y <= int((range.y+range.h)/Tile::TILE_SIZE); y++)
		{
			tilesInRange.push_back( XYPair<int>(x, y) );
		}
	}

	return tilesInRange;
}

std::vector< XYPair<int> > ObjectField::GetFirstAndLastTileInRange(Rect<double> range)
{
	std::vector< XYPair<int> > tilesInRange;

	//Keeping it on objectField
	if(range.x < 0)
	{
		range.w += range.x; //shorten width
		range.x = 0; 
	}
	if(range.y < 0)
	{
		range.h += range.y; //shorten height
		range.y = 0;
	}
	if( (range.y+range.h)/Tile::TILE_SIZE > (int)objectField[0].size())
	{
		range.h = objectField[0].size()*Tile::TILE_SIZE - range.y;
	}
	//if range extends beyond the width of the tiles, shorten the range
	if( (range.x+range.w)/Tile::TILE_SIZE > (int)objectField.size())
	{
		range.w = objectField.size()*Tile::TILE_SIZE - range.x;
	}

	XYPair<int> firstTile = XYPair<int>(int(range.x/Tile::TILE_SIZE), int(range.y/Tile::TILE_SIZE));
	XYPair<int> lastTile = XYPair<int>( int( (range.x+range.w)/Tile::TILE_SIZE)-1, 
										int((range.y+range.h)/Tile::TILE_SIZE)-1);
	tilesInRange.push_back(firstTile);
	tilesInRange.push_back(lastTile);


	return tilesInRange;
}

//Call this once to correct tile images
void ObjectField::CorrectGateTiles()
{}

void ObjectField::Insert(PlayerCharacter *pc)
{
	pc->tilesIntersected.clear();
	pc->tilesIntersected = GetTilesInRange(pc->GetHitbox());
	for(auto tile = pc->tilesIntersected.begin(); tile != pc->tilesIntersected.end(); tile++)
	{
		collisionGrid[tile->x][tile->y].Insert(pc);
	}

	InsertCharacterReference(pc);
}

void ObjectField::Insert(Monster *monster)
{
	monster->tilesIntersected.clear();
	monster->tilesIntersected = GetTilesInRange(monster->GetHitbox());
	for(auto tile = monster->tilesIntersected.begin(); tile != monster->tilesIntersected.end(); tile++)
	{
		collisionGrid[tile->x][tile->y].Insert(monster);
	}

	InsertCharacterReference(monster);
}

void ObjectField::Remove(PlayerCharacter *pc)
{
	for(auto tile = pc->tilesIntersected.begin(); tile != pc->tilesIntersected.end(); tile++)
	{
		collisionGrid[tile->x][tile->y].Remove(pc);
	}	
	pc->tilesIntersected.clear();

	RemoveCharacterReference((pc->GetID()));
}

void ObjectField::Remove(Monster *monster)
{
	for(auto tile = monster->tilesIntersected.begin(); tile != monster->tilesIntersected.end(); tile++)
	{
		collisionGrid[tile->x][tile->y].Remove(monster);
	}
	monster->tilesIntersected.clear();

	RemoveCharacterReference((monster->GetID()));
}

Spawner *ObjectField::GetSpawner(int tileX, int tileY){ return objectField[tileX][tileY].spawner; }
void ObjectField::SetSpawner(int tileX, int tileY, Spawner *newSpawner)
{
	if(objectField[tileX][tileY].spawner)
	{
		delete objectField[tileX][tileY].spawner;
		objectField[tileX][tileY].spawner = NULL;
	}

	objectField[tileX][tileY].spawner = newSpawner;
}

void ObjectField::RemoveCharacterReference(unsigned int characterID)
{
	if(charactersOnMap.count(characterID) == 1)
	{
		charactersOnMap.erase(characterID);
	}
}

void ObjectField::InsertCharacterReference(GameCharacter *character)
{
	auto id = character->GetID();
	charactersOnMap.insert(std::pair<unsigned int, GameCharacter*>(id, character));
	if(nextCharacterIdAvailable <= id)
	{
		nextCharacterIdAvailable = id+1;
	}
}

GameCharacter *ObjectField::GetCharacterFromID(unsigned int characterID)
{
	GameCharacter *ret = NULL;

	if(charactersOnMap.count(characterID) == 1)
	{
		ret = charactersOnMap[characterID];
	}
	//else do nothing

	return ret;
}

unsigned int ObjectField::GetNextCharacterReferenceID()
{
	return nextCharacterIdAvailable;
}
#include "ObjectField.hpp"
#include "Tile.hpp"

using namespace gauntlet;

ObjectField::ObjectField(){}
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
			

	for(int x = range.x/Tile::TILE_SIZE; x < (range.x+range.w)/Tile::TILE_SIZE; x++)
	{	
		for(int y = range.y/Tile::TILE_SIZE; y < (range.y+range.h)/Tile::TILE_SIZE; y++)
		{
			tilesInRange.push_back( XYPair<int>(x, y) );
		}
	}

	return tilesInRange;
}

//Call this once to correct tile images
void ObjectField::CorrectGateTiles()
{}
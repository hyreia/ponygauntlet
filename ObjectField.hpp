#ifndef OBJECT_FIELD__HPP
#define OBJECT_FIELD__HPP

#include <vector>
#include "RectAndCircle.hpp"
#include "XYPair.hpp"

#include "CollisionGridCell.hpp"
/* A collection of dungeon entities in the tile grid locations where they are
Anything that can appear in more than one tile grid (Projectiles, Monsters, PlayerCharacters) 
don't belong to the ObjectField and are just pointers. Everything else belongs to the ObjectField. */
typedef std::vector< std::vector< gauntlet::CollisionGridCell> > CollisionGrid;

/* A cell in an ObjectField that can contain various objects. 
ObjectField is normally in charge of inserting and removing objects
from these cells. Basically Each cell is a bunch of pointers. Instead of
having a virtual methods or a bunch of dynamic casts I opted for 
a lot of pointers. This means memory usage is higher but it's less messy.*/
namespace gauntlet
{
	class GameCharacter;

	//Things that generally don't impede movement and are types of GameEntities
	class Exit;
	class Item;

	//Things that generally impede movement and are TileObjects
	class Spawner;
	class Lever;
	class BreakableWall;
	class Gate;
	class TreasureChest;
	class Hazard;

	struct ObjectFieldCell
	{
		ObjectFieldCell():
		exit(NULL),
		item(NULL),
		spawner(NULL),
		lever(NULL),
		breakable(NULL),
		gate(NULL),
		chest(NULL),
		hazard(NULL)
		{}
		~ObjectFieldCell(){}

		Exit *exit;
		Item *item;
		Spawner *spawner;
		Lever *lever;
		BreakableWall *breakable;
		Gate *gate;
		TreasureChest *chest;
		Hazard *hazard;
	};

	class ObjectField
	{
	public:
		ObjectField();
		~ObjectField();
		
		void InitializeToSize(int width, int height);
		void Clear();
		std::vector< XYPair<int> > GetTilesInRange(Rect<double> range);
		std::vector< XYPair<int> > GetFirstAndLastTileInRange(Rect<double> range);
		void CorrectGateTiles(); //Call this once to correct tile images

		bool TileHasObjectThatBlocksCharacterFromMoving(GameCharacter *character, int tileX, int tileY);

		/* Keeps pointers to projectiles, monsters and players and keep it publically reachable*/
		CollisionGrid collisionGrid;
		void Insert(PlayerCharacter *pc);
		void Insert(Monster *monster);
		void Remove(PlayerCharacter *pc);
		void Remove(Monster *monster);

		void SetSpawner(int tileX, int tileY, Spawner *newSpawner);
		Spawner *GetSpawner(int tileX, int tileY);

	private:
		std::vector< std::vector< ObjectFieldCell> > objectField;

	};

};

#endif
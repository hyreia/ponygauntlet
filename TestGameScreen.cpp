#include "TestGameScreen.hpp"
#include <allegro5\allegro.h>
#include "GauntletApp.hpp"
#include "GameCharacter.hpp"
#include "GameEntity.hpp"
#include <allegro5/allegro_primitives.h>
#include "Tile.hpp"
#include "Spawner.hpp"
#include <algorithm>


#include "Bitmap.hpp"
using namespace gauntlet; 

TestGameScreen::TestGameScreen()
{
	cameraOffset = Rect<double>(XYPair<double>(0,0), 
		XYPair<double>(Allegro::BUFFER_WIDTH, Allegro::BUFFER_HEIGHT));


	map.Initialize("graveyard.tmx");
	objectField.InitializeToSize(map.GetTileWidthInTiles(), map.GetTileHeightInTiles());
	

	CreatePlayerCharactersFromPlayers();

	if(playerCharacters.size() > 0)
	{
		app->controls.PollMousePosition();
		auto charPos = playerCharacters[0]->GetPosition();
		auto angleToMouse = app->controls.GetAngleToMouse(charPos.x, charPos.y);
		playerCharacters[0]->SetFacingAngle(angleToMouse);
		playerCharacters[0]->UpdateAnimation();
		
	}

	//Create test monster
	Monster *newMonster = new Monster(XYPair<double>(playerCharacters[0]->GetPosition().x,playerCharacters[0]->GetPosition().y+400), DIR_DOWN, app->data.monsterTypes[SPOOKY_BONES]);
	objectField.Insert(newMonster);
	monstersInView.push_back(newMonster);



	intervalBetweenMonsterSpawners = 3.0;
	timeUntilNextSpawnerIsCreated = intervalBetweenMonsterSpawners;
	
}

TestGameScreen::~TestGameScreen(){}

void TestGameScreen::HandleInput(ALLEGRO_EVENT &ev)
{
	switch(ev.type)
	{
	case ALLEGRO_EVENT_DISPLAY_CLOSE:
		app->SetIsTimeToQuit();
			break;
	case ALLEGRO_EVENT_MOUSE_AXES:
		app->allegro.TranslateMousePositionToBufferPosition(ev.mouse.x, ev.mouse.y);
		app->controls.ReportMousePosition(ev.mouse.x, ev.mouse.y);
			break;
		default:
			app->controls.HandlePlayerControls(ev);
			break;
	}
}
void TestGameScreen::Render(double alpha)
{
	GameCharacter::drawAlpha = alpha;
	app->allegro.ClearBufferToColor(64,32,32);

	map.DrawGroundLayerRegion(0, 0, app->allegro.BUFFER_WIDTH,
		app->allegro.BUFFER_HEIGHT, 0, 0, 0);


	//map.DrawCeilingLayerRegion(0, 0, app->allegro.BUFFER_WIDTH,
		//app->allegro.BUFFER_HEIGHT, 0, 0, 0);

	std::vector< GameCharacter* > gameCharacters;

	for(auto sIter = spawnersInView.begin(); sIter != spawnersInView.end(); sIter++)
	{
		auto bitmap = (*sIter)->GetSourceBitmap();
		auto imageBox = (*sIter)->GetImageBox();
		auto imageSource = (*sIter)->ImageSource();
		bitmap->Draw(imageSource.x, imageSource.y, imageBox.w, imageBox.h, imageBox.x, imageBox.y);
	}

	gameCharacters.reserve( monstersInView.size() + playerCharacters.size() );
	gameCharacters.insert(gameCharacters.begin(), monstersInView.begin(), monstersInView.end());
	gameCharacters.insert(gameCharacters.begin(), playerCharacters.begin(), playerCharacters.end());
	std::stable_sort(gameCharacters.begin(), gameCharacters.end(), CompareCharactersByYAxis);

	double ceilingLayerVerticalDrawDistance = 0;
	for(auto charIter = gameCharacters.begin(); charIter != gameCharacters.end(); charIter++)
	{
		auto shadow = (*charIter)->GetHitboxAtDeltaTime(alpha);
		al_draw_filled_ellipse(shadow.x + shadow.w/2, shadow.y + shadow.h/2, 
			shadow.w, shadow.h*(2.0/3.0), 
			al_map_rgba(0,0,0,192));
		//bitmap->Draw(imageSource.x, imageSource.y, imageBox.w, imageBox.h, imageBox.x, imageBox.y, imageFlags);
	}

	map.DrawWallLayerRegion(0, 0, app->allegro.BUFFER_WIDTH,
		app->allegro.BUFFER_HEIGHT, 0, 0, 0);


	for(auto charIter = gameCharacters.begin(); charIter != gameCharacters.end(); charIter++)
	{
		auto character = (*charIter);
		auto bitmap = character->GetSourceBitmap();
		auto imageBox = character->GetImageBoxAtDeltaTime(alpha);
		auto imageSource = character->ImageSource();
		auto imageFlags = character->ImageDrawingFlags();
		//auto shadow = character->GetHitboxAtDeltaTime(alpha);


		/*while(ceilingLayerVerticalDrawDistance+Tile::TILE_SIZE > imageBox.Y2())
		{
			map.DrawCeilingLayerRegion(0, ceilingLayerVerticalDrawDistance, app->allegro.BUFFER_WIDTH, Tile::TILE_SIZE,
				0, ceilingLayerVerticalDrawDistance, 0);
			//do
			//{
				//printf("IMAGE BOX: %f, Drawing From %f %f\n", imageBox.y, ceilingLayerVerticalDrawDistance, ceilingLayerVerticalDrawDistance+Tile::TILE_SIZE);
				//map.DrawCeilingLayerRegion(0, ceilingLayerVerticalDrawDistance, app->allegro.BUFFER_WIDTH, Tile::TILE_SIZE,
					//0, ceilingLayerVerticalDrawDistance, 0);
					//ceilingLayerVerticalDrawDistance += Tile::TILE_SIZE;
			//} while( ceilingLayerVerticalDrawDistance+Tile::TILE_SIZE < imageBox.y );
			ceilingLayerVerticalDrawDistance+= Tile::TILE_SIZE;	
		}*/

		bitmap->Draw(imageSource.x, imageSource.y, imageBox.w, imageBox.h, imageBox.x, imageBox.y, imageFlags);


	}
	while(ceilingLayerVerticalDrawDistance < app->allegro.BUFFER_HEIGHT)
	{
		map.DrawCeilingLayerRegion(0, ceilingLayerVerticalDrawDistance, app->allegro.BUFFER_WIDTH, Tile::TILE_SIZE,
			0, ceilingLayerVerticalDrawDistance, 0);
		ceilingLayerVerticalDrawDistance += Tile::TILE_SIZE;
	}



} 
void TestGameScreen::Update()
{
	UpdateCharactersFromPlayerInput();
	UpdateMonstersFromAI();
	PerformCollisionsForCharacters();

	CalculateTilesInView();
	FindMonstersAndSpawnersInView();


	/* Update attacks (move, animate, collide) */

	/* Update Spawners and ObjectField objects */
	UpdateMonsterSpawners();
	CreateSpawners();

	/* Update debris and lose dungeon animations, etc */
	/* Update any gates */
	/* Update action bar */
}

void TestGameScreen::UpdateCharactersFromPlayerInput()
{
	for(auto ch = playerCharacters.begin(); ch != playerCharacters.end(); ch++)
	{
		/* Directional facing */
		auto character = (*ch);
		if(!character->IsFacingAngleLocked())
		{
			auto charPos = character->GetPosition();
			auto angleToMouse = app->controls.GetAngleToMouse(charPos.x, charPos.y);
			character->SetFacingAngle(angleToMouse);
			character->UpdateAnimation();
		}

		auto id = character->GetPlayer().ID;
		auto controls = app->controls.GetPlayerInput(id);

		/* Update desired velocity */
		if(!character->isMovementLocked())
		{
			double MULTIPLIER = 1.7;
			auto player = character->GetPlayer();
			auto initialVelocity = controls->GetMoveVector();
			auto speed = double(app->players.GetPlayer(1).SPD() * MULTIPLIER) / app->TPS;
			//auto speed = (app->players.GetPlayer(1).SPD() * app->dt) + speedBase;
			//speed = (200 * 1/20) + speedBase


			initialVelocity.x *= speed;
			initialVelocity.y *= speed;

			
			if(character->GetWasTileBlocked() )
			{
				character->SetWasTileBlocked(false);

				auto playerHitbox = character->GetHitbox();

				const double MAX_ADJUST_DISTANCE = 0.2;
				//Adjust so it's centering on closest tile
				XYPair<double> closestTileCenterPos = XYPair<double>( floor(playerHitbox.Center().x / Tile::TILE_SIZE) * Tile::TILE_SIZE, 
					floor(playerHitbox.Center().y / Tile::TILE_SIZE) * Tile::TILE_SIZE);

				XYPair<double> differenceFromTile(closestTileCenterPos.x - playerHitbox.x,
					closestTileCenterPos.y - playerHitbox.y);
				if( differenceFromTile.x > MAX_ADJUST_DISTANCE) differenceFromTile.x = MAX_ADJUST_DISTANCE;
				else if( differenceFromTile.x < -MAX_ADJUST_DISTANCE) differenceFromTile.x = -MAX_ADJUST_DISTANCE;
				if( differenceFromTile.y > MAX_ADJUST_DISTANCE) differenceFromTile.y = MAX_ADJUST_DISTANCE;
				else if( differenceFromTile.y < -MAX_ADJUST_DISTANCE) differenceFromTile.y = -MAX_ADJUST_DISTANCE;


				initialVelocity.x += differenceFromTile.x;
				initialVelocity.y += differenceFromTile.y;
			}

			character->AddStepsToAnimation(speed / app->TPS);

			character->SetVelocity(initialVelocity.x, initialVelocity.y);
		}
		else character->SetVelocity(0,0);

		/* Respond to actions */
		if(controls->buttons[USE_MAIN_ITEM].IsPressed())
		{}
		if(controls->buttons[USE_SEC_ITEM].IsPressed())
		{}
		if(controls->buttons[INC_SECONDARY].IsPressed())
		{}
		if(controls->buttons[DEC_SECONDARY].IsPressed())
		{}
	}
}

void TestGameScreen::PerformCollisionsForCharacters()
{
	std::vector< GameCharacter* > characters;
	characters.insert(characters.begin(), playerCharacters.begin(), playerCharacters.end());
	characters.insert(characters.begin(), monstersInView.begin(), monstersInView.end());

	bool isCircleCircleCollisionCrazy = true;

	for(auto c = characters.begin(); c != characters.end(); c++)
	{
		auto character = (*c);
		
		//Get a rect covering all potential movement
		auto vel = character->GetVelocity();

		auto topleft = character->GetHitbox();
		auto bottomRight = character->ProjectedNextPosition();
		if(bottomRight.x < topleft.x)
		{
			auto temp = topleft.x;
			topleft.x = bottomRight.x;
			bottomRight.x = temp;
		}
		if(bottomRight.y < topleft.y)
		{
			auto temp = topleft.y;
			topleft.y = bottomRight.y;
			bottomRight.y = temp;
		}
		Rect<double> movementRange(topleft.x, topleft.y,
					bottomRight.x - topleft.x + topleft.w, 
					bottomRight.y - topleft.y + topleft.h);

		std::vector< XYPair<int> >tilesIntersected = objectField.GetTilesInRange(movementRange);
		//Get characters that might be overlapping this character, we don't care if they're monsters or players in this instance
		const size_t ARBITRARY_REASONABLE_MAX_CHARACTERS_OVERLAPPING = 12;
		std::vector< GameCharacter *> charactersOverlappingTiles;
		charactersOverlappingTiles.reserve(ARBITRARY_REASONABLE_MAX_CHARACTERS_OVERLAPPING);
		for(auto tileIter = tilesIntersected.begin(); tileIter != tilesIntersected.end(); tileIter++)
		{
			//TODO: only insert unique
			charactersOverlappingTiles.insert( charactersOverlappingTiles.begin(),
					objectField.collisionGrid[tileIter->x][tileIter->y].playersOverlappingCell.begin(),
					objectField.collisionGrid[tileIter->x][tileIter->y].playersOverlappingCell.end());
			charactersOverlappingTiles.insert( charactersOverlappingTiles.begin(),
					objectField.collisionGrid[tileIter->x][tileIter->y].monstersOverlappingCell.begin(),
					objectField.collisionGrid[tileIter->x][tileIter->y].monstersOverlappingCell.end());

		}

		
		auto velocity = character->GetVelocity();
		for(auto otherC = characters.begin(); otherC != characters.end(); otherC++)
		{
			if( (*otherC) != character)
			{
				auto charBox = character->GetHitbox();
				charBox.y += character->GetVelocity().y;
				auto otherBox = (*otherC)->GetHitbox();

				if(velocity.y != 0.0)
				{
					if(velocity.y>0 && (*otherC)->GetHitbox().IsHorizontalLineIntersecting(charBox.x+1, charBox.X2()-1, charBox.Bottom()) ) //Going Down
					{
						velocity.y = otherBox.Top() - charBox.Bottom();
						if( velocity.y>0) character->SetYVelocity( velocity.y );
						else character->SetYVelocity(0);
					}
					if(velocity.y<0 && (*otherC)->GetHitbox().IsHorizontalLineIntersecting(charBox.x+1, charBox.X2()-1, charBox.Top()) ) //Going Up
					{
						velocity.y = otherBox.Bottom() - charBox.Top();
						if( velocity.y<0) character->SetYVelocity( velocity.y );
						else character->SetYVelocity(0);
					}
				}

				velocity = character->GetVelocity();
				charBox = character->GetHitbox();
				charBox.x += velocity.x;

				if(velocity.x != 0.0)
				{
					if(velocity.x>0 && (*otherC)->GetHitbox().IsVerticalLineIntersecting(charBox.Right(), charBox.y+1, charBox.Y2()-1) ) //Going Right
					{
						velocity.x = otherBox.Left() - charBox.Right();
						if( velocity.x>0) character->SetXVelocity( velocity.x );
						else character->SetXVelocity(0);

					}
					if(velocity.x<0 && (*otherC)->GetHitbox().IsVerticalLineIntersecting(charBox.Left(), charBox.y+1, charBox.Y2()-1) ) //Going Left
					{
						velocity.x = otherBox.Right() - charBox.Left();
						if( velocity.x<0) character->SetXVelocity( velocity.x );
						else character->SetXVelocity(0);
					}
				}

			}
		}
		
		
		//Avoid the tiles
		for(auto tile = tilesIntersected.begin(); tile != tilesIntersected.end(); tile++)
		{
			if(!map.CanBeWalkedOn(tile->x, tile->y))
			{
				auto tileBox = Tile::TileToRect(tile->x, tile->y);
				auto hitbox = character->GetHitbox();
				auto velocity = character->GetVelocity();
				hitbox.y += velocity.y;

				if(velocity.y != 0.0)
				{
					if(velocity.y>0 && tileBox.IsHorizontalLineIntersecting(hitbox.x+1, hitbox.X2()-1, hitbox.Bottom()) ) //Going Down
					{
						character->SetWasTileBlocked(true);
						velocity.y = tileBox.Top() - hitbox.Bottom();
						if(velocity.y>0) character->SetYVelocity( velocity.y );
						else character->SetYVelocity(0);
					}
					if(velocity.y<0 && tileBox.IsHorizontalLineIntersecting(hitbox.x+1, hitbox.X2()-1, hitbox.Top()) ) //Going Up
					{
						character->SetWasTileBlocked(true);
						velocity.y = tileBox.Bottom() - hitbox.Top();
						if(velocity.y<0) character->SetYVelocity( velocity.y );
						else character->SetYVelocity(0);
					}
				}
			
				velocity = character->GetVelocity();
				hitbox = character->GetHitbox();
				hitbox.x += velocity.x;

				if(velocity.x != 0.0)
				{
					if(velocity.x>0 && tileBox.IsVerticalLineIntersecting(hitbox.Right(), hitbox.y+1, hitbox.Y2()-1) ) //Going Right
					{
						character->SetWasTileBlocked(true);
						velocity.x = tileBox.Left() - hitbox.Right();
						if( velocity.x>0 ) character->SetXVelocity( velocity.x );
						else character->SetXVelocity(0);
					}
					if(velocity.x<0 && tileBox.IsVerticalLineIntersecting(hitbox.Left(), hitbox.y+1, hitbox.Y2()-1) ) //Going Left
					{
						character->SetWasTileBlocked(true);
						velocity.x = tileBox.Right() - hitbox.Left();
						if( velocity.x<0) character->SetXVelocity( velocity.x );
						else character->SetXVelocity(0);
					}
				}
			}
		}

		character->SetLastPositionToCurrentPosition();
		Monster *monPtr = dynamic_cast<Monster*>(character);
		PlayerCharacter *playPtr = dynamic_cast<PlayerCharacter*>(character);
		if(monPtr)
		{
			objectField.Remove(monPtr);
			character->AddVelocityToPosition();
			objectField.Insert(monPtr);
		}
		else if(playPtr)
		{
			objectField.Remove(playPtr);
			character->AddVelocityToPosition();
			objectField.Insert(playPtr);
		}
		

	}
}
void TestGameScreen::CreatePlayerCharactersFromPlayers()
{
	auto startPositions = map.GetPlayerStartPositions();
	size_t startPos = 0;

	//For each active player, create them at the next available startPosition, if we run out of start positions loop back around
	for(int playerID = 1; playerID < app->players.MAX_PLAYERS; playerID++)
	{
		Player &player = app->players.GetPlayer(playerID);
		if(player.isPlaying)
		{

			PlayerCharacter *newPC = new PlayerCharacter(startPositions[startPos], player);
			objectField.Insert(newPC);

			playerCharacters.push_back(newPC);

			startPos++;
			if(startPos >= startPositions.size()) startPos = 0;
		}
	}


}

int TestGameScreen::GetPositionOfClosestPlayer(XYPair<double> &position)
{
	int ret = -1;

	double distanceToClosestSquared = 0;

	for(int p = 0; p < (int)playerCharacters.size(); p++)
	{
		auto playerPos = playerCharacters[p]->GetPosition();
		double distanceToThisPlayerSquared = (position.x-playerPos.x)*(position.x-playerPos.x) + (position.y-playerPos.y)*(position.y-playerPos.y);
		if(ret == -1 || distanceToClosestSquared > distanceToThisPlayerSquared)
		{
			ret = p;
			distanceToClosestSquared = distanceToThisPlayerSquared;
		}
	}

	if(ret != -1)
	{
		position = playerCharacters[ret]->GetPosition();
	}
	return ret;
}

void TestGameScreen::UpdateMonstersFromAI()
{
	for(auto mon = monstersInView.begin(); mon != monstersInView.end(); mon++)
	{
		(*mon)->DecrementThinkCooldown();
		if((*mon)->IsAbleToThink())
		{
			int monsterAi = (*mon)->GetAiType();
			if(monsterAi == BASIC_MONSTER_AI)
			{
				XYPair<double> position = (*mon)->GetPosition();
				auto playerID = GetPositionOfClosestPlayer(position);
				if(playerID != -1)
				{
					(*mon)->LookAt(position);
				} //else no player found

				(*mon)->SetVelocityToForward();
				(*mon)->AddStepSpeedToAnimation();
				(*mon)->UpdateAnimation();
			}
			(*mon)->ResetThinkCooldown();
		}
		(*mon)->UpdateAnimation();
	}
}

void TestGameScreen::CalculateTilesInView()
{
	auto tiles = objectField.GetFirstAndLastTileInRange(cameraOffset);

	firstTileInView = tiles.front();
	lastTileInView = tiles.back();
}

void TestGameScreen::FindMonstersAndSpawnersInView()
{

	const size_t TILE_WIDTH = Tile::TILE_SIZE;
	size_t x1 = size_t(cameraOffset.x)/TILE_WIDTH;
	size_t y1 = size_t(cameraOffset.y)/TILE_WIDTH;
	size_t x2 = size_t(cameraOffset.X2())/TILE_WIDTH;
	size_t y2 = size_t(cameraOffset.Y2())/TILE_WIDTH;

	spawnersInView.clear();
	monstersInView.clear();
	for(size_t y = y1; y <= y2; y++)
	{
		for(size_t x = x1; x <= x2; x++)
		{
			if(!objectField.collisionGrid[x][y].monstersOverlappingCell.empty())
			{
				auto monsterIterator = objectField.collisionGrid[x][y].monstersOverlappingCell.begin();
				while(monsterIterator != objectField.collisionGrid[x][y].monstersOverlappingCell.end())
				{

					if( std::find(monstersInView.begin(), monstersInView.end(), (*monsterIterator)) == monstersInView.end()
						&& (*monsterIterator)->GetHitbox().IsOverlapping(cameraOffset) )
					{

						//monstersInRange.push_back( (*monsterIterator)); 
						monstersInView.push_back( (*monsterIterator));
					}
					monsterIterator++;
				}
			}
			if(auto spawner = objectField.GetSpawner(x,y))
			{
				spawnersInView.push_back(spawner);
			}
		}
	}


}

void TestGameScreen::UpdateMonsterSpawners()
{
	//Update MonsterSpawners
	for(auto monsterSpawnerIter = spawnersInView.begin(); monsterSpawnerIter != spawnersInView.end(); /*monsterSpawnerIter++*/)
	{
		auto spawner = (*monsterSpawnerIter);

		spawner->AddTimeToAnimation(app->dt);

		//Time to create a monster
		if( spawner->DecrementSpawnCooldown() == 0)
		{
			bool wasMonsterSpawned = false;
			
			if(spawner->GetSpawnsLeft() != 0) //-1 is unlimited
			{
				auto monsterType = spawner->GetMonsterTypeToSpawn();
				
				//find where to spawn the monster
				if(!spawner->WillBlockMovement())
				{
					XYPair<double> spawnLocation = spawner->GetCenterOfTile();
					//spawn on the spawner
					Monster *newMonster = new Monster(spawnLocation, DIR_DOWN, *monsterType);
					objectField.Insert(newMonster);
					monstersInView.push_back(newMonster);
					wasMonsterSpawned = true;
					spawner->ResetSpawnCooldown();
					spawner->DecrementSpawnsLeft();
				}
				else //spawn next to it
				{
					/* we'll create a list of candiate spots for it to spawn and check which are valid
					then rand()omly choose from those valid spots */
					std::vector< XYPair<double> > possibleSpawnLocations;

					auto monsterRadius = monsterType->radius;
					auto spawnerLocation = spawner->GetTileRect();
					
					/*calculate where the monster would spawn in the 8 surrounding areas*/
					//above
					possibleSpawnLocations.push_back(XYPair<double>(spawnerLocation.Center().x,
														spawnerLocation.Top() - monsterRadius));
					//above right
					possibleSpawnLocations.push_back(XYPair<double>(spawnerLocation.Right()+monsterRadius,
						spawnerLocation.Top()-monsterRadius));
					//right
					possibleSpawnLocations.push_back(XYPair<double>(spawnerLocation.Right()+monsterRadius,
						spawnerLocation.Center().y));
					//bottom right
					possibleSpawnLocations.push_back(XYPair<double>(spawnerLocation.Right()+monsterRadius,
						spawnerLocation.Bottom()+monsterRadius));
					//bottom
					possibleSpawnLocations.push_back(XYPair<double>(spawnerLocation.Center().x,
						spawnerLocation.Bottom()+monsterRadius));
					//bottom left
					possibleSpawnLocations.push_back(XYPair<double>(spawnerLocation.Left()-monsterRadius,
						spawnerLocation.Bottom()-monsterRadius));
					//left
					possibleSpawnLocations.push_back(XYPair<double>(spawnerLocation.Left()-monsterRadius,
						spawnerLocation.Center().y));
					//above left
					possibleSpawnLocations.push_back(XYPair<double>(spawnerLocation.Left()-monsterRadius,
						spawnerLocation.Top()-monsterRadius));

					//check each location to see if they are free, if not free remove from list
					auto locationIter = possibleSpawnLocations.begin();
					while(locationIter != possibleSpawnLocations.end())
					{
						bool isClear = true;
						Circle<double> locationBeingTested(locationIter->x, locationIter->y, monsterRadius);

						//check tiles overlapping test location
						auto tilesInRange = objectField.GetTilesInRange(locationBeingTested.Squared());
						for(auto tileInRange = tilesInRange.begin(); tileInRange != tilesInRange.end(); tileInRange++)
						{
							auto tileRect = Tile::TileToRect(tileInRange->x, tileInRange->y);
							if(IsRectIntersectingCircle(locationBeingTested.cx, locationBeingTested.cy,
								locationBeingTested.radius, tileRect))
							{
								if(map.IsThereAWallAt(tileInRange->x, tileInRange->y) ||
									(monsterType->startAltitude == 0 && !map.IsThereAFloorAt(tileInRange->x,
										tileInRange->y))
								  )
								{
									//nothing can stand there at that altitude
									isClear = false;
									break;
								}
								else //is anything else standing in the way?
								{
									if(!objectField.collisionGrid[tileInRange->x][tileInRange->y].monstersOverlappingCell.empty() ||
										!objectField.collisionGrid[tileInRange->x][tileInRange->y].playersOverlappingCell.empty())
									{
										isClear = false;
										break;
									}
								}

							}
						}
						
						if(isClear) locationIter++;
						else 
						{
							locationIter = possibleSpawnLocations.erase(locationIter);
						}
					} //while locationIter isn't done
					
					
					//if we have any free locations left, randomly chose from those
					if(!possibleSpawnLocations.empty())
					{
						auto spawnLocation = possibleSpawnLocations[rand()%possibleSpawnLocations.size()];
						Monster *newMonster = new Monster(spawnLocation, DIR_DOWN, *monsterType);
						objectField.Insert(newMonster);
						monstersInView.push_back(newMonster);
						wasMonsterSpawned = true;
						spawner->DecrementSpawnsLeft();
						spawner->ResetSpawnCooldown();
					}
				} //else spawn next to it
			} //if spawns left is not -1
		} //if time to spawn monster

			if((*monsterSpawnerIter)->GetSpawnsLeft() > 0)
			{
				monsterSpawnerIter++;
			}
			else //there are no spawns left for this spawner, destroy it
			{
				monsterSpawnerIter = spawnersInView.erase(monsterSpawnerIter);
			}

		}//iterating over each monster spawner in view
}

void TestGameScreen::CreateSpawners()
{
	timeUntilNextSpawnerIsCreated -= app->dt;
	if(timeUntilNextSpawnerIsCreated <= 0.0)
	{
		//For now we just grab this one
		auto monsterType = (&app->data.spawnerTypes[SHALLOW_GRAVE]);

		int tileX=0, tileY=0;
		if(FoundOpenTileForMonsterSpawner(tileX, tileY))
		{
			Spawner *spawner = new Spawner(monsterType, tileX, tileY);
			objectField.SetSpawner(tileX, tileY, spawner);			
		}
		timeUntilNextSpawnerIsCreated = intervalBetweenMonsterSpawners;
	}

}

bool TestGameScreen::FoundOpenTileForMonsterSpawner(int &x, int &y)
{
	bool wasTileFound = false;

	std::vector< XYPair<int> > openTiles;
	
	

	//Find all tiles that are unblocked, clear of others and not blocked
	for(int tileY = 0; tileY < map.GetTileHeightInTiles(); tileY++)
	for(int tileX = 0; tileX < map.GetTileWidthInTiles(); tileX++)
	{
		if(!map.IsThereAWallAt(tileX, tileY))
		{
			if(objectField.collisionGrid[tileX][tileY].monstersOverlappingCell.empty() &&
				objectField.collisionGrid[tileX][tileY].playersOverlappingCell.empty() &&
				!objectField.GetSpawner(tileX, tileY))
			{
				openTiles.push_back(XYPair<int>(tileX, tileY));
			}
		}
	}

	if(!openTiles.empty())
	{
		wasTileFound = true;

		int chosenTileIndex = rand()% openTiles.size();
		x = openTiles[chosenTileIndex].x;
		y = openTiles[chosenTileIndex].y;
	}
	//else no tile was found
	
	return wasTileFound;
}
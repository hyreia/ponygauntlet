#include "TestGameScreen.hpp"
#include <allegro5\allegro.h>
#include "GauntletApp.hpp"
#include "GameCharacter.hpp"
#include "GameEntity.hpp"
#include <allegro5/allegro_primitives.h>
#include "Tile.hpp"
#include "Spawner.hpp"
#include <algorithm>
#include"Attack.hpp"

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
	unsigned int id = objectField.GetNextCharacterReferenceID();
	Monster *newMonster = new Monster(id, XYPair<double>(playerCharacters[0]->GetPosition().x,playerCharacters[0]->GetPosition().y+400), DIR_DOWN, app->data.monsterTypes[SPOOKY_BONES]);
	objectField.Insert(newMonster);
	monstersInView.push_back(newMonster);



	intervalBetweenMonsterSpawners = 3.0;
	timeUntilNextSpawnerIsCreated = intervalBetweenMonsterSpawners;
	
	actionHUD = Bitmap::Load("action icons.png", false);
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
		if(ev.mouse.dz != 0) app->controls.AdjustMouseZPosition(ev.mouse.dz>0);
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
	}


	for(auto animation = animationsOnFloor.begin(); animation != animationsOnFloor.end(); animation++)
	{
		XYPair<double> center = animation->offset;
		auto type = animation->animation.type;
		auto imageBox = animation->animation.GetImageBox(center.x, center.y);
		auto imageSource = animation->animation.ImageSource();
		auto angle = animation->animation.rotation;

		if(animation->characterIndexAnimationIsCenteredOn != -1)
		{
			auto character = objectField.GetCharacterFromID(animation->characterIndexAnimationIsCenteredOn);
			if(character)
			{
				auto charPos = character->GetHitbox().Center();
				imageBox.x += charPos.x;
				imageBox.y += charPos.y;
			}
		}

		type->bitmap->DrawRotated(imageSource.x, imageSource.y, imageBox.w, imageBox.h, imageBox.x+imageBox.w/2, imageBox.y+imageBox.h/2, 
			imageBox.w/2, imageBox.h/2, angle);
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

		auto hitbox = (*charIter)->GetHitbox();
		//al_draw_rectangle(hitbox.x, hitbox.y, hitbox.X2(), hitbox.Y2(), al_map_rgb(0,255,0), 2);
		al_draw_rectangle(imageBox.x, imageBox.y, imageBox.X2(), imageBox.Y2(), al_map_rgb(0,255,0), 2);

		bitmap->Draw(imageSource.x, imageSource.y, imageBox.w, imageBox.h, imageBox.x, imageBox.y, imageFlags);
	}

	for(auto animation = animationsInSky.begin(); animation != animationsInSky.end(); animation++)
	{
		XYPair<double> center = animation->offset;
		auto type = animation->animation.type;
		auto imageBox = animation->animation.GetImageBox(center.x, center.y);
		auto imageSource = animation->animation.ImageSource();
		auto angle = animation->animation.rotation;

		if(animation->characterIndexAnimationIsCenteredOn != -1)
		{
			auto character = objectField.GetCharacterFromID(animation->characterIndexAnimationIsCenteredOn);
			if(character)
			{
				auto charPos = character->GetHitbox().Center();
				imageBox.x += charPos.x;
				imageBox.y += charPos.y;
			}
		}

		type->bitmap->DrawRotated(imageSource.x, imageSource.y, imageBox.w, imageBox.h, imageBox.x+imageBox.w/2, imageBox.y+imageBox.h/2, 
			imageBox.w/2, imageBox.h/2, angle);
	}

	map.DrawCeilingLayerRegion(0, 0, app->allegro.BUFFER_WIDTH,
		app->allegro.BUFFER_HEIGHT, 0, 0, 0);




	for(auto attack = attacksInView.begin(); attack != attacksInView.end(); attack++)
	{
		auto bitmap = (*attack)->imageBitmap;
		if(bitmap)
		{
			auto hitbox = (*attack)->GetHitboxAtDeltaTime(0);

			auto imageSource = (*attack)->animation->ImageSource();
			auto imageBox = (*attack)->animation->GetImageBox(hitbox.x + hitbox.w/2, hitbox.y + hitbox.h/2);

			auto animation = (*attack)->animation;
		
			double half = animation->type->HALF_DIMENSION;

			auto angle = (*attack)->GetFacing();

			//bitmap->Draw(imageSource.x, imageSource.y, imageBox.w, imageBox.h, 
				//imageBox.x, imageBox.y, imageFlags);
			bitmap->DrawRotated(imageSource.x,imageSource.y, imageBox.w, imageBox.h, imageBox.Center().x, imageBox.Center().y,
				imageBox.w/2, imageBox.h/2, 
				angle);
			al_draw_rectangle(hitbox.x, hitbox.y, hitbox.X2(), hitbox.Y2(), al_map_rgb(255,255,255), 2);
		}
		//else do nothing; some attacks have no animation at their position
	}

	RenderHUD();

} 

void TestGameScreen::RenderHUD()
{
		const ALLEGRO_COLOR EMPTY_BAR_COLOR = al_map_rgb(0,0,0);
		const ALLEGRO_COLOR HEALTH_BAR_COLOR = al_map_rgb(255,0,0);
		const ALLEGRO_COLOR ENERGY_BAR_COLOR = al_map_rgb(255,255,0);
		const ALLEGRO_COLOR FONT_COLOR = al_map_rgb(255,255,255);


		const double HUD_BAR_X = 100;
		const double HUD_BAR_LENGTH = 300;
		const double HUD_BAR_HEIGHT = 40;

		const double HEALTH_BAR_Y = app->allegro.BUFFER_HEIGHT-120;
		const double ENERGY_BAR_Y = HEALTH_BAR_Y+HUD_BAR_HEIGHT+10;
		const double NAME_LINE_Y = HEALTH_BAR_Y-24;
		const double CREDIT_LINE_Y = ENERGY_BAR_Y+HUD_BAR_HEIGHT+10;

		const double ACTION_BAR_X = HUD_BAR_X + HUD_BAR_LENGTH + 50;
		const int ACTIONS_PER_ROW_IN_IMG = 8;
		const double ACTION_ICON_SIZE = 64;
		double SECONDARY_ACTION_X = ACTION_BAR_X+ACTION_ICON_SIZE+10;

		auto player = app->players.GetPlayer(1);

		//health
		al_draw_filled_rectangle(HUD_BAR_X, HEALTH_BAR_Y, HUD_BAR_X+HUD_BAR_LENGTH, HEALTH_BAR_Y+HUD_BAR_HEIGHT, 
			EMPTY_BAR_COLOR);
		al_draw_filled_rectangle(HUD_BAR_X, HEALTH_BAR_Y, HUD_BAR_X + 
			(HUD_BAR_LENGTH * (player.health / player.maxHealth)), HEALTH_BAR_Y+HUD_BAR_HEIGHT, 
			HEALTH_BAR_COLOR);

		//energy
		al_draw_filled_rectangle(HUD_BAR_X, ENERGY_BAR_Y, HUD_BAR_X+HUD_BAR_LENGTH, ENERGY_BAR_Y+HUD_BAR_HEIGHT, 
			EMPTY_BAR_COLOR);
		al_draw_filled_rectangle(HUD_BAR_X, ENERGY_BAR_Y, HUD_BAR_X + 
			(HUD_BAR_LENGTH * ( player.energy / player.maxEnergy)), ENERGY_BAR_Y+HUD_BAR_HEIGHT, 
			ENERGY_BAR_COLOR);

		//name
		app->data.fonts[MESSAGE_FONT].DrawTxt(HUD_BAR_X, NAME_LINE_Y, TEXT_ALIGN_LEFT,
			FONT_COLOR.r, FONT_COLOR.g, FONT_COLOR.b, FONT_COLOR.a, player.name.c_str());

		app->data.fonts[MESSAGE_FONT].DrawTxtf(HUD_BAR_X, CREDIT_LINE_Y, TEXT_ALIGN_LEFT,
			FONT_COLOR.r, FONT_COLOR.g, FONT_COLOR.b, FONT_COLOR.a, "%05d BITS", player.credit);

		//action hud
		auto mainAttack = player.characterType->mainAttack;
		double actionSX = (mainAttack.iconIndex%ACTIONS_PER_ROW_IN_IMG)*ACTION_ICON_SIZE;
		double actionSY = (mainAttack.iconIndex/ACTIONS_PER_ROW_IN_IMG)*ACTION_ICON_SIZE;
		actionHUD->Draw(actionSX, actionSY, ACTION_ICON_SIZE, ACTION_ICON_SIZE,
			ACTION_BAR_X, HEALTH_BAR_Y);
		al_draw_rectangle(ACTION_BAR_X, HEALTH_BAR_Y, ACTION_BAR_X+ACTION_ICON_SIZE,
			HEALTH_BAR_Y+ACTION_ICON_SIZE, al_map_rgb(0,0,0), 4);
		
		for(auto actionIter = player.characterType->secondaryInherentActions.begin();
			actionIter != player.characterType->secondaryInherentActions.end();
			actionIter++)
		{
			int action = (*actionIter).iconIndex;
			double actionSX = (action%ACTIONS_PER_ROW_IN_IMG)*ACTION_ICON_SIZE;
			double actionSY = (action/ACTIONS_PER_ROW_IN_IMG)*ACTION_ICON_SIZE;
			actionHUD->Draw(actionSX, actionSY, ACTION_ICON_SIZE, ACTION_ICON_SIZE,
			SECONDARY_ACTION_X,
			HEALTH_BAR_Y);
			SECONDARY_ACTION_X += ACTION_ICON_SIZE+10;
		}
		SECONDARY_ACTION_X = ACTION_BAR_X+ACTION_ICON_SIZE+10 + ( (ACTION_ICON_SIZE+10) * actionBar.selectedNode);
		
		al_draw_rectangle(SECONDARY_ACTION_X, HEALTH_BAR_Y, SECONDARY_ACTION_X+ACTION_ICON_SIZE,
			HEALTH_BAR_Y+ACTION_ICON_SIZE, al_map_rgb(255,255,255), 4);
		/*al_draw_rectangle(SECONDARY_ACTION_X, HEALTH_BAR_Y, SECONDARY_ACTION_X+ACTION_ICON_SIZE,
			HEALTH_BAR_Y+ACTION_ICON_SIZE, FONT_COLOR, 2);*/
}

void TestGameScreen::Update()
{
	RebuildActionBar();
	
	UpdateCharactersFromPlayerInput();
	UpdateMonstersFromAI();

	PerformCollisionsForCharacters();

	CalculateTilesInView();
	FindMonstersAndSpawnersInView();


	/* Update attacks (move, animate, collide) */
	UpdateAttacks();
	UpdateAnimations();

	/* Update Spawners and ObjectField objects */
	UpdateMonsterSpawners();
	CreateSpawners();

	/* Update debris*/
	/* Update any gates */
}

void TestGameScreen::UpdateCharactersFromPlayerInput()
{
	for(auto ch = playerCharacters.begin(); ch != playerCharacters.end(); ch++)
	{
		/* Directional facing */
		auto character = (*ch);
		if(!character->IsRotationLockedFromAnAttack())
		{
			auto charPos = character->GetPosition();
			auto angleToMouse = app->controls.GetAngleToMouse(charPos.x, charPos.y);
			character->SetFacingAngle(angleToMouse);
			character->UpdateAnimation();
		}

		auto id = character->GetPlayer().ID;
		auto controls = app->controls.GetPlayerInput(id);

		XYPair<double> velocity;
		if(!character->IsMovementLockedFromAnAttack())
		{


			double MULTIPLIER = 1.2;
			double SPEED_BASE = 50;
			auto player = character->GetPlayer();
			auto initialVelocity = controls->GetMoveVector();
			auto speed = double( (app->players.GetPlayer(1).SPD() + SPEED_BASE) * MULTIPLIER) / app->TPS;


			initialVelocity.x *= speed;
			initialVelocity.y *= speed;
			velocity.x += initialVelocity.x;
			velocity.y += initialVelocity.y;
			character->AddStepsToAnimation( speed*1.25 / app->TPS);
			

		}

		auto forcedVelocity = character->GetForcedVelocity();
		velocity.x += forcedVelocity.x;
		velocity.y += forcedVelocity.y;
		character->SetForcedVelocity(0,0);
		character->SetVelocity(velocity.x, velocity.y);


		/* Respond to actions */
		if(controls->buttons[USE_MAIN_ITEM].IsPressed())
		{
			Player &player = character->GetPlayer();
			int attackIndex = player.characterType->mainAttack.typeIndex;
			auto attackType = &app->data.attackTypes[attackIndex];

			CreateNewAttack(character, player.STR(), attackType);
		}
		if(controls->buttons[USE_SEC_ITEM].IsPressed())
		{
			auto action = actionBar.nodes[actionBar.selectedNode].action;
			//printf("Performing action %d:%d\n", action.actionType, action.typeIndex);
			if(action.actionType == ATTACK_ACTION_TYPE)
			{
				Player &player = character->GetPlayer();
				int attackIndex = action.typeIndex;
				auto attackType = &app->data.attackTypes[attackIndex];
				CreateNewAttack(character, player.STR(), attackType);
			}
			else if(action.actionType == TRIPLE_SHOT_ACTION)
			{

			}
			else if(action.actionType == ITEM_ACTION_TYPE)
			{
				
			}
		}
		if(controls->buttons[INC_SECONDARY].IsPressed())
		{
			actionBar.selectedNode++;
			if(actionBar.selectedNode >= (int)actionBar.nodes.size())
			{
				actionBar.selectedNode = 0;
			}
		}
		if(controls->buttons[DEC_SECONDARY].IsPressed())
		{
			actionBar.selectedNode--;
			if(actionBar.selectedNode < 0)
			{
				if(!actionBar.nodes.empty()) actionBar.selectedNode = (int)actionBar.nodes.size()-1;
				else actionBar.selectedNode = 0;
			}
		}
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
						//character->SetWasTileBlocked(true);
						velocity.y = tileBox.Top() - hitbox.Bottom();
						if(velocity.y>0) character->SetYVelocity( velocity.y );
						else character->SetYVelocity(0);
					}
					if(velocity.y<0 && tileBox.IsHorizontalLineIntersecting(hitbox.x+1, hitbox.X2()-1, hitbox.Top()) ) //Going Up
					{
						//character->SetWasTileBlocked(true);
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
						//character->SetWasTileBlocked(true);
						velocity.x = tileBox.Left() - hitbox.Right();
						if( velocity.x>0 ) character->SetXVelocity( velocity.x );
						else character->SetXVelocity(0);
					}
					if(velocity.x<0 && tileBox.IsVerticalLineIntersecting(hitbox.Left(), hitbox.y+1, hitbox.Y2()-1) ) //Going Left
					{
						//character->SetWasTileBlocked(true);
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

			unsigned int id = objectField.GetNextCharacterReferenceID();
			PlayerCharacter *newPC = new PlayerCharacter(id, startPositions[startPos], player);
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

		if(!(*mon)->IsStunTimeLeft())
		{
			if((*mon)->IsAbleToThink())
			{
				int monsterAi = (*mon)->GetAiType();
				if(monsterAi == BASIC_MONSTER_AI)
				{
					//Rotation
					auto facing = (*mon)->GetFacing();
					XYPair<double> position = (*mon)->GetPosition();
					auto playerID = GetPositionOfClosestPlayer(position);
					if(playerID != -1)
					{
						(*mon)->LookAt(position);
					} //else no player found

					//Movement
					(*mon)->SetVelocityToForward();
					(*mon)->AddStepSpeedToAnimation();

					//Reset instead of preventing rotation at all so that movement isn't necessarily affected
					if( (*mon)->IsRotationLockedFromAnAttack())
					{
						(*mon)->SetFacingAngle(facing);
					}
				}


				(*mon)->ResetThinkCooldown();
			}
		}
		else
		{
			(*mon)->SetVelocity(0,0);
			(*mon)->DecreaseStunTime(app->dt);
		}

		if( (*mon)->IsMovementLockedFromAnAttack())
		{
			(*mon)->SetVelocity(0,0);
		}
		auto forcedVelocity = (*mon)->GetForcedVelocity();
		(*mon)->AddForcedVelocityToVelocity();

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
					unsigned int id = objectField.GetNextCharacterReferenceID();
					Monster *newMonster = new Monster(id, spawnLocation, DIR_DOWN, *monsterType);
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
						unsigned int id = objectField.GetNextCharacterReferenceID();
						Monster *newMonster = new Monster(id, spawnLocation, DIR_DOWN, *monsterType);
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

void TestGameScreen::RebuildActionBar()
{
	actionBar.nodes.clear();
	auto player = app->players.GetPlayer(1);
	for(auto i = player.characterType->secondaryInherentActions.begin();
		i != player.characterType->secondaryInherentActions.end(); i++)
	{
		ActionBarNode node;
		node.action = (*i);
		actionBar.nodes.push_back(node);
	}
	for(auto i = player.inventory.begin(); i != player.inventory.end(); i++)
	{
		//TODO: add inventory to actionBar as ITEM_TYPE nodes
	}

}

void TestGameScreen::CreateNewAttack(GameCharacter *attacker, double attackerPower, AttackType *type)
{
	printf("ATTACK!\n");
	
	//position of attack
	auto position = attacker->GetHitbox().Center();

	//get attack direction
	double angle = 0 - attacker->GetFacing();

	//TODO: adjust position based on attack path type



	if(type->PATH_TYPE == PATH_FORWARD)
	{
		//move forward a little
	}
	else if(type->PATH_TYPE == PATH_AROUND_CHARACTER)
	{
		//set to a position so that it moves the character across their front
		auto attackerHitbox = attacker->GetHitbox();
		
		position.x += attackerHitbox.w * cos(angle);
		position.y += attackerHitbox.h * sin(angle);

	}
	else if(type->PATH_TYPE == PATH_NON_MOVING_LINE)
	{

		//Find further position not colliding with anything
	}

	
	//calculate damage and speed
	double speed = type->SPEED_MULTIPLIER * app->TPS;
	double damage = type->DAMAGE_MULTIPLIER * attackerPower;
	


	//get animation (if there is one)
	SpriteSheetAnimation *animation = NULL;

	if(type->ANIMATION_ON_PROJECTILE_INDEX != 0 && 
		(int)app->data.spriteSheetAnimationTypes.size() > type->ANIMATION_ON_PROJECTILE_INDEX)
	{
		animation = new SpriteSheetAnimation(&app->data.spriteSheetAnimationTypes[type->ANIMATION_ON_PROJECTILE_INDEX]);
	}

	//get bitmap
	std::shared_ptr<Bitmap> bitmap = NULL;
	if(animation)
	{
		bitmap = animation->type->bitmap;
	}



	//Adjust
	position.x -= type->SIZE/2;
	position.y -= type->SIZE/2;
	
	auto attackerId = attacker->GetID();

	Attack *newAttack = new Attack(attackerId, position, type->SIZE, angle, type, damage, speed, bitmap, 
		animation);
	attacksInView.push_back(newAttack);

	if(attacker)
	{
		if(type->ATTRIBUTES & PREVENTS_MOVEMENT) attacker->LockMovementFromAnAttack();
		if(type->ATTRIBUTES & PREVENTS_ROTATING) attacker->LockRotationFromAnAttack();
	}


	int attackerAnimationIndex = newAttack->type->ANIMATION_ON_ATTACKER_INDEX;
	if(attackerAnimationIndex != NO_ANIMATION && (int)app->data.spriteSheetAnimationTypes.size() > attackerAnimationIndex)
	{
		//auto position = attacker->GetHitbox().Center();
		XYPair<double> position;
		auto animation = (&app->data.spriteSheetAnimationTypes[attackerAnimationIndex]);

		AddAnimationToScreen(position, angle, animation, attacker);
	}
}

void TestGameScreen::UpdateAnimations()
{
	auto floorAnimation = animationsOnFloor.begin();
	while( floorAnimation != animationsOnFloor.end())
	{
		floorAnimation->animation.AddTimeToAnimation(app->dt);
		if(floorAnimation->animation.isDone)
		{
			floorAnimation = animationsOnFloor.erase(floorAnimation);
		}
		else floorAnimation++;
	}

	auto skyAnimation = animationsInSky.begin();
	while( skyAnimation != animationsInSky.end())
	{
		skyAnimation->animation.AddTimeToAnimation(app->dt);
		if(skyAnimation->animation.isDone)
		{
			skyAnimation = animationsInSky.erase(skyAnimation);
		}
		else skyAnimation++;
	}
}

void TestGameScreen::AddAnimationToScreen(XYPair<double> position, double angle, SpriteSheetAnimationType *type, GameCharacter *character)
{

	if(type)
	{
		auto characterId = -1;
		if(character)
		{
			characterId = character->GetID();
		}

		if(type->IS_ON_FLOOR_LAYER)
		{	
			animationsOnFloor.push_back(
				AnimationOnGameCharacter(type, angle, characterId, position));
		}
		else
		{
			animationsInSky.push_back(
				AnimationOnGameCharacter(type, angle, characterId, position));
		}
	}
}

void TestGameScreen::UpdateAttacks()
{
	for(auto attackIter = attacksInView.begin(); attackIter != attacksInView.end(); /* attackIter++ */)
	{
		/* UPDATE */
		auto attack = (*attackIter);

		auto attributes = attack->type->ATTRIBUTES;
		auto attackerID = attack->attackerId;
		GameCharacter *attacker = objectField.GetCharacterFromID(attackerID);
		if(attack->animation)	attack->animation->AddTimeToAnimation(app->dt);
		attack->DecrementTimeLeft();

		/* COLLISION */
		//test for collision with other attacks, game characters that aren't the attacker and walls in that order
		for(auto otherAttack = attacksInView.begin(); otherAttack != attacksInView.end(); otherAttack++)
		{
			auto other = (*otherAttack);
			//If it hits another attack, that attack should also stop existing as necessary
			if(other != attack && attack->hitbox.IsOverlapping(other->hitbox))
			{
				if(attributes & DESTROYED_ON_HIT) attack->ZeroOutTimeLeft();
				if(other->type->ATTRIBUTES & DESTROYED_ON_HIT) other->ZeroOutTimeLeft();
			}
		}

		//compare position with monsters and characters nearby
		auto tilesOverlapped = objectField.GetTilesInRange(attack->hitbox);

		const size_t ARBITRARY_REASONABLE_MAX_CHARACTERS_OVERLAPPING = 12;
		std::vector< Monster *> monsters;
		std::vector< PlayerCharacter *> playerCharacters;

		monsters.reserve(ARBITRARY_REASONABLE_MAX_CHARACTERS_OVERLAPPING);
		playerCharacters.reserve(app->players.MAX_PLAYERS);

		for(auto tileIter = tilesOverlapped.begin(); tileIter != tilesOverlapped.end(); tileIter++)
		{
			auto monstersInCell = objectField.collisionGrid[tileIter->x][tileIter->y].monstersOverlappingCell;
			for(auto monsterIter = monstersInCell.begin(); monsterIter != monstersInCell.end(); monsterIter++)
			{
				if(std::find(monsters.begin(), monsters.end(), (*monsterIter)) == monsters.end())
				{
					monsters.push_back((*monsterIter));
				}
			}

			auto playersInCell = objectField.collisionGrid[tileIter->x][tileIter->y].playersOverlappingCell;
			for(auto playerCharacterIter = playersInCell.begin(); playerCharacterIter != playersInCell.end(); playerCharacterIter++)
			{
				if(std::find(playerCharacters.begin(), playerCharacters.end(), (*playerCharacterIter)) != playerCharacters.end())
				{
					playerCharacters.push_back((*playerCharacterIter));
				}
			}
		}

		//Has a monster been hit
		for(auto monster = monsters.begin(); monster != monsters.end(); monster++)
		{
			if( (*monster) != attacker) //stop hitting yourself
			{
				auto monsterHitbox = (*monster)->GetHitbox();
				if(attack->hitbox.IsOverlapping(monsterHitbox))
				{
					//damage the monster 
					(*monster)->DecreaseHealth(attack->damage);
					if(attack->type->ANIMATION_ON_ATTACKED_INDEX != NO_ANIMATION)
					{
						SpriteSheetAnimationType *type = &(app->data.spriteSheetAnimationTypes[attack->type->ANIMATION_ON_ATTACKED_INDEX]);
						
						auto position = (*monster)->GetHitbox().Center();
						position.x -= attack->hitbox.Center().x;
						position.y -= attack->hitbox.Center().y;						
						AddAnimationToScreen(position, attack->facingAngle, type, (*monster));
					}

					//if it should knock back
					if(attributes & KNOCKS_BACK)
					{
						auto attackVelocity = attack->GetVelocity();
						(*monster)->SetForcedVelocity(attackVelocity.x, attackVelocity.y);
					}
					//if it should stun
					if(attributes & WILL_STUN)
					{
						if(!(*monster)->IsStunTimeLeft())
						{
							(*monster)->SetStunTime(1.0);
						}
					}
					//if attack should be destroyed
					if(attributes & DESTROYED_ON_HIT)
					{
						attack->ZeroOutTimeLeft();
					}
				}
				//else it didn't hit, continue
			}
		}

		//Has a player been hit
		for(auto player = playerCharacters.begin(); player != playerCharacters.end(); player++)
		{
			if( (*player)!= attacker)
			{
				auto playerHitbox = (*player)->GetHitbox();
				if(attack->hitbox.IsOverlapping(playerHitbox))
				{
					//damage the player
					if(attack->type->ANIMATION_ON_ATTACKED_INDEX != NO_ANIMATION)
					{
						SpriteSheetAnimationType *type = &(app->data.spriteSheetAnimationTypes[attack->type->ANIMATION_ON_ATTACKED_INDEX]);
						
						auto position = playerHitbox.Center();
						position.x -= attack->hitbox.Center().x;
						position.y -= attack->hitbox.Center().y;						
						AddAnimationToScreen(position, attack->facingAngle, type, (*player));
					}

					//if it should knock back
					if(attributes & KNOCKS_BACK)
					{
						auto attackVelocity = attack->GetVelocity();
						(*player)->SetForcedVelocity(attackVelocity.x, attackVelocity.y);
					}
					//if it should stun
					if(attributes & WILL_STUN)
					{
						if(!(*player)->IsStunTimeLeft())
						{
							(*player)->SetStunTime(1.0);
						}
					}
					//if attack should be destroyed
					if(attributes & DESTROYED_ON_HIT)
					{
						attack->ZeroOutTimeLeft();
					}
				}
			}
		}


		//Check if it's hit a wall
		//non-moving line attacks are hitting a wall by definition
		if(attack->type->PATH_TYPE != PATH_NON_MOVING_LINE)
		{
			for(auto tileXY = tilesOverlapped.begin(); tileXY != tilesOverlapped.end(); tileXY++)
			{
				if(map.IsThereAWallAt(tileXY->x, tileXY->y))
				{
					//it hit a wall so we destroy it
					attack->ZeroOutTimeLeft();
				}
				//else continue
			}
		}


		//attack is melee
		if(attributes & IS_MELEE)
		{
			if(attacker) //move with the player
			{
				//add velocity to melee
				auto attackersVelocity = attacker->GetVelocity();
				
				attack->lastPosition.x += attackersVelocity.x;
				attack->lastPosition.y += attackersVelocity.y;
				attack->hitbox.x += attackersVelocity.x;
				attack->hitbox.y += attackersVelocity.y;
			}
			else //attacker couldn't be found on the map
			{
				//delete attack because it's melee it relies on the attacker being exist
				attack->ZeroOutTimeLeft();
			}
		}

		//moves in attackers facing direction
		if(attributes & MOVES_IN_ATTACKERS_FACING_DIRECTION)
		{
			if(attacker)
			{
				auto facing = attacker->GetFacing();
				attack->SetFacing(-facing);
			}
			//else attack doesn't exist, attack isn't necessarily destroyed though
		}

		/* MOVE */
		if(attack->type->PATH_TYPE == PATH_FORWARD)
		{
			//move forward
			double distance = (attack->type->SPEED_MULTIPLIER * attack->speed/ app->TPS);
			XYPair<double> velocity;
			velocity.x = distance * cos(attack->facingAngle);
			velocity.y = distance * sin(attack->facingAngle);

			attack->lastPosition.x = attack->hitbox.x;
			attack->lastPosition.y = attack->hitbox.y;

			attack->hitbox.x += velocity.x;
			attack->hitbox.y += velocity.y;
		}
		else if(attack->type->PATH_TYPE == PATH_AROUND_CHARACTER)
		{
			if(attacker)
			{
				double distance = (attack->type->SPEED_MULTIPLIER * attack->speed/ app->TPS);
				auto attackerCenter = attacker->GetHitbox().Center();
				auto attackCenter = attack->hitbox.Center();
				
				//distance to attack
				
				XYPair<double> difference(attackCenter.x-attackerCenter.x, attackCenter.y-attackerCenter.y);
				double angleToAttack = atan2(difference.y, difference.x);

				//this probably works
				XYPair<double> newPosition( attackCenter.x + distance*cos(angleToAttack),
					attackCenter.y - distance*sin(angleToAttack));


			}
		} //else line which doesn't move

		if(attack->timeLeft != 0)
		{
			attackIter++;
		}
		else
		{
			attackIter = attacksInView.erase(attackIter);
			if(attacker)
			{
				if(attributes & PREVENTS_MOVEMENT) attacker->UnlockMovementFromAnAttack();
				if(attributes & PREVENTS_ROTATING) attacker->UnlockRotationFromAnAttack();
			}
		}
	} //for each attack
}
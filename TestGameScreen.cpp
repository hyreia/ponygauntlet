#include "TestGameScreen.hpp"
#include <allegro5\allegro.h>
#include "GauntletApp.hpp"
#include "GameCharacter.hpp"
#include "GameEntity.hpp"
#include <allegro5/allegro_primitives.h>
#include "Tile.hpp"

#include "Bitmap.hpp"
using namespace gauntlet; 

TestGameScreen::TestGameScreen()
{
	temp = Bitmap::Load("WizardUpdate.png");
	character = new GameCharacter(XYPair<double>(100, 500), 16, temp, 
		Rect<double>(0, 96*4, 96, 96), DIR_UP);
	character->UpdateAnimation();

	entity = new GameCharacter(XYPair<double>(600, 500), 16, temp, 
		Rect<double>(0, 96*4, 96, 96), DIR_LEFT);
	entity->UpdateAnimation();

	map.Initialize("graveyard.tmx");
	objectField.InitializeToSize(map.GetTileWidthInTiles(), map.GetTileHeightInTiles());
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
	app->allegro.ClearBufferToColor(64,32,32);

	map.DrawGroundLayerRegion(0, 0, app->allegro.BUFFER_WIDTH,
		app->allegro.BUFFER_HEIGHT, 0, 0, 0);

	map.DrawWallLayerRegion(0, 0, app->allegro.BUFFER_WIDTH,
		app->allegro.BUFFER_HEIGHT, 0, 0, 0);




	auto bitmap = character->GetSourceBitmap();
	auto imageBox = character->GetImageBoxAtDeltaTime(alpha);
	auto imageSource = character->ImageSource();
	auto imageFlags = character->ImageDrawingFlags();
	bitmap->Draw(imageSource.x, imageSource.y, imageBox.w, imageBox.h, imageBox.x, imageBox.y, imageFlags);
	
	//auto hitbox = character->GetHitbox();
	//al_draw_circle(hitbox.cx,hitbox.cy, hitbox.radius, al_map_rgb(255,0,0), 3);

	bitmap = entity->GetSourceBitmap();
	imageBox = entity->GetImageBox();
	imageSource = entity->ImageSource();
	imageFlags = entity->ImageDrawingFlags();
	bitmap->Draw(imageSource.x, imageSource.y, imageBox.w, imageBox.h, imageBox.x, imageBox.y, imageFlags);
	//hitbox = entity->GetHitbox();
	//al_draw_circle(hitbox.cx,hitbox.cy, hitbox.radius, al_map_rgb(255,0,0), 3);

	map.DrawCeilingLayerRegion(0, 0, app->allegro.BUFFER_WIDTH,
		app->allegro.BUFFER_HEIGHT, 0, 0, 0);

} 
void TestGameScreen::Update()
{
	UpdateCharactersFromPlayerInput();
	/* Perform collision detection for player characters */
	PerformCollisionsForPlayerCharacters();
	/* Update monster logic */
	/* Perform collision detection for monsters */
	/* Update attacks (move, animate, collide) */
	/* Update Spawners and ObjectField objects */
	/* Update debris and lose dungeon animations, etc */
	/* Update any gates */
	/* Update action bar */
}

void TestGameScreen::UpdateCharactersFromPlayerInput()
{
	/* Directional facing */
	if(!character->IsFacingAngleLocked())
	{
		auto charPos = character->GetPosition();
		auto angleToMouse = app->controls.GetAngleToMouse(charPos.x, charPos.y);
		character->SetFacingAngle(angleToMouse);
		character->UpdateAnimation();
	}

	auto controls = app->controls.GetPlayerInput();

	/* Update desired velocity */
	if(!character->isMovementLocked())
	{
		double speedBase = 5.0;
		auto initialVelocity = controls->GetMoveVector();
		auto speed = (app->players.GetPlayer(1).SPD() * app->dt) + speedBase;
		printf("Speed %f\n", speed);
		initialVelocity.x *= speed;
		initialVelocity.y *= speed;

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

void TestGameScreen::PerformCollisionsForPlayerCharacters()
{
	//projected hitbox
	auto nextHitbox = character->GetNextHitbox();
	auto tilesInRange = objectField.GetTilesInRange(nextHitbox.Square());
	
	for(auto tileIter = tilesInRange.begin(); tileIter != tilesInRange.end(); tileIter++)
	{
		XYPair<int> tile(tileIter->x, tileIter->y);

		//Keep players out of nearby monsters and other player characters
		auto monstersOverlappingCell = objectField.collisionGrid[tile.x][tile.y].monstersOverlappingCell;
		for(auto mon = monstersOverlappingCell.begin(); mon != monstersOverlappingCell.end(); mon++)
		{

		}

		//Keep players out of solid tiles
		auto tileRect = Tile::TileToRect(tile.x, tile.y);

		if(IsRectIntersectingCircle(nextHitbox.cx, nextHitbox.cy, nextHitbox.radius, tileRect))
		if(map.IsThereAFloorAt(tile.x,tile.y))
		{
			if(map.IsThereAWallAt(tile.x, tile.y) &&
				character->GetAltitude() < map.GetHeightOfWall(tile.x,tile.y))
			{
				//Find closest point on tile side to circle
				XYPair<double> closestPoint(nextHitbox.cx, nextHitbox.cy);

				if(closestPoint.x > tileRect.Right()) closestPoint.x = tileRect.Right();
				if(closestPoint.x < tileRect.Left()) closestPoint.x = tileRect.Left();
				if(closestPoint.y > tileRect.Bottom()) closestPoint.y = tileRect.Bottom();
				if(closestPoint.y < tileRect.Top()) closestPoint.y = tileRect.Top();

				XYPair<double> differenceFromCenter(nextHitbox.cx-closestPoint.x, nextHitbox.cy-closestPoint.y);

				//finding angle from center of circle to closest point, then rotate by 180
				auto radian = atan2(differenceFromCenter.y, differenceFromCenter.x);
				radian -= ALLEGRO_PI;
				if(radian < 0) radian += 2*ALLEGRO_PI;

				//radian now holds the direction we need to move the circle
				//the length we need to move is the difference of the radius - the rectangle's closest point to the center
				double lengthFromCenter = hypot(differenceFromCenter.x, differenceFromCenter.y);

				//distance is distance we would need to move in our next position
				double distance = lengthFromCenter -  nextHitbox.radius;

				nextHitbox.cx += distance * cos(radian);
				nextHitbox.cy += distance * sin(radian);

				auto currentPosition = character->GetPosition();
				XYPair<double> newVelocity(nextHitbox.cx - currentPosition.x, nextHitbox.cy - currentPosition.y);

				character->SetVelocity(newVelocity.x, newVelocity.y);
			}
			else //check what's on the tile
			{

			}
		}
	
	}
		//Set last position to current position
		auto position = character->GetPosition();
		character->SetLastPosition(position);
		//Add adjusted velocity to current position
		auto velocity = character->GetVelocity();
		character->AddToPosition(velocity.x, velocity.y);
	



	
}
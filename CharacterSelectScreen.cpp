#include "CharacterSelectScreen.hpp"
#include "TestGameScreen.hpp"
#include "Bitmap.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "GauntletApp.hpp"
#include "Players.hpp"

using namespace gauntlet;

CharacterSelectScreen::CharacterSelectScreen():
characterSelected(-1),
BUTTON_SIZE(200, 30)
{
	
	al_fixtan(9);
	background = Bitmap::Load("Character Select Backdrop.png");
	archerBitmask = Bitmap::Load("Archer Bitmask.png");
	warriorBitmask = Bitmap::Load("Warrior Bitmask.png");
	wizardBitmask = Bitmap::Load("Wizard Bitmask.png");
	valkyrieBitmask = Bitmap::Load("Valkyrie Bitmask.png");

	backdropPos.x = app->allegro.BUFFER_WIDTH/2 - background->Width()/2;
	backdropPos.y = app->allegro.BUFFER_HEIGHT/2 - background->Height()/2;

	descriptionPos.x = 100; descriptionPos.y = 500;

	startButton.x = backdropPos.x + background->Width();
	startButton.y = 100;
	startButton.w = BUTTON_SIZE.x;
	startButton.h = BUTTON_SIZE.y;

	backButton.x = backdropPos.x - BUTTON_SIZE.x;
	backButton.y = 100;
	backButton.w = BUTTON_SIZE.x;
	backButton.h = BUTTON_SIZE.y;
}

CharacterSelectScreen::~CharacterSelectScreen()
{}

void CharacterSelectScreen::HandleInput(ALLEGRO_EVENT &ev)
{
	switch(ev.type)
	{
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		app->allegro.TranslateMousePositionToBufferPosition(ev.mouse.x, ev.mouse.y);
		if(ev.mouse.x > backdropPos.x && ev.mouse.y > backdropPos.y &&
			ev.mouse.x < backdropPos.x+background->Width() && 
			ev.mouse.y < backdropPos.y+background->Height())
		{
			ev.mouse.x -= backdropPos.x;
			ev.mouse.y -= backdropPos.y;

			ALLEGRO_COLOR archerMask = al_get_pixel(archerBitmask->bitmap, ev.mouse.x, ev.mouse.y);
			ALLEGRO_COLOR warriorMask = al_get_pixel(warriorBitmask->bitmap, ev.mouse.x, ev.mouse.y);
			ALLEGRO_COLOR wizardMask = al_get_pixel(wizardBitmask->bitmap, ev.mouse.x, ev.mouse.y);
			ALLEGRO_COLOR valkyrieMask = al_get_pixel(valkyrieBitmask->bitmap, ev.mouse.x, ev.mouse.y);
			if(archerMask.r==0 && archerMask.g==0 && archerMask.b==0)
			{
				Player *player = &app->players.GetPlayer(1);
				player->characterType = &app->data.playerCharacterTypes[PC_ARCHER];
				player->isPlaying = true;
				player->SetNameToCharacterName();
				player->SetStatsToDefaultForCharacterType();
				characterSelected = PC_ARCHER;
			}
			else if(warriorMask.r==0 && warriorMask.g==0 && warriorMask.b==0)
			{
				Player *player = &app->players.GetPlayer(1);
				player->characterType = &app->data.playerCharacterTypes[PC_WARRIOR];
				player->isPlaying = true;
				player->SetNameToCharacterName();
				player->SetStatsToDefaultForCharacterType();
				characterSelected = PC_WARRIOR;
			}
			else if(wizardMask.r==0 && wizardMask.g==0 && wizardMask.b==0)
			{
				Player *player = &app->players.GetPlayer(1);
				player->characterType = &app->data.playerCharacterTypes[PC_WIZARD];
				player->isPlaying = true;
				player->SetNameToCharacterName();
				player->SetStatsToDefaultForCharacterType();
				characterSelected = PC_WIZARD;
			}
			else if(valkyrieMask.r==0 && valkyrieMask.g==0 && valkyrieMask.b==0)
			{
				Player *player = &app->players.GetPlayer(1);
				player->characterType = &app->data.playerCharacterTypes[PC_VALKYRIE];
				player->isPlaying = true;
				player->SetNameToCharacterName();
				player->SetStatsToDefaultForCharacterType();
				characterSelected = PC_VALKYRIE;
			}
			else
			{	//Click was inside of image
				printf("CLICK\n");
			}
		}
		else if(ev.mouse.x > backButton.x && ev.mouse.y > backButton.y &&
			ev.mouse.x < backButton.X2() && ev.mouse.y < backButton.Y2())
		{
			//
		}
		else if(characterSelected>-1 && ev.mouse.x > startButton.x && ev.mouse.y > startButton.y &&
			ev.mouse.x < startButton.X2() && ev.mouse.y < startButton.Y2())
		{
			app->ChangeToNewScreen(new TestGameScreen());
		}
		break;
	case ALLEGRO_EVENT_DISPLAY_CLOSE:
		app->SetIsTimeToQuit();
		break;
	default:
		break;
	}
}

void CharacterSelectScreen::Render(double alpha)
{
	app->allegro.ClearBufferToColor(255,255,255);
	background->Draw(backdropPos.x, backdropPos.y);
	Font &font = app->data.fonts[MESSAGE_FONT];
	int lineHeightHalf = font.LineHeight() / 2;
	
	al_draw_filled_rectangle(backButton.x, backButton.y, backButton.x+backButton.w, 
		backButton.y+backButton.h, al_map_rgb(64,64,64));
	al_draw_rectangle(backButton.x, backButton.y, backButton.x+backButton.w, 
		backButton.y+backButton.h, al_map_rgb(255,255,0),2);
	font.DrawTxt(backButton.x + BUTTON_SIZE.x/2, backButton.y + BUTTON_SIZE.y/2 - lineHeightHalf, 
		TEXT_ALIGN_CENTER, 1.0, 1.0, 1.0, 1.0, "Back");

	if(characterSelected>-1)
	{
		Player *player = &app->players.GetPlayer(1);
		font.DrawTxtf(descriptionPos.x, descriptionPos.y,0,0,0,0,1.0,"%s", player->characterType->defaultName.c_str());
		font.DrawTxtf(descriptionPos.x, descriptionPos.y+20,0,0,0,0,1.0,"STR %d", player->strength);
		font.DrawTxtf(descriptionPos.x, descriptionPos.y+40,0,0,0,0,1.0,"MAG %d", player->magic);
		font.DrawTxtf(descriptionPos.x, descriptionPos.y+60,0,0,0,0,1.0,"SPD %d", player->speed);
		font.DrawTxtf(descriptionPos.x, descriptionPos.y+80,0,0,0,0,1.0,"ARM %d", player->armor);
		font.DrawTxtf(descriptionPos.x, descriptionPos.y+120,0,0,0,0,1.0, "HEALTH %.0f", player->health);

		std::string description = player->characterType->personalityDescription;
		int paragraphEnd = font.DrawTextWrapped(descriptionPos.x, descriptionPos.y+160, 400, description.c_str(), 0,0,0,1.0,0,true);
		paragraphEnd+= 20;

		description = player->characterType->abilitiesDescription;
		font.DrawTextWrapped(descriptionPos.x, paragraphEnd, 400, description.c_str(), 0,0,0,1.0,0,true);

		al_draw_filled_rectangle(startButton.x, startButton.y, startButton.x+startButton.w, 
			startButton.y+startButton.h, al_map_rgb(64,64,64));
		al_draw_rectangle(startButton.x, startButton.y, startButton.x+startButton.w, 
			startButton.y+startButton.h, al_map_rgb(255,255,0),2);
		font.DrawTxt(startButton.x + BUTTON_SIZE.x/2, startButton.y + BUTTON_SIZE.y/2 - lineHeightHalf, TEXT_ALIGN_CENTER, 1.0, 1.0, 1.0, 1.0, "Start");
	}

}

void CharacterSelectScreen::Update()
{

}


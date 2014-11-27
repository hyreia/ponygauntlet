#include "GauntletApp.hpp"
#include "GameScreen.hpp"

#include "TestGameScreen.hpp"

using namespace gauntlet;

std::unique_ptr<GauntletApp> app;

const char *GauntletApp::title = "Pony Gauntlet";
const int GauntletApp::TPS = 20;
const double GauntletApp::dt = 1.0/TPS;

GauntletApp::GauntletApp():
isTimeToQuit(false)
{}

GauntletApp::~GauntletApp()
{}

void GauntletApp::Initialize()
{
	if(allegro.Initialize())
	{
		//Initialize all other subsystems
		sound.Initialize();
		controls.Initialize();
		data.Initialize();
	}
	else isTimeToQuit = true;
}

void GauntletApp::Destroy()
{
	controls.Destroy();
	sound.Destroy();
	data.Destroy();
	allegro.Destroy();
}

void GauntletApp::Run()
{
	if(!isTimeToQuit)
	{
		StartFirstScreen();
		al_start_timer(allegro.timer);
		startTimeOffset = al_get_time();
		MainLoop();
	}
	//else immediately return
}

void GauntletApp::SetIsTimeToQuit()
{
	isTimeToQuit = true;
}

void GauntletApp::StartFirstScreen()
{
	//TODO: this
	Player *player = &players.GetPlayer(1);
	player->characterType = &data.playerCharacterTypes[PC_ARCHER];
	player->SetNameToCharacterName();
	player->SetStatsToDefaultForCharacterType();
	
	currentScreen = new TestGameScreen();
}

void GauntletApp::MainLoop()
{
	do
	{
		HandleEvents();
		Output();
	} while (!isTimeToQuit);
}

void GauntletApp::ChangeToNewScreen(GameScreen *newScreen)
{
	if(currentScreen)
	{
		delete currentScreen;
		currentScreen = newScreen;
	}
}

void GauntletApp::HandleEvents()
{
	ALLEGRO_EVENT ev;
	while(al_get_next_event(allegro.event_queue, &ev))
	{
		switch(ev.type)
		{
			case ALLEGRO_EVENT_TIMER:
				gameTime = ev.timer.count * dt;
				Update();
				break;
				//TODO: consider handle enter/leave display
			default:
				if(currentScreen) currentScreen->HandleInput(ev);
				else isTimeToQuit = true;
				break;
		}
	}
}

void GauntletApp::Output()
{
	const double alpha = (al_get_time() - startTimeOffset - gameTime) / dt;
	allegro.SetBufferAsDrawTarget();
	currentScreen->Render(alpha);
	allegro.DrawBufferToDisplay();
}

void GauntletApp::Update()
{
	if(currentScreen)
	{
		currentScreen->Update();
	}
	else isTimeToQuit = true;
	sound.sfx.PlayQueuedSounds();
}







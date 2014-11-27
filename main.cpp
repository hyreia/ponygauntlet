#include "GauntletApp.hpp"

using namespace gauntlet;

int main()
{
	app.reset(new GauntletApp());
	app->Initialize();
	app->Run();

	app->Destroy();
}
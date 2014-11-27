#include "Sound.hpp"
#include "ConfigFile.hpp"

using namespace gauntlet;

const char *Sound::soundInfoFile = "sound.ini";

Sound::Sound(){}
Sound::~Sound(){}

void Sound::Initialize()
{
	sfx.Initialize();
	music.Initialize();

	ConfigFile sound(soundInfoFile);
	if(!sound.IsOpen())
	{
		sound.Close();
		RecreateSoundIniFile();
		sound.Open(soundInfoFile);
	}

	sfx.SetVolume( sound.GetIntValue(NULL, "sfx volume") );
	music.SetVolume( sound.GetIntValue(NULL, "music volume") );
}

void Sound::Destroy()
{
	sfx.Destroy();
	music.Destroy();
}

void Sound::RecreateSoundIniFile()
{
	ConfigFile sound(soundInfoFile);
	sound.CreateEmpty();

	sound.SetValue(NULL, "sfx volume", 100);
	sound.SetValue(NULL, "music volume", 100);
	sound.Save();
}
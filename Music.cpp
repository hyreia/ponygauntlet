#include "Music.hpp"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace gauntlet;

Music::Music():
currentMusic(NULL),
filenameOfCurrentMusic(""),
volume(1.0f)
{
}

/* Shares folder with sfx to avoid excessive folders */
const char *Music::musicDirectory = "sound";

Music::~Music()
{
	if(currentMusic)
	{
		al_destroy_audio_stream(currentMusic);
	}
}

void Music::SetFile(std::string filename)
{
	if( filenameOfCurrentMusic != filename )
	{

		if(currentMusic)
		{
			al_destroy_audio_stream(currentMusic);
		}


		filenameOfCurrentMusic = filename;

		ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
		al_append_path_component(path, musicDirectory);

		const char *filePath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
		std::string filePathAndFile = filePath;
		filePathAndFile.append(filename);

		al_destroy_path(path);

		const size_t BUFFER_COUNT = 4;
		const unsigned int SAMPLE_SIZE = 1024*4;


		currentMusic = al_load_audio_stream(filePathAndFile.c_str(), BUFFER_COUNT, SAMPLE_SIZE);
		if(currentMusic)
		{
			al_attach_audio_stream_to_mixer(currentMusic, al_get_default_mixer());
			al_set_audio_stream_playmode(currentMusic, ALLEGRO_PLAYMODE_LOOP);
			al_set_audio_stream_playing(currentMusic, false);
		}
		else
		{
			//printf("COULDN'T PLAY %s\n", filename.c_str());
			printf("COULDN'T PLAY %s\n", filePathAndFile.c_str());
		}

	}
}

void Music::Play()
{
	if(currentMusic)
	{
		al_set_audio_stream_playing(currentMusic, true);
		al_set_audio_stream_gain(currentMusic, volume);
	}
}

bool Music::IsPlaying()
{
	return al_get_audio_stream_playing(currentMusic);
}

void Music::SetIsLooping(bool isLooping)
{
	this->isLooping = isLooping;
	if(isLooping)
	{
		if(currentMusic) al_set_audio_stream_playmode(currentMusic, ALLEGRO_PLAYMODE_LOOP);
	}
	else
	{
		if(currentMusic) al_set_audio_stream_playmode(currentMusic, ALLEGRO_PLAYMODE_ONCE);
	}
}


void Music::Pause(bool pause)
{
	if(currentMusic)
		al_set_audio_stream_playing(currentMusic, !pause);
}

void Music::SetVolume(int volume)
{
	double gain = (double)volume/100;
	if(currentMusic)
	{
		
		al_set_audio_stream_gain(currentMusic, gain);
	}

	this->volume = gain;
}

int Music::GetVolume()
{
	double gain = 0.0f;

	if(currentMusic)
		gain = al_get_audio_stream_gain(currentMusic);

	gain *= 100;

	int volume = static_cast<int>(gain);

	return volume;
}

std::string Music::GetFilename()
{
	return filenameOfCurrentMusic;
}

void Music::Initialize()
{
	//Do nothing
}

void Music::Destroy()
{
	al_destroy_audio_stream(currentMusic);
	currentMusic = NULL;
}
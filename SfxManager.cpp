#include "SfxManager.hpp"
#include <allegro5\allegro_audio.h>

using namespace gauntlet;

/* * * * * * * * * * * * * * * * * * * * *
 *  SfxManager class 
 * * * * * * * * * * * * * * * * * * * * *
*/

SfxManager::SfxManager():
volume(1.0f)
{} // Assumes allegro sound is already set up

SfxManager::~SfxManager(){}

void SfxManager::LoadSound(std::string name)
{
	std::shared_ptr<SoundEffect>newSound (new SoundEffect(name.c_str()));
	soundLibrary.insert( std::pair<std::string, std::shared_ptr<SoundEffect>>(name, newSound));
}

void SfxManager::LoadSound(std::string file, std::string internalName)
{
	std::shared_ptr<SoundEffect>newSound (new SoundEffect(file.c_str()));
	soundLibrary.insert( std::pair<std::string, std::shared_ptr<SoundEffect>>(internalName, newSound));
}

void SfxManager::FreeSound(std::string name)
{
	if(soundLibrary.count( name ) == 1)
	{
		soundLibrary.erase( name );
	}
}

void SfxManager::PlayASound(std::string soundName)
{
	if(soundLibrary.count(soundName) == 1)
	{
		soundLibrary[soundName]->Play(volume);
	} //else don't play anything
}

void SfxManager::QueueSoundToPlay(std::string soundName, std::string instanceName)
{
	QueueSoundToPlayLater(soundName, 0, instanceName);	
}

void SfxManager::QueueSoundToPlayLater(std::string soundName, int delayInFrames, std::string instanceName)
{		
	std::shared_ptr<SoundInstance>newSoundInstance(
		new SoundInstance(soundName, instanceName, delayInFrames) );
		
	queuedSounds.push( newSoundInstance );
}

void SfxManager::QueueSoundToPlayAfterOtherSound(std::string soundName, std::string playAfterThisInstance, int fps, std::string instanceName)
{
	//Using delay to store the fps for later 
	std::shared_ptr<SoundInstance>delayedSound( new SoundInstance( soundName, instanceName, fps) );

	//delayedSounds.insert( std::pair<std::string, std::shared_ptr<SoundInstance>>(playAfterThisInstance, delayedSound));

}

void SfxManager::PlayQueuedSounds()
{
	size_t soundsToPlay = queuedSounds.size();
	while(soundsToPlay != 0)
	{
		if(queuedSounds.front()->GetDelay() == 0)
		{
			std::string soundToPlay = queuedSounds.front()->GetSoundName();
			std::string instanceName = queuedSounds.front()->GetInstanceName();

			//If a sound is waiting for this instance
			if(delayedSounds.count( instanceName ) == 1)
			{
				auto delayedSound = delayedSounds[instanceName];
				delayedSounds.erase( instanceName );

				//FPS is stored in delayedSound's delay, get the name of the sound we're going to play
				//In order to figure out how long to play the sound
				int delay = 0;
				if(soundLibrary.count(soundToPlay) == 1)
				{
					
					double seconds = soundLibrary[soundToPlay]->Length();
					delay = static_cast<int>(seconds*delayedSound->GetDelay());
				}

				delayedSound->SetDelay(delay);
				queuedSounds.push( delayedSound );

				
			}
			//either way we're ready to play this

			if(soundLibrary.count(soundToPlay) == 1)
			{
				bool played = soundLibrary[soundToPlay]->Play(volume);
				if(!played) printf("Couldn't play %s\n", soundToPlay.c_str());
			}
			queuedSounds.pop();
		}
		else //sound isn't ready to be played
		{
			//remove from the queue
			auto delayedSound = queuedSounds.front();
			queuedSounds.pop();

			//decrement it and put it back on
			delayedSound->DecrementDelay();
			queuedSounds.push(delayedSound);
		}

		--soundsToPlay;
	}
}

// 100 for full volume, 0 for completely silent
void SfxManager::SetVolume(int vol)
{
	double gain = (double)vol/100;

	this->volume = gain;
}

// Returns current volume of music, 100 for full volume
int SfxManager::GetVolume()
{
	double gain = 0.0f;

	gain = volume;

	gain *= 100;

	return static_cast<int>(gain);
}

void SfxManager::StopAllSoundsAndClearSoundQueue()
{
	al_stop_samples();
	while(!queuedSounds.empty()) queuedSounds.pop();
}

void SfxManager::LoadSoundsFromList()
{
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(path, SoundEffect::soundDirectory);
	al_set_path_filename(path, "sounds.ini");
	
	const char *filePath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
	ALLEGRO_CONFIG *config = al_load_config_file(filePath);
	if(config)
	{
		ALLEGRO_CONFIG_ENTRY *iterator = NULL;

		//prime it
		const char *soundKey = al_get_first_config_entry(config, NULL, &iterator);
		while(soundKey)
		{
			std::string soundName = soundKey;
			const char *soundFilename = al_get_config_value(config, NULL, soundKey);
			
			LoadSound(soundFilename, soundKey);

			soundKey = al_get_next_config_entry(&iterator);
		}
		al_destroy_config(config);
	}
	else
	{
		printf("No sounds.ini\n");
	}
}

void SfxManager::Initialize()
{
	LoadSoundsFromList();
}

void SfxManager::Destroy()
{
	for(auto soundPtr = soundLibrary.begin(); soundPtr != soundLibrary.end(); soundPtr++)
	{
		(soundPtr)->second.reset();
	}
	soundLibrary.clear();
}

/* * * * * * * * * * * * * * * * * * * * *
 *  SoundEffect class 
 * * * * * * * * * * * * * * * * * * * * *
*/
const char *SoundEffect::soundDirectory = "sounds";
const char *SoundEffect::soundExtension = ".ogg";

SoundEffect::SoundEffect(const char *filename):
sound(NULL)
{
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
		al_append_path_component(path, soundDirectory);
	const char *filePath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

	std::string filenameAndPath(filePath);
	filenameAndPath.append(filename);
	filenameAndPath.append(soundExtension);

	sound = al_load_sample(filenameAndPath.c_str());

	if(!sound) //check if there are multiples, all ending with _#
	{

		ALLEGRO_SAMPLE *temp_sample = NULL;
		int number = 1;

		do
		{
			
			std::string sound_name(filePath);
			sound_name.append(filename);
			sound_name.append("_");

			/* Because MSVC 2010 is silly there is no int overload for to_string()*/
			sound_name.append( std::to_string(static_cast<long long>(number)) );
			sound_name.append(soundExtension);

			temp_sample = al_load_sample(sound_name.c_str());
			if(temp_sample)
			{
				sounds.push_back(temp_sample);
			}
			number++;

		} while (temp_sample);
	}

	sounds.shrink_to_fit();

	if(!sound && sounds.empty())
	{
		//printf("No sounds found for %s\n", filename);
	}
}

SoundEffect::~SoundEffect()
{
	if(sound)
		al_destroy_sample(sound);
}

bool SoundEffect::Play(double volume, double pan, double speed)
{
	bool hasSoundEffectPlayed = true;

	if(sound)
		hasSoundEffectPlayed = al_play_sample(sound, volume, pan, speed, ALLEGRO_PLAYMODE_ONCE, NULL);
	else if(!sounds.empty())
	{
		//Pick a sound at random
		int index = rand()%sounds.size();
		ALLEGRO_SAMPLE *soundSelected = sounds[index];
		if(soundSelected)
		{
			hasSoundEffectPlayed = al_play_sample( soundSelected, volume, pan, speed, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}

	return hasSoundEffectPlayed;
}

double SoundEffect::Length()
{
	/* Length is raw length in bytes and frequency divides the byte giving us the raw length in seconds */
	if(sound)
		return al_get_sample_length(sound) / al_get_sample_frequency(sound);
	else return 0.0f;
}

/* * * * * * * * * * * * * * * * * * * * *
 * SoundInstance class
 * * * * * * * * * * * * * * * * * * * * *
*/
SoundInstance::SoundInstance(std::string _soundName, std::string nameOfInstance, int _delay):
soundName(_soundName), instanceName(nameOfInstance), delay(_delay)
{}

int SoundInstance::GetDelay()
{ return delay; }

void SoundInstance::SetDelay(int delay)
{
	this->delay = delay;
}

void SoundInstance::DecrementDelay()
{
	if(delay > 0) --delay;
}

std::string SoundInstance::GetSoundName()
{
	return soundName;
}

std::string SoundInstance::GetInstanceName()
{
	return instanceName;
}
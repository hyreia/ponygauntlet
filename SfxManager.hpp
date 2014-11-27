#ifndef SFX_MANAGER_HPP
#define SFX_MANAGER_HPP

#include <string>
#include <queue>
#include <unordered_map>
#include <memory>

struct ALLEGRO_SAMPLE;

namespace gauntlet
{

/* Sound files are loaded into memory and assigned a given name with LoadSound()

Sound are queued to play using these assigned names with QueueSoundToPlay(). 
Queued sounds are sent to GauntletApp which will request sounds as needed and play them.

Sound may be delayed by specifying a count until sounds are played. This will tick down
each time PlaySounds() is called. Extenally this should be synched and converted into
whatever frequency you want (fps * seconds is suggested)

Sound library is discarded upon destruction.

When a sound is queued to play, a SoundInstance will be created, which is composed of the name of
the sound, an optional name for this instance and an optional delay for this instance.

Not all sounds have to be queued to play. Queuing sounds is to either put in a delay or to
play a string of sounds when one finishes another should start. PlaySound() plays the sound immediately.
Although this technically means output (sound) is being output outside of Output() the overhead is
negligible with sound

Only the constructor will work without Allegro initialized, call Initialize after doing so
before any other Sound methods
*/

class GauntletApp;
class SoundEffect;
class SoundInstance;

class SfxManager
{
public:

	/* Sets default sound effect volume to 100% */
	SfxManager();
	/* Does nothing */
	~SfxManager();

	/* use for filename and internal name are identical (except for .ogg extension) */
	void LoadSound(std::string name);
	void LoadSound(std::string filename, std::string internalName);
	void FreeSound(std::string internalName);

	/* Loads sounds from sounds.ini into sound library */
	void Initialize();
	/* Frees the sound library*/
	void Destroy();

	void QueueSoundToPlay(std::string soundName, std::string instanceName = "");
	void QueueSoundToPlayLater(std::string soundName, int delayInFrames, std::string instanceName = "");
	void QueueSoundToPlayAfterOtherSound(std::string soundName, std::string playAfterThisInstance, int fps, std::string instanceName = "");
		
	/* Play sound immediately, shouldn't necessarily use this externally */
	void PlayASound(std::string soundName);
	/* Called at end of each logic tick to play sounds queued during that frame */
	void PlayQueuedSounds();

	/* 100 for full volume, 0 for completely silent */
	void SetVolume(int volume);

	/* Returns current volume of music, 100 for full volume*/
	int GetVolume();

	/* Useful for when screen transitioning*/
	void StopAllSoundsAndClearSoundQueue();

	/* Number of sound samples that can be played simultaneously */
	static const int NUM_OF_ALLEGRO_SAMPLES_RESERVED = 32;

private:

	void LoadSoundsFromList();

	//All sounds currently loaded in memory
	std::unordered_map< std::string, std::shared_ptr<SoundEffect> > soundLibrary;

	//All instances of sounds currently waiting to be played
	std::queue< std::shared_ptr<SoundInstance> > queuedSounds;

	//Sound instances waiting for other sound instanced to be played
	std::unordered_map< std::string, std::shared_ptr<SoundInstance> > delayedSounds;

	double volume;
};



class SoundEffect
{
public:
	SoundEffect(const char *filename);
	~SoundEffect();

	/* returns false if failed to play (meaning all reserved samples are being used right now) 
		If the sound failed to load and is null, it will return true anyway */
	bool Play(double volume = 1.0f, double pan = 0.0f, double speed = 1.0f);

	/* Returns how long sound will play when played, in seconds,
		does NOT work with multiple sound options which need a strict sound length anyway
	*/
	double Length();

	/* Directory path to sound files relative to executable file */
	static const char *soundDirectory;
	static const char *soundExtension;

private:
	ALLEGRO_SAMPLE *sound;
	std::vector<ALLEGRO_SAMPLE*> sounds;

};


/* Describes an instance of a given sound to be played. Used to queue sounds to play sounds later or play
a sound after another one. */
class SoundInstance
{
public:
	SoundInstance(std::string soundName, std::string nameOfInstance = "", int delay = 0);

	int GetDelay();
	void SetDelay(int delay);
	void DecrementDelay();

	std::string GetSoundName();
	std::string GetInstanceName();

private:
	int delay;
	std::string soundName;
	std::string instanceName;
};

};

#endif
#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <string>

/*
Something simple to just play a continuous track of music until
it's stopped. Music volume can go up or down, it can be told to stop or
pause, but only one music track can play at once. The filename is stored
so when it is chosen to play so later the track can be checked if it's the right track
and can be kept going if the track is not the right one (music will play
continuously)
*/

struct ALLEGRO_AUDIO_STREAM;

namespace gauntlet
{

class Music
{
public:
	Music();
	~Music();

	void Initialize();
	void Destroy();

	/* Sets the music to play by filename within music directory. Stop any currently playing music */
	void SetFile(std::string filename);
	/* Returns name of music that is loaded to play*/
	std::string GetFilename();

	/* Music will start playing, does nothing if music is already playing */
	void Play();

	/* Returns true if music is currently playing */
	bool IsPlaying();

	/* Set if music should loop, starts on true */
	void SetIsLooping(bool isLooping);

	/* If music is loaded it will pause if passed true, unpause if false*/
	void Pause(bool isPausing);

	/* 100 for full volume, 0 for completely silent, values outside this range are set to closest number in range (100 or 0)*/
	void SetVolume(int volume);
	/* Returns volume of music as a value between 0 (silent) and 100 (full volume) */
	int GetVolume();

private:
	/* Directory path to music files relative to executable file */
	static const char *musicDirectory;

	/* Filename of music loaded in currentMusic, empty if no music is currently set. */
	std::string filenameOfCurrentMusic;
	ALLEGRO_AUDIO_STREAM *currentMusic;

	/* starts as 'true' */
	bool isLooping;

	/* starts as '100'*/
	double volume;
};


};


#endif
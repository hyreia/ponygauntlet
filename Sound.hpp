#ifndef SOUND_HPP
#define SOUND_HPP

#include "SfxManager.hpp"
#include "Music.hpp"

namespace gauntlet
{

/* Convenience class because it makes logical sense and keeps GauntletApp a little cleaner 
 * Combines sfx and music class

*/
class Sound
{
public:
	/* Does nothing */
	Sound();
	/* Does nothing */
	~Sound();

	/* Calls Initialize() on components and sets volumes from ini*/
	void Initialize();

	/* Calls destroy on components*/
	void Destroy();

	SfxManager sfx;
	Music music;
	//At some point there will be voice samples too, maybe

private:
	static const char *soundInfoFile;
	void RecreateSoundIniFile();
};

};

#endif
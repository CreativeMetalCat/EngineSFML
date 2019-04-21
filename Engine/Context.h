#pragma once
#include "SoundContainer.h"

#ifndef _FMOD_HPP
#include <fmod.hpp>
#endif

#ifndef _FMOD_ERRORS_H
#include <fmod_errors.h>
#endif


//class that contains and hanldes all game resources
//Made to create ability to pass resources between objects
class Context sealed
{
public:
	std::unique_ptr<Engine::Resources::Sound::CSoundContainer> Sounds;

	FMOD::System* lowLevelSoundSystem = NULL;

	void Init(std::string path);


	Context(std::string path);
	~Context();
};


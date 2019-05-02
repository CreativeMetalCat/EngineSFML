#pragma once
#include "SoundContainer.h"

#ifndef _FMOD_HPP
#include <fmod.hpp>
#endif

#ifndef _FMOD_ERRORS_H
#include <fmod_errors.h>
#endif

#ifndef CHIPMUNK_H
#include <chipmunk.h>
#endif

namespace Engine
{
	class CActor;

	//class that contains and hanldes all game resources
	//Made to create ability to pass resources between objects
	class Context sealed
	{
		std::string path;
	public:
		std::unique_ptr<Engine::Resources::Sound::CSoundContainer> Sounds;

		std::vector<std::shared_ptr<Engine::CActor>>SceneActors;

		FMOD::System* lowLevelSoundSystem = NULL;

		cpSpace* space;

		void Init(std::string path);

		void AddActor(CActor* actor)
		{
			
			SceneActors.push_back(std::shared_ptr<CActor>(actor));
		}
		Context(std::string path);
		~Context();
	};
}

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
	namespace MapLoading
	{
		enum MapAction
		{
			//no action is needed right now
			None,
			//map is needed to load(this action does not unloads previously loaded)
			Load,
			//map is needed to unload(this action does not loads next)
			UnLoad,
			//map is needed to change(unloads old one, loads new)
			Change
		};

		struct MapActionData
		{
		protected:
		
		public:
			bool m_loaded = false;
			std::string m_name = "";

			MapAction m_current_action = MapAction::None;

			MapAction GetCurrentAction()const { return m_current_action; }

			bool GetMapIsLoaded()const { return m_loaded; }

			std::string GetCurrentMapName()const { return m_name; }

			void SetNewMap(std::string name);

			MapActionData(std::string Name = "");

		};
	}
	class CActor;

	//class that contains and hanldes all game resources
	//Made to create ability to pass resources between objects
	class Context sealed
	{
		std::string path;
	public:
		MapLoading::MapActionData MapAction; 

		//part of LoadMap
		void UnloadMap() { MapAction.m_current_action = MapLoading::MapAction::UnLoad; }
		
		//Part of the LoadMap
		void LoadMapFromFile(std::string name) { MapAction.SetNewMap(name); }

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

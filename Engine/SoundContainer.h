#pragma once
//#include "Object.h"
#include <iostream>
#include <vector>

#ifndef CLASS_CSOUNDSOURCE
#include "SoundResource.h"
#endif // !CLASS_CSOUNDSOURCE

#define CLASS_CSOUNDHANDLER 9

namespace Engine::Resources::Sound
{
	class CSoundContainer
	{
	public:

		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_CSOUNDHANDLER;

		virtual int GetClassID()const { return CLASS_CSOUNDHANDLER; }

		std::vector<std::shared_ptr< CSoundResource>> Sounds;

		CSoundResource* GetSoundByName(std::string name);

		CSoundResource* GetSoundByID(size_t id);

		void AddSoundResource(std::shared_ptr<CSoundResource> r);

		//loads all textures by calling CTextureResource->Init(std::string path)
		virtual void Init(std::string path, FMOD::System*& system);

		CSoundContainer(std::string path);
		~CSoundContainer();
	};
}

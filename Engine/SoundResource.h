#pragma once
#include <iostream>

#ifndef _FMOD_HPP
#include <fmod.hpp>
#endif

#ifndef _FMOD_ERRORS_H
#include <fmod_errors.h>
#endif

#define CLASS_CSOUNDSOURCE 7

namespace Engine::Resources::Sound
{
	class CSoundResource
	{
	protected:
		
	public:
		FMOD::Sound* m_sound;

		FMOD::Sound*& GetSound() { return m_sound; }
		
		//name of the texture that will be used 
		std::string Name;

		//name of the file that will be loaded
		std::string NameOfFile;


		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_CSOUNDSOURCE;

		virtual int GetClassID()const { return CLASS_CSOUNDSOURCE; }

		void Init(std::string path);

		FMOD_CREATESOUNDEXINFO InitSound(std::string path,FMOD::System*&system);

		bool IsNull()const{}
		CSoundResource(std::string Name, std::string NameOfFile,std::string path);
		~CSoundResource();
	};
}

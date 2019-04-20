#include "SoundResource.h"


namespace Engine::Resources::Sound
{
	void CSoundResource::Init(std::string path)
	{

	}

	FMOD_CREATESOUNDEXINFO CSoundResource::InitSound(std::string path, FMOD::System*& system)
	{
		FMOD_CREATESOUNDEXINFO CreateSoundInfo;
		FMOD_RESULT res =system->createSound(NameOfFile.c_str(), FMOD_2D, &CreateSoundInfo, &m_sound);
		if (res != FMOD_RESULT::FMOD_OK)
		{
			std::cout << "Failed to load sound. " << FMOD_ErrorString(res) << std::endl;
		}
		return CreateSoundInfo;
	}

	CSoundResource::CSoundResource(std::string Name, std::string NameOfFile, std::string path) :
		CObject(path),
		NameOfFile(NameOfFile),
		Name(Name)
	{

	}


	CSoundResource::~CSoundResource()
	{
	}
}
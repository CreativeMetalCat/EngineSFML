#include "Context.h"




namespace Engine
{

	Context::Context(std::string path):path(path)
	{

		Sounds = std::make_unique<Engine::Resources::Sound::CSoundContainer>(path);
		TextureResources = std::make_unique<Resources::Materials::CTextureContainer>(path);
		ShaderResources = std::make_unique<Resources::Materials::CShaderContainer>(path);

		FMOD_RESULT res;
		res = FMOD::System_Create(&lowLevelSoundSystem);
		if (res != FMOD_RESULT::FMOD_OK)
		{
			std::cout << FMOD_ErrorString(res) << std::endl;
		}
		else
		{
			std::cout << "Sound system was created without errors" << std::endl;
		}
		res = lowLevelSoundSystem->init(1024, FMOD_INIT_NORMAL, NULL);
		if (res != FMOD_RESULT::FMOD_OK)
		{
			std::cout << FMOD_ErrorString(res) << std::endl;
		}
		else
		{
			std::cout << "Sound system was inited without errors" << std::endl;
		}

		// cpVect is a 2D vector and cpv() is a shortcut for initializing them.
		cpVect gravity = cpv(0, 9.8f / 100000);

		// Create an empty space.
		space = cpSpaceNew();
		cpSpaceSetGravity(space, gravity);

	}

	Context::~Context()
	{
	}
}

void Engine::MapLoading::MapActionData::SetNewMap(std::string name)
{
	if (name != m_name && !this->m_loaded)
	{
		this->m_name = name;
		this->m_loaded = false;
		this->m_current_action = MapAction::Load;
	}
	else if (name != m_name && this->m_loaded)
	{
		this->m_name = name;
		this->m_loaded = false;
		this->m_current_action = MapAction::Change;
	}
}

Engine::MapLoading::MapActionData::MapActionData(std::string Name):m_name(Name)
{
}

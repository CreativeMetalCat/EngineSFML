#include "Context.h"



Context::Context(std::string path)
{

	Sounds = std::make_unique<Engine::Resources::Sound::CSoundContainer>(path);

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

}


Context::~Context()
{
}

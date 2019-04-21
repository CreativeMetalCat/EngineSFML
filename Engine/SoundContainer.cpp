#include "SoundContainer.h"


namespace Engine::Resources::Sound
{
	CSoundResource* CSoundContainer::GetSoundByName(std::string name)
	{
		if (Sounds.empty()) { return nullptr; }
		else
		{
			for (auto var : Sounds)
			{
				if (var->Name == name) { return &(*var); }
			}
		}
	}

	CSoundResource* CSoundContainer::GetSoundByID(size_t id)
	{
		if (Sounds.empty()) { return nullptr; }
		else if (id<Sounds.size() && id>-1)
		{
			return &(*Sounds[id]);
		}
	}

	void CSoundContainer::AddSoundResource(std::shared_ptr<CSoundResource> r)
	{
		this->Sounds.push_back(r);
	}

	void CSoundContainer::Init(std::string path, FMOD::System*& system)
	{
		if (!Sounds.empty())
		{
			for (auto Sound : Sounds)
			{
				Sound->InitSound(path,system);
			}
		}
	}

	CSoundContainer::CSoundContainer(std::string path)
	{
		
	}


	CSoundContainer::~CSoundContainer()
	{
	}

}
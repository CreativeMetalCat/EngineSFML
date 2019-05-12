#include "SpriteArrayAnimation.h"


namespace Engine::Animation
{
	SpriteArrayAnimation::SpriteArrayAnimation(std::string Name, bool IsRepated, float FrameDuration, std::vector<std::string>spriteNames):
		m_sprite_names(spriteNames),
		Name(Name)
	{
	}

	//we can not use WorldContext here because it will create loop depency
	void SpriteArrayAnimation::Init(std::string path)
	{
		//we can not use WorldContext here because it will create loop depency
	}

	void SpriteArrayAnimation::Update(sf::Time dt)
	{
		m_time += dt.asSeconds();
		if (m_time >= FrameDuration)
		{
			(CurrentSpriteId + 1 < m_sprite_array.size()) ? CurrentSpriteId++ : (IsRepated ? CurrentSpriteId = 0 : CurrentSpriteId = CurrentSpriteId);
		}
	}

	void SpriteArrayAnimation::Release()
	{
		m_sprite_array.~vector();
		m_sprite_names.~vector();
	}

	SpriteArrayAnimation::~SpriteArrayAnimation()
	{
	}
}
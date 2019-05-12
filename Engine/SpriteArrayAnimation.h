#pragma once
#include "SpriteAnimation.h"
namespace Engine::Animation
{
	class SpriteArrayAnimation :
		public SpriteAnimation
	{
	public:

		std::string Name;

		//time since last frame update. if your object doesn't keep track of it
		float m_time = 0.0f;

		//if set to false stops on last frame of animation
		bool IsRepated = true;

		float FrameDuration = 2.f;

		std::vector<sf::Sprite>m_sprite_array;

		std::vector<std::string>m_sprite_names;

		size_t CurrentSpriteId = 0;

		SpriteArrayAnimation(std::string Name, bool IsRepated, float FrameDuration,std::vector<std::string>spriteNames);

		sf::Sprite GetSprite()const override { return (CurrentSpriteId < m_sprite_array.size()) ? m_sprite_array[CurrentSpriteId] : m_sprite_array[0]; }

		sf::Sprite GetSpriteById(size_t id )const { return (id < m_sprite_array.size()) ? m_sprite_array[id] : m_sprite_array[0]; }

		//we can not use WorldContext here because it will create loop depency
		virtual void Init(std::string path);

		virtual void Update(sf::Time dt);

		virtual void Release();

		~SpriteArrayAnimation();
	};
}

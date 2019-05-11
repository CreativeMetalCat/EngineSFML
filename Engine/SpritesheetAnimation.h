#pragma once
#include "SpriteAnimation.h"
namespace Engine::Animation
{
	class SpritesheetAnimationPattern
	{
	protected:
		
	public:

		std::string Name;
		//time since last frame update. if your object doesn't keep track of it
		float m_time = 0.0f;

		//if set to false stops on last frame of animation
		bool IsRepated = true;

		size_t CurrentIndex = 0;

		std::vector<sf::Vector2i>Indices;

		float FrameDuration = 2.f;

		virtual void Update(sf::Time dt);

		virtual void Release();

		sf::Vector2i GetCurrentIndex()const { return CurrentIndex < Indices.size() ? Indices[CurrentIndex] : Indices[0]; }

		SpritesheetAnimationPattern(std::string Name,std::vector<sf::Vector2i>Indices) :Indices(Indices),Name(Name) {}
	};

	class SpritesheetAnimation :
		public SpriteAnimation
	{
	public:
		sf::Sprite m_sprite;

		//current textureRect
		sf::IntRect m_frame;
		//size of frame height(y) and width(x)
		sf::Vector2f m_frameSize;

		std::string m_spriteName;
	

		std::string CurrentAnimationName = "";

		std::vector<SpritesheetAnimationPattern>Animations;

		SpritesheetAnimation(sf::Vector2f frameSize, std::string spriteName);

		const sf::IntRect GetTextureRect() { return m_frame; }
			
		sf::Vector2f GetFrameSize()const { return m_frameSize; }		

		
		sf::Sprite GetSprite()const { return m_sprite; }

		virtual void Init(std::string path);

		void Update(sf::Time dt);

		virtual void Release();

		~SpritesheetAnimation();
	};
}

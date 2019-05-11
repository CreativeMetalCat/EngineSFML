#include "SpritesheetAnimation.h"

namespace Engine::Animation
{

	void SpritesheetAnimationPattern::Update(sf::Time dt)
	{
		m_time += dt.asSeconds();
		if (m_time >= FrameDuration)
		{
			//c++ is cool (condition ? if true: if false)
			//1) we check if CurrentIndex + 1 is not greater than amount of indices 
			//2) if it is we check if IsRepeated is true
			//3) if yes CurrentIndex is 0 else we do not change it
			(CurrentIndex + 1 >= Indices.size()) ? (IsRepated ? CurrentIndex = 0 : CurrentIndex = CurrentIndex) : CurrentIndex++;
			m_time = 0.f;
		}
	}

	void SpritesheetAnimationPattern::Release()
	{
		Indices.~vector();
	}

	SpritesheetAnimation::SpritesheetAnimation(sf::Vector2f frameSize, std::string spriteName):
		m_frameSize(frameSize),
		m_spriteName(spriteName),
		m_frame({ 0, 0 }, { static_cast<int>(frameSize.x),static_cast<int>(frameSize.y) })
	{

	}

	void SpritesheetAnimation::Init(std::string path)
	{
		
	}

	void SpritesheetAnimation::Update(sf::Time dt)
	{
		if (!Animations.empty() && CurrentAnimationName != "")
		{
			for (auto& pattern : Animations)
			{
				if (pattern.Name == CurrentAnimationName)
				{
					pattern.Update(dt);
					if (this->m_sprite.getTexture()->getSize().x != 0 && this->m_sprite.getTexture()->getSize().y != 0)
					{
						this->m_frame.left = pattern.GetCurrentIndex().x * this->m_frame.width;
						this->m_frame.top = pattern.GetCurrentIndex().y * this->m_frame.height;

						this->m_sprite.setTextureRect(this->m_frame);
					}
				}
			}
		}
	}

	void SpritesheetAnimation::Release()
	{
	}


	SpritesheetAnimation::~SpritesheetAnimation()
	{
	}
	
}
#pragma once
#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif

#include <iostream>
#include <memory>
namespace Engine::Animation
{
	//base class for sprite-based animations
	class SpriteAnimation
	{
	public:
		SpriteAnimation();

		virtual void Init(std::string path) = 0;

		virtual void Update(sf::Time) = 0;

		//Made for easier usage
		//Uses dynamic_cast as of now
		template<class T *> T As()
		{
			return dynamic_cast<T>(this);
		}

		virtual void Release() = 0;

		~SpriteAnimation();
	};
}

#pragma once

#include "Context.h"

namespace Engine
{
	//class that includes sf::Sprite and handles animations etc.
	class Sprite
	{
		
	public:
		sf::Sprite m_sprite;

		std::string m_texture_name = "";

		//Sprite that should be used by Owner
		//defined by animtion or any other way that should be used
		sf::Sprite& GetSprite() { return m_sprite; }

		//avoid using this so sprite and name won't differ
		void SetSprite(sf::Sprite sprite) { this->m_sprite = sprite; }

		//use this only if you know that there is texture with this name loaded in game
		void SetSpriteTexture(sf::Texture& texture, std::string name = "");

		std::string GetTextureName()const { return m_texture_name; }

		//avoid using this so sprite and name won't differ
		void SetTextureName(std::string name) { this->m_texture_name = name; }

		//Finds texture inside of texture array of the Context 
		//returns false if no match was found
		bool SetSpriteByName(std::string name, Engine::Context* WorldContext);

		Sprite(sf::Sprite sprite, std::string name);

		~Sprite();
	};
}

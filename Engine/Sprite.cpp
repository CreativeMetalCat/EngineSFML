#include "Sprite.h"
namespace Engine
{
	void Sprite::SetSpriteTexture(sf::Texture& texture, std::string name)
	{
		m_sprite.setTexture(texture);
		m_texture_name = name;
	}
	bool Sprite::SetSpriteByName(std::string name, Engine::Context* WorldContext)
	{
		if (WorldContext == nullptr)
		{
			for (auto& texture : WorldContext->TextureResources->Textures)
			{
				if (texture->Name == name)
				{
					m_sprite.setTexture(texture->GetTexture());
					m_texture_name = name;
					return true;
					break;
				}
			}
			return false;
		}
		else
		{
			return false;
		}
	}

	Sprite::Sprite(sf::Sprite sprite, std::string name):
		m_sprite(sprite),
		m_texture_name(name)
	{
	}


	Sprite::~Sprite()
	{
	}
}
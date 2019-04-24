#pragma once
#include "Actor.h"

//Actor that has one texture(sprite) and static physical body
//Category: Basic

namespace Engine
{
	class CSolidBlock : public Engine::CActor
	{
		sf::Sprite m_sprite;

		sf::Vector2f Size;


	public:

		void SetSprite(sf::Sprite s) { this->m_sprite = s; }

		sf::Sprite GetSprite()const { return m_sprite; }

		void SetSpriteTexture(sf::Texture& texture);

		virtual void Init(std::string path, Context* context)override;
		virtual void InitPhysBody(std::string path, cpSpace*& world);

		void Draw(sf::RenderWindow& window)override;

		CSolidBlock(sf::Sprite sprite, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, Context* WorldContext, std::string path = "./../");
		~CSolidBlock();
	};
}

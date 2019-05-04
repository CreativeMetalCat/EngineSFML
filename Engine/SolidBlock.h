#pragma once
#include "Actor.h"

//Actor that has one texture(sprite) and static physical body
//Category: Basic

#define CLASS_SOLIDBLOCK 21

namespace Engine
{
	class CSolidBlock : public Engine::CActor
	{
		sf::Sprite m_sprite;

		sf::Vector2f Size;


	public:

		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_SOLIDBLOCK;


		virtual int GetClassID()const { return ClassID; }

		void SetSprite(sf::Sprite s) { this->m_sprite = s; }

		sf::Sprite GetSprite()const { return m_sprite; }

		void SetSpriteTexture(sf::Texture& texture);

		virtual void Init(std::string path)override;
		virtual void InitPhysBody(std::string path, cpSpace*& world);

		void Draw(sf::RenderWindow& window)override;

		CSolidBlock(sf::Sprite sprite, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, Context* WorldContext, std::string path = "./../");
		~CSolidBlock();
	};
}

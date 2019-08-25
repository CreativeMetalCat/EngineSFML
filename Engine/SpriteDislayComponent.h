#pragma once

#include "Component.h"

#define COMP_SPRITE 40

namespace Engine::Component
{
	class CSpriteDisplayComponent :
		public CComponent
	{
	protected:
		sf::Sprite m_sprite;

		sf::Vector2f m_relativeLocation;

		float m_rotation = 0;
	public:

		static const int ClassID = COMP_SPRITE;

		virtual int GetClassID()const { return ClassID; }

		//Create LUA class from this for usage in LUA
		static void RegisterClassLUA(lua_State*& L);

		CSpriteDisplayComponent(sf::Sprite sprite, Context* WorldContext, std::string path = "./../");

		sf::Sprite GetSprite()const { return m_sprite; }

		sf::Vector2f GetWorldLocation()const;

		sf::Vector2f GetRelativeLocation()const { return  m_relativeLocation; }

		float GetWorldRotation()const { return (owner == nullptr) ? m_rotation : owner->GetActorRotation() + this->m_rotation; }

		void SetSprite(sf::Sprite s) { this->m_sprite = s; }

		void SetSpriteTexture(sf::Texture& texture);

		void Draw(sf::RenderWindow& window);

		virtual bool LoadSprite(std::string name);

		virtual void Update(sf::Time)override;

		~CSpriteDisplayComponent();
	};
}

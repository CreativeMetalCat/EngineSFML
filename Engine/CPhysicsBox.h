#pragma once

#include "PhysicalObject.h"

//example of box-shaped physical object with texture
class CPhysicsBox : public Engine::CPhysicalObject
{
protected:
	sf::Vector2f Size;

	sf::Sprite m_sprite;
public:

	void SetSprite(sf::Sprite s) { this->m_sprite = s; }

	sf::Sprite GetSprite()const { return m_sprite; }

	virtual void Init(std::string path, Engine::Context* context)override;

	virtual void Update(sf::Time, Engine::Context* context)override;

	void Draw(sf::RenderWindow& window)override;

	virtual void InitPhysBody(std::string path, cpSpace*& world)override;

	CPhysicsBox(sf::Sprite sprite, sf::Vector2f Size,sf::Vector2f position, std::string path, Engine::Context* WorldContext, float Mass = 100.f, std::string MaterialName = "");
	~CPhysicsBox();
};


#pragma once
#include "Actor.h"
namespace Test
{
	//test class for kinematic body based class
	class FuncElevator : public Engine::CActor
	{
	protected:
		sf::Sprite m_sprite;

		FMOD::Channel* m_movement_sound_channel;

		float m_sound_time = 2.f;
		float m_passed_sound_time = 0.f;
	public:
		sf::Vector2f Size;
		FuncElevator(sf::Sprite sprite, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path = "./../");

		virtual void Init(std::string path)override;

		virtual void InitPhysBody(std::string path, cpSpace*& world)override;

		void Draw(sf::RenderWindow& window)override;

		void Update(sf::Time dt);

		~FuncElevator();
	};
}

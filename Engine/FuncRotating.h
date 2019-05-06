#pragma once
#include "Actor.h"


#define CLASS_FUNCROTATING 23
namespace Test
{
	// test class for kinematic body based class
	class FuncRotating :public Engine::CActor
	{
	protected:
		sf::Sprite m_sprite;

		FMOD::Channel* m_movement_sound_channel;

		FMOD::Channel* m_stop_sound_channel;

		float m_sound_time = 2.f;
		float m_passed_sound_time = 0.f;

		bool m_arrived = false;
		
		//defines should body move or not
		//mainly to prevent playing sound when body is not moving
		bool m_moving = false;
	public:
		sf::Vector2f Size;

		bool Clockwise = true;

		float StartAngle = 0.f;
		
		float EndAngle = 0.f;

		//Actor will be rotated around this point
		//point relative to actor
		sf::Vector2f RotationPoint = sf::Vector2f(0, 0);
		FuncRotating(bool Clockwise, float StartAngle, float EndAngle,sf::Sprite sprite, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path = "./../");

		virtual void Init(std::string path)override;

		virtual void InitPhysBody(std::string path, cpSpace*& world)override;

		void StartMovement();

		void StopMovement();

		void Draw(sf::RenderWindow& window)override;

		void Update(sf::Time dt);
		~FuncRotating();
	};

}
#pragma once
#include "Actor.h"

#define CLASS_FUNCELEVATOR 22
namespace Test
{
	//test class for kinematic body based class
	class FuncElevator : public Engine::CActor
	{
	protected:
		sf::Sprite m_sprite;

		FMOD::Channel* m_movement_sound_channel;

		FMOD::Channel* m_stop_sound_channel;

		float m_sound_time = 2.f;
		float m_passed_sound_time = 0.f;

		bool m_arrived = false;

		float m_distance = 0.f;

		//defines should body move or not
		//mainly to prevent playing sound when body is not moving
		bool m_moving = false;

		bool m_moving_back = false;
	public:
		float Speed = 0.01f;

		sf::Vector2f EndLocation;
		sf::Vector2f StartLocation;

		sf::Vector2f Size;

		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_FUNCELEVATOR;


		virtual int GetClassID()const { return ClassID; }
		FuncElevator(sf::Vector2f EndLocation,sf::Sprite sprite, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path = "./../");

		virtual void Init(std::string path)override;

		virtual void InitPhysBody(std::string path, cpSpace*& world)override;

		void StartMovement();

		void StopMovement();

		void Draw(sf::RenderWindow& window)override;

		void Update(sf::Time dt);

		~FuncElevator();
	};
}

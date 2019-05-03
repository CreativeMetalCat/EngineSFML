#pragma once
#include "Projectile.h"

#define CLASS_TESTPROJECTILE 16

namespace Test
{
	class TestProjectile : public Engine::CProjectile
	{
		sf::Sprite m_sprite;

		sf::Vector2f Size;
	public:


		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_TESTPROJECTILE;

		int GetClassID()const { return  CLASS_TESTPROJECTILE; }

		void SetSprite(sf::Sprite s) { this->m_sprite = s; }

		sf::Sprite GetSprite()const { return m_sprite; }

		virtual void Init(std::string path)override;

		virtual void InitPhysBody(std::string path, cpSpace*& world)override;

		//Create LUA class from this for usage in LUA
		static void RegisterClassLUA(lua_State*& L);

		void Draw(sf::RenderWindow& window)override;

		//Creates object using new key word and 
		//mainly for lua
		//if you need to create object outside of lua use new or smart pointers
		static void CreateInWorld(sf::Vector2f spawnImpulse, sf::Vector2f spawnImpulseLocalLocation, sf::Sprite sprite, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path);

		virtual void Update(sf::Time dt)override;

		virtual void Release()override;

		//PATH - Path to main folder and usually used to access scripts
		//Defined by window.lua
		virtual void OnBeginCollision(cpArbiter*& arb, CActor* otherActor);

		//PATH - Path to main folder and usually used to access scripts
		//Defined by window.lua
		virtual void OnEndCollision(cpArbiter*& arb, CActor* otherActor);

		TestProjectile(sf::Sprite sprite, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path = "./../");

		~TestProjectile();
	};
}


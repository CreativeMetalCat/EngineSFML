#pragma once
#include "CTrigger.h"

	class CElevatorTrigger : public Engine::CTrigger
	{
	public:
		std::string targetName = "";

		CElevatorTrigger(std::string targetName, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path);

		virtual void Init(std::string path)override;

		virtual void InitPhysBody(std::string path, cpSpace*& world)override;

		//PATH - Path to main folder and usually used to access scripts
		//Defined by window.lua
		virtual void OnBeginCollision(cpArbiter*& arb, CActor* otherActor);

		virtual void OnEndCollision(cpArbiter*& arb, CActor* otherActor);

		//Create LUA class from this for usage in LUA
		static void RegisterClassLUA(lua_State*& L);

		~CElevatorTrigger();
	};


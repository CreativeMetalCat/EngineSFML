#pragma once
#include "Actor.h"
namespace Engine
{
	class CTrigger : public CActor
	{
	public:
		sf::Vector2f Size;
		CTrigger(std::string CollisionScriptFileName,sf::Vector2f Size,sf::Vector2f Location, Context* WorldContext, std::string path = "./../");

		virtual void Init(std::string path)override;

		virtual void InitPhysBody(std::string path, cpSpace*& world)override;

		//PATH - Path to main folder and usually used to access scripts
		//Defined by window.lua
		virtual void OnBeginCollision(cpArbiter*& arb, CActor* otherActor);

		virtual void OnEndCollision(cpArbiter*& arb, CActor* otherActor);

		//Create LUA class from this for usage in LUA
		static void RegisterClassLUA(lua_State*& L);

		~CTrigger();
	};
}

#pragma once
#include "Actor.h"

namespace Engine
{
	//base class for the Charaters,that defines basic movement using Box2D's physics and LUA as well as basic collision
	class Character : public CActor
	{
	protected:
		sf::Vector2f MaxVelocity = sf::Vector2f(1, 50);

		//LUA script file that will be used for movement
		//it is not obligatory to use LUA script
		std::string MovementScriptFileName = "character.lua";

		sf::Vector2f Size;


		bool m_isOnTheGround = false;

		cpConstraint* FeetJoint = nullptr;
	public:
		bool GetIsOnTheGround();

		bool IsMovingX = false;

		bool IsMovingY = false;

		//LUA script file that will be used for movement
		//it is not obligatory to use LUA script
		void SetMovementScriptFileName(std::string f) { MovementScriptFileName = f; }

		//LUA script file that will be used for movement
		//it is not obligatory to use LUA script
		std::string GetMovementScriptFileName()const { return MovementScriptFileName; }

		sf::Vector2f GetMaxVelocity()const { return MaxVelocity; }

		void SetMaxVelocity(sf::Vector2f v) { this->MaxVelocity = v; }


		inline void MoveX(float value);

		void Jump();

		//Forces body to stop x velocity
		void StopXMovement();

		DEPRECATED_ERROR inline void MoveY(float value);

		virtual void Init(std::string path);
		virtual void InitPhysBody(std::string path, cpSpace*& world);

		//Create LUA class from this for usage in LUA
		static void RegisterClassLUA(lua_State*& L);

		//PATH - Path to main folder and usually used to access scripts
		//Defined by window.lua
		virtual void OnBeginCollision(cpArbiter*& arb, CActor* otherActor);

		virtual void OnEndCollision(cpArbiter*& arb, CActor* otherActor);


		void Update(sf::Time dt);
		Character(sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, Context* WorldContext, std::string path = "./../");
		~Character();
	};

}

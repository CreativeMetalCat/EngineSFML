#pragma once
#include "Object.h"
#include "Sprite.h"



#ifndef CHIPMUNK_H
#include <chipmunk.h>
#endif

#define CLASS_ACTOR 2

namespace Engine
{
	//Base of all objects that can be placed in the scene
	class CActor : public CObject
	{

	protected:
		sf::Vector2f Location;

		std::vector<std::shared_ptr<CActor>> Children;

		//is set manually or by some function in children class
		//unnessesary if alternative already exists
		bool physBodyInitialized = false;

		//it's id that is usually assinged by layer on the map 
		//needed for the optimization
		int area_id = 0;

		//LUA file that will be used
		std::string CollisionScriptFileName;

		std::vector<cpShape*>shapes;

		float m_lifetime = 0.f;

		float m_lived_time = 0.f;

		bool m_pending_kill = false;
	public:

		//0.f for infinite
		float GetLifeTime()const { return m_lifetime; }

		float GetLivedTime()const { return m_lifetime == 0.f ? 0.f : m_lived_time; }

		bool LifeTimeEnded()const { return m_lifetime != 0.f ? (m_lived_time >= m_lifetime) : false; }

		bool IsPendingKill()const { return m_lifetime == 0.f ? m_pending_kill : (LifeTimeEnded() || m_pending_kill); }

		//Use this to safely mark object for destruction
		virtual void DestroyActor();

		

		virtual bool GetIsValid()const;

		//0.f for infinite
		void setLifeTime(float time) { m_lifetime = time; }

		//Shape that will be used for the collision
		//THIS SHOULD BE USED ONLY IF POLYGON SHAPE IS NOT WORKING (If this can not be used it's better to use ShadowShape)
		//there is no way to properly check if collision is good for the polygon shape at runtime and change
		sf::FloatRect CollisionRectangle;

		//Shape that will be used for making shadows in game
		sf::ConvexShape ShadowShape;

		int GetClassID()const { return ClassID; }

		cpShape* GetShape(int i = 0);

		//LUA file that will be used
		void SetCollisionScriptFileName(std::string CollisionScriptFileName) { this->CollisionScriptFileName = CollisionScriptFileName; }

		//LUA file that will be used
		std::string GetCollisionScriptFileName()const { return CollisionScriptFileName; }

		void SetAreaId(int id) { area_id = id; }

		int GetAreaId()const { return area_id; }


		bool GetPhysBodyInitialized()const { return physBodyInitialized; }

		void SetPhysBodyInitialized(bool is) { physBodyInitialized = is; }

		//Physical body of the CActor
		//find way use smart pointers right now it does not
		cpBody* Body = nullptr;

		//for the lua
		//not const for LUA
		cpBody* GetBody() { return Body; }

		sf::Vector2f GetLinearVelocity()const
		{
			cpVect vel = cpBodyGetVelocity(this->Body);
			return sf::Vector2f(vel.x, vel.y);
		}

		//Imp - impluse to apply
		//LocalPoint - Point in the body where impulse will be aplied
		void ApplyLinearImpulse(cpVect imp, cpVect localPoint)
		{
			cpBodyApplyImpulseAtLocalPoint(this->GetBody(), imp, localPoint);
			//Body->SetLinearVelocity(vel);
		}

		void ApplyLinearImpulseToZero(float x, float y)
		{
			cpBodyApplyImpulseAtLocalPoint(this->GetBody(), cpv(x, y), cpv(0, 0));
		}

		//returns copy of the Array
		//Made primarly for the LUA
		std::vector<std::shared_ptr<CActor>> GetChildren()const { return Children; }

		//Add child to array
		//Made primarly for the LUA
		void AddChildRaw(CActor* a);

		//Add child to array
		//Made primarly for the LUA
		void AddChild(std::shared_ptr<CActor> a);

		//Get child by index
		//Made primarly for the LUA
		CActor* GetChild(unsigned int index);

		//Get child by index
		std::shared_ptr<CActor> GetChildAsSharedPtr(unsigned int index);

		//ID of parent for checking
		//default is CObject
		static const int ParentClassID = CLASS_OBJECT;

		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_ACTOR;

		sf::Vector2f GetActorLocation() const { return Location; }

		void SetActorLocation(sf::Vector2f l) { Location = l; }

		//void SetActorLocation(float x, float y) { Location.x = x; Location.y = y; }

		virtual void Draw(sf::RenderWindow& window) {}

		virtual void InitPhysBody(std::string path, cpSpace*& world) {}

		//Create LUA class from this for usage in LUA
		static void RegisterClassLUA(lua_State*& L);

		//PATH - Path to main folder and usually used to access scripts
		//Defined by window.lua
		virtual void OnBeginCollision(cpArbiter*& arb, CActor* otherActor);

		//PATH - Path to main folder and usually used to access scripts
		//Defined by window.lua
		virtual void OnEndCollision(cpArbiter*& arb, CActor* otherActor);

		virtual void Release()override;

		CActor(sf::Vector2f Location, Context* WorldContext, std::string path = "./../");
		virtual ~CActor();
	};

}
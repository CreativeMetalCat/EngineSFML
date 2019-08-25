#pragma once
#include "Object.h"
#include "Actor.h"

#define CLASS_COMPONENT 39

namespace Engine::Component
{
	/*Base Class that can be dynamicly added to the object*/
	class CComponent :public Engine::CObject
	{
	protected:
		CActor* owner = nullptr;
	public:
		static const int ClassID = CLASS_COMPONENT;

		virtual int GetClassID()const { return ClassID; }

		bool IsOwnerValid()const { return owner->GetIsValid(); }

		CActor*& TryGetOwner() { return owner; }

		//Create LUA class from this for usage in LUA
		static void RegisterClassLUA(lua_State*& L);

		virtual void AddToActor(CActor* actor);

		CComponent(Context* WorldContext, std::string path);
		~CComponent();
	};
}


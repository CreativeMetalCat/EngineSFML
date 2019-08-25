#include "Component.h"

namespace Engine::Component
{
	void CComponent::RegisterClassLUA(lua_State*& L)
	{
		using namespace luabridge;
		try
		{
			//Register CActor in lua
			getGlobalNamespace(L)
				.beginClass<CComponent>("CComponent")
				.addConstructor<void(*)(Context * WorldContext, std::string path)>()
				.addFunction("GetClassID", &CComponent::GetClassID)
				.addFunction("Init", &CComponent::Init)
				.addFunction("AddToActor",&CComponent::AddToActor)
				.endClass();
		}
		catch (LuaException e)
		{
			std::cout << "Failed to register class in LUA" << std::endl;
		}
		catch (std::exception e)
		{
			std::cout << "Failed to register class in LUA " << e.what() << std::endl;
		}
	}

	void CComponent::AddToActor(CActor* actor)
	{
		actor->AddComponent(this);
	}

	CComponent::CComponent(Context* WorldContext, std::string path) :CObject(WorldContext, path)
	{
	}


	CComponent::~CComponent()
	{
	}
}
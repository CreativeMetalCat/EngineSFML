#include "Actor.h"


namespace Engine
{
	void CActor::DestroyActor()
	{
		this->m_pending_kill = true;
	}
	bool CActor::GetIsValid() const
	{
		return m_pending_kill;
	}
	cpShape* CActor::GetShape(int i)
	{
		if (i >= shapes.size() || i < 0) { return nullptr; }
		else
		{
			return shapes.at(i);
		}

	}

	void CActor::AddChildRaw(CActor* a)
	{
		std::shared_ptr<CActor>ptr(a);
		this->Children.push_back(ptr);
	}

	void CActor::AddChild(std::shared_ptr<CActor> a)
	{
		this->Children.push_back(a);
	}

	CActor* CActor::GetChild(unsigned int index)
	{
		if (index < this->Children.size())
		{
			return  &(*this->Children.at(index));
		}
		else
		{
			return nullptr;
		}
	}

	std::shared_ptr<CActor> CActor::GetChildAsSharedPtr(unsigned int index)
	{
		if (index < this->Children.size() - 1 && index >= 0)
		{
			return  this->Children.at(index);
		}
		else
		{
			return nullptr;
		}

	}


	void CActor::RegisterClassLUA(lua_State*& L)
	{
		using namespace luabridge;
		try
		{
			//Register CActor in lua
			getGlobalNamespace(L)
				.beginClass<CActor>("CActor")
				//.addConstructor<void(*) (sf::Vector2f)>()

				.addProperty("Location", &CActor::GetActorLocation, &CActor::SetActorLocation)
				.addProperty("PhysBodyInitialized", &CActor::GetPhysBodyInitialized, &CActor::SetPhysBodyInitialized)
				.addProperty("AreaId", &CActor::GetAreaId, &CActor::SetAreaId)

				.addFunction("AddChildRaw", &CActor::AddChildRaw)
				.addFunction("GetChild", &CActor::GetChild)
				.addFunction("GetBody", &CActor::GetBody)
				.addFunction("ApplyLinearImpulse", &CActor::ApplyLinearImpulseToZero)
				.addFunction("GetClassID", &CActor::GetClassID)
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

	void CActor::OnBeginCollision(cpArbiter*& arb, CActor* otherActor)
	{
		using namespace luabridge;
		lua_State* L = luaL_newstate();
		std::string d = (path + "scripts/actor.lua");
		try
		{

			int status = luaL_dofile(L, d.c_str());
			if (status != 0)
			{
				fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
			}
			luaL_openlibs(L);

			lua_pcall(L, 0, 0, 0);

			//Register this class in lua
			this->RegisterClassLUA(L);

			//register other CActor's class
			otherActor->RegisterClassLUA(L);

		

			//Register Vector2 in lua
			getGlobalNamespace(L)
				.beginClass<sf::Vector2f>("Vector2")
				//add x,y and some functions possibly
				.addData<float>("x", &sf::Vector2<float>::x)
				.addData<float>("y", &sf::Vector2<float>::y)
				.addConstructor<void(*) (void)>()
				.endClass();

			getGlobalNamespace(L)
				.beginClass<cpArbiter>("cpArbiter")

				.endClass();

			LuaRef LUAOnBeginCollision = getGlobal(L, "OnBeginCollision");
			if (LUAOnBeginCollision.isFunction())
			{
				LUAOnBeginCollision(this, arb, &(*otherActor));
			}
		}
		catch (LuaException e)
		{
			std::cout << e.what() << std::endl;
		}

		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	void CActor::OnEndCollision(cpArbiter * &arb, CActor * otherActor)
	{
		using namespace luabridge;
		lua_State* L = luaL_newstate();
		std::string d = (path + "scripts/actor.lua");
		try
		{

			int status = luaL_dofile(L, d.c_str());
			if (status != 0)
			{
				fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
			}
			luaL_openlibs(L);

			lua_pcall(L, 0, 0, 0);

			//Register this class in lua
			this->RegisterClassLUA(L);

			//register other CActor's class
			otherActor->RegisterClassLUA(L);

		

			//Register Vector2 in lua
			getGlobalNamespace(L)
				.beginClass<sf::Vector2f>("Vector2")
				//add x,y and some functions possibly
				.addData<float>("x", &sf::Vector2<float>::x)
				.addData<float>("y", &sf::Vector2<float>::y)
				.addConstructor<void(*) (void)>()
				.endClass();

			getGlobalNamespace(L)
				.beginClass<cpArbiter>("cpArbiter")

				.endClass();

			LuaRef LUAOnEndCollision = getGlobal(L, "OnEndCollision");
			if (LUAOnEndCollision.isFunction())
			{
				LUAOnEndCollision(this, arb, &(*otherActor));
			}
		}
		catch (LuaException e)
		{
			std::cout << e.what() << std::endl;
		}

	}

	void CActor::Release()
	{
		cpSpaceRemoveBody(WorldContext->space, this->Body);
		for (auto& shape : this->shapes)
		{
			cpSpaceRemoveShape(WorldContext->space, shape);
			cpShapeDestroy(shape);
			cpShapeFree(shape);
		}

		cpBodyDestroy(this->Body);
		cpBodyFree(this->Body);
	}

	CActor::CActor(sf::Vector2f Location, Context* WorldContext, std::string path)
		:CObject(WorldContext,path),
		Location(Location)
	{

	}


	CActor::~CActor()
	{
	}
}
#include "Actor.h"



cpShape * CActor::GetShape(int i)
{
	if (i >= shapes.size() || i < 0) { return nullptr; }
	else
	{
		return shapes.at(i);
	}

}

void CActor::AddChildRaw(CActor * a)
{
	std::shared_ptr<CActor>ptr(a);
	this->Children.push_back(ptr);
}

void CActor::AddChild(std::shared_ptr<CActor> a)
{
	this->Children.push_back(a);
}

CActor * CActor::GetChild(unsigned int index)
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


void CActor::RegisterClassLUA(lua_State *&L)
{
	using namespace luabridge;
	try
	{
		//Register CActor in lua
		getGlobalNamespace(L)
			.beginClass<CActor>("CActor")
			.addConstructor<void(*) (sf::Vector2f)>()
			.addProperty("Location", &CActor::GetActorLocation, &CActor::SetActorLocation)
			.addProperty("PhysBodyInitialized",&CActor::GetPhysBodyInitialized,&CActor::SetPhysBodyInitialized)
			.addProperty("AreaId",&CActor::GetAreaId,&CActor::SetAreaId)
			.addFunction("AddChildRaw", &CActor::AddChildRaw)
			.addFunction("GetChild", &CActor::GetChild)
			.addFunction("GetBody",&CActor::GetBody)
			.endClass();
	}
	catch(LuaException e)
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

		//Register b2Fixture in lua
		getGlobalNamespace(L)
			.beginClass<b2Fixture>("b2Fixture")
			.endClass();

		//Register Vector2 in lua
		getGlobalNamespace(L)
			.beginClass<sf::Vector2f>("Vector2")
			//add x,y and some functions possibly
			.addData<float>("x", &sf::Vector2<float>::x)
			.addData<float>("y", &sf::Vector2<float>::y)
			.addConstructor<void(*) (void)>()
			.endClass();

		getGlobalNamespace(L)
			.beginClass<b2Vec2>("b2Vector")
			.addData<float>("x", &b2Vec2::x)
			.addData<float>("y", &b2Vec2::y)
			.addConstructor<void(*) (void)>()
			.endClass();

		getGlobalNamespace(L)
			.beginClass<b2Body>("b2Body")
			.addFunction("GetLinearVelocity", &b2Body::GetLinearVelocity)
			.addFunction("GetMass", &b2Body::GetMass)
			.addFunction("ApplyImpulse", &b2Body::ApplyLinearImpulseToCenter)
			.endClass();

		getGlobalNamespace(L)
			.beginClass<cpArbiter>("cpArbiter")

			.endClass();

		LuaRef LUAOnBeginCollision = getGlobal(L, "OnBeginCollision");
		if (LUAOnBeginCollision.isFunction())
		{
			LUAOnBeginCollision(this,arb, &(*otherActor));
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

void CActor::OnEndCollision(CActor* otherActor, b2Fixture * fixtureA, b2Fixture * fixtureB, std::string PATH)
{
	using namespace luabridge;
	lua_State* L = luaL_newstate();
	std::string d = (PATH + "scripts/actor.lua");
	try
	{

		int status = luaL_dofile(L, d.c_str());
		if (status != 0)
		{
			fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		}
		luaL_openlibs(L);

		lua_pcall(L, 0, 0, 0);

		//Register CActor in lua
		CActor::RegisterClassLUA(L);

		//register other CActor's class
		otherActor->RegisterClassLUA(L);

		//Register b2Fixture in lua
		getGlobalNamespace(L)
			.beginClass<b2Fixture>("b2Fixture")
			.endClass();


		LuaRef LUAOnEndCollision = getGlobal(L, "OnEndCollision");
		if (LUAOnEndCollision.isFunction())
		{
			LUAOnEndCollision(this, &(*otherActor), fixtureA, fixtureB);
		}
	}
	catch (LuaException e)
	{
		std::cout << e.what() << std::endl;
	}

}

CActor::CActor(sf::Vector2f Location, std::string path):CObject(path),Location(Location)
{
	
}


CActor::~CActor()
{
}

#include "TestActor.h"
#include "SpriteDislayComponent.h"


CTestActor::CTestActor(sf::Vector2f Location, Engine::Context* WorldContext, std::string path):CActor(Location,WorldContext,path)
{
	this->ScriptFileName = "scripts/testactor.lua";

	using namespace luabridge;
	lua_State* L = luaL_newstate();
	std::string d = (path + ScriptFileName);
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

		Engine::Component::CSpriteDisplayComponent::RegisterClassLUA(L);

		

		//register other CActor's class




		//Register Vector2 in lua
		getGlobalNamespace(L)
			.beginClass<sf::Vector2f>("Vector2")
			//add x,y and some functions possibly
			.addData<float>("x", &sf::Vector2<float>::x)
			.addData<float>("y", &sf::Vector2<float>::y)
			.addConstructor<void(*) (void)>()
			.endClass();

		getGlobalNamespace(L)
			.beginClass<Engine::Context>("WorldContext")

			.endClass();


		LuaRef LuaConstruct = getGlobal(L, "Construct");
		if (LuaConstruct.isFunction())
		{
			LuaConstruct(this, WorldContext, path);
		}
	}
	catch (LuaException e)
	{
		std::cout << e.what() << std::endl;
	}
}

void CTestActor::RegisterClassLUA(lua_State*& L)
{
	using namespace luabridge;
	try
	{
		CActor::RegisterClassLUA(L);

		//Register CTestActor in lua
		getGlobalNamespace(L)
			.deriveClass<CTestActor,CActor>("CTestActor")
			//.addConstructor<void(*) (sf::Vector2f)>()

			.addProperty("Location", &CTestActor::GetActorLocation, &CTestActor::SetActorLocation)
			.addProperty("PhysBodyInitialized", &CTestActor::GetPhysBodyInitialized, &CTestActor::SetPhysBodyInitialized)
			.addProperty("AreaId", &CTestActor::GetAreaId, &CTestActor::SetAreaId)

			.addFunction("AddChildRaw", &CTestActor::AddChildRaw)
			.addFunction("GetChild", &CTestActor::GetChild)
			.addFunction("GetBody", &CTestActor::GetBody)
			.addFunction("ApplyLinearImpulse", &CTestActor::ApplyLinearImpulseToZero)
			.addFunction("GetClassID", &CTestActor::GetClassID)
			.addFunction("AddComponent", &CTestActor::AddComponent)
			.addFunction("Test",&CTestActor::TestFunction)
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

void CTestActor::Draw(sf::RenderWindow& window)
{
	if (!this->ActorComponents.empty())
	{
		for (size_t i = 0; i < this->ActorComponents.size(); i++)
		{
			if (this->ActorComponents[i] != nullptr)
			{
				this->ActorComponents[i]->As<Engine::Component::CSpriteDisplayComponent*>()->Draw(window);
			}
		}
	}
}

CTestActor::~CTestActor()
{
}

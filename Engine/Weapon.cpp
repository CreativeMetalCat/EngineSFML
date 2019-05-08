#include "Weapon.h"


namespace Gameplay
{
	Weapon::Weapon(std::string luaScriptName, Engine::Context* WorldContext, std::string path) :
		Engine::CObject(WorldContext, path),
		m_luaScriptName(luaScriptName)
	{
		m_weaponLua = luaL_newstate();
	}


	void Weapon::Init(std::string path)
	{
		//open Lua State for weapon
		using namespace luabridge;
		std::string scriptPath = path + m_luaScriptName;
		luaL_dofile(this->m_weaponLua, scriptPath.c_str());
		luaL_openlibs(this->m_weaponLua);

		lua_pcall(this->m_weaponLua, 0, 0, 0);
		this->RegisterClassLUA(this->m_weaponLua);

	}

	void Weapon::RegisterClassLUA(lua_State*& L)
	{
		using namespace luabridge;
		try
		{
			//Register CActor in lua
			getGlobalNamespace(L)
				.beginClass<Weapon>("CWeapon")
				.addFunction("GetClassID", &Weapon::GetClassID)
				.addFunction("Init", &Weapon::Init)

				.addData<std::string>("Path", &Weapon::path,false)

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

	void Weapon::Shoot(sf::Sprite sprite, sf::Vector2f Location, float angle)
	{
		using namespace luabridge;

		Test::TestProjectile::RegisterClassLUA(this->m_weaponLua);

		this->RegisterClassLUA(this->m_weaponLua);

		//Register Vector2 in lua
		getGlobalNamespace(this->m_weaponLua)
			.beginClass<sf::Vector2f>("Vector2")
			//add x,y and some functions possibly
			.addData<float>("x", &sf::Vector2<float>::x)
			.addData<float>("y", &sf::Vector2<float>::y)
			.addConstructor<void(*) (void)>()
			.endClass();

		getGlobalNamespace(this->m_weaponLua)
			.beginClass<sf::Sprite>("Sprite")

			.endClass();

		getGlobalNamespace(this->m_weaponLua)
			.beginClass<sf::ConvexShape>("ConvexShape")

			.endClass();

		getGlobalNamespace(this->m_weaponLua)
			.beginClass<Engine::Context>("WorldContext")

			.endClass();

		LuaRef LuaShoot = getGlobal(this->m_weaponLua, "Shoot");
		if (!LuaShoot.isNil() && LuaShoot.isFunction())
		{
			LuaShoot(this,sprite, Location, angle, this->WorldContext);
		}

		LuaShoot.~LuaRef();
	}

	void Weapon::Update(sf::Time dt)
	{
		luabridge::LuaRef LuaUpdate = luabridge::getGlobal(this->m_weaponLua, "Update");
		if (!LuaUpdate.isNil() && LuaUpdate.isFunction())
		{
			LuaUpdate(this,dt.asSeconds());
		}
		LuaUpdate.~LuaRef();
	}

	Weapon::~Weapon()
	{
		lua_close(this->m_weaponLua);
	}
}
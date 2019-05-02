#pragma once
#include "Object.h"
#include "TestProjectile.h"

#define CLASS_WEAPON 17
namespace Gameplay
{
	//Weapon system uses lua scripts to be more flexible
	class Weapon : public Engine::CObject
	{
		lua_State* m_weaponLua;

		std::string m_luaScriptName = "scripts/weapons/weapon.lua";
	public:
		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_WEAPON;

		virtual int GetClassID()const { return ClassID; }

		Weapon(std::string luaScriptName, Engine::Context* WorldContext, std::string path = "./../");

		virtual void Init(std::string path)override;

		virtual void Shoot(sf::Sprite sprite,sf::Vector2f Location, float angle);

		//Create LUA class from this for usage in LUA
		static void RegisterClassLUA(lua_State*& L);
		~Weapon();
	};
}

#pragma once
#include "Actor.h"

#define CLASS_CPROJECTILE 15

namespace Engine
{
	class CProjectile :	public CActor
	{
		
	public:
		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_CPROJECTILE;

		int GetClassID()const { return  CLASS_CPROJECTILE; }


		CProjectile(sf::Vector2f Location, Context* WorldContext, std::string path = "./../");

		//Creates object using new key word and 
		//mainly for lua
		//if you need to create object outside of lua use new or smart pointers
		static void CreateInWorld(sf::Vector2f spawnImpulse, sf::Vector2f spawnImpulseLocalLocation, sf::Vector2f Location, Context* WorldContext, std::string path = "./../");

		~CProjectile();
	};
}


#pragma once
#include <iostream>
#include <memory>

#include <imgui.h>
#include <imgui-SFML.h>


#ifndef lua_h
extern "C"
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
#include <LuaBridge/LuaBridge.h>
#endif



//#include "MainFuncLib.h"


#define CLASS_OBJECT 1

#ifndef DEPRECATED_WARING
#define DEPRECATED_WARING __declspec(deprecated)
#endif // !DEPRECATED_WARING


#ifndef DEPRECATED_ERROR
//Depending on compiler this will act as error or warning ( in vs 2015 it is an error while vs 2017 it is warning)
#define DEPRECATED_ERROR [[deprecated]]
#endif // !DEPRECATED_ERROR

#include "Context.h"

#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif

namespace Engine
{
	//base of the base
	class CObject
	{
	protected:

	public:
		Context* WorldContext;

		std::string path;
		//ID of parent for checking
		//default is CObject
		static const int ParentClassID = CLASS_OBJECT;

		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_OBJECT;


		virtual int GetClassID()const { return ClassID; }

		//Made for easier usage
		//Uses dynamic_cast as of now
		template<class T = CObject *> T As()
		{
			return dynamic_cast<T>(this);
		}

		//Create LUA class from this for usage in LUA
		static void RegisterClassLUA(lua_State*& L);

		virtual void Init(std::string path) {}

		virtual void Update(sf::Time) {}

		virtual void HandleEvent(sf::Event event) {}

		CObject(Context* WorldContext,std::string path);

		virtual void Release() {}
		~CObject();
	};


}
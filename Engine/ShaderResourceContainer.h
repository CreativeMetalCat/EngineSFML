#pragma once
#include "ShaderResource.h"
//prevent from multyple includes
#ifndef _VECTOR_
#include <vector>
#endif
#include <iostream>


//prevent from multyple includes
#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif

#define CLASS_SHADERHANDLER 26

namespace Engine::Resources::Materials
{
	class CShaderContainer
	{
	public:

		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_SHADERHANDLER;

		virtual int GetClassID()const { return  CLASS_SHADERHANDLER; }
		CShaderContainer(std::string path);

		std::vector<std::shared_ptr<CShaderResource>> Shaders;


		CShaderResource* GetShaderByName(std::string name);

		CShaderResource* GetShaderByID(size_t id);

		void AddShaderResource(std::shared_ptr<CShaderResource> r);

		//loads all textures by calling CTextureResource->Init(std::string path)
		virtual void Init(std::string path);
		~CShaderContainer();
	};
}

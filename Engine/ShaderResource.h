#pragma once

#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif
#include <iostream>
#define CLASS_SHADERRESOURCE 25
namespace Engine::Resources::Materials
{
	class CShaderResource
	{
	public:
		std::string path;

		sf::Shader Shader;

		sf::Shader& GetShader() { return Shader; }

		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_SHADERRESOURCE;

		virtual int GetClassID()const { return CLASS_SHADERRESOURCE; }

		//name of the texture that will be used 
		std::string Name;

		//name of the file that will be loaded
		std::string NameOfFileVert;

		//name of the file that will be loaded
		std::string NameOfFileFrag;


		CShaderResource(std::string Name, std::string NameOfFileVert, std::string NameOfFileFrag, std::string path);

		void Init(std::string path);

		~CShaderResource();
	};
}

#pragma once
#include "TextureResource.h"

//prevent from multyple includes
#ifndef _VECTOR_
#include <vector>
#endif

//prevent from multyple includes
#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif

#define CLASS_CTEXTUREHANDLER 5



namespace Engine::Resources
{
	class CTextureContainer : public CObject
	{
	public:

		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_CTEXTUREHANDLER;

		virtual int GetClassID()const { return CLASS_CTEXTUREHANDLER; }

		std::vector<CTextureResource> Textures;

		CTextureContainer(std::string path);

		sf::Texture& GetTextureByName();

		sf::Texture& GetTextureByID();
		//release all resources used by class including container


		~CTextureContainer();
	};
}


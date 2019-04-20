#pragma once
#ifndef CLASS_CTEXTURERESOURCE
#include "TextureResource.h"
#endif // !CLASS_CTEXTURERESOURCE

//prevent from multyple includes
#ifndef _VECTOR_
#include <vector>
#endif

//prevent from multyple includes
#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif

#define CLASS_CTEXTUREHANDLER 5




namespace Engine::Resources::Materials
{
	class CTextureContainer : public CObject
	{
	public:

		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_CTEXTUREHANDLER;

		virtual int GetClassID()const { return CLASS_CTEXTUREHANDLER; }

		std::vector<std::shared_ptr<CTextureResource>> Textures;

		CTextureContainer(std::string path);

		CTextureResource* GetTextureByName(std::string name);

		CTextureResource* GetTextureByID(size_t id);

		void AddTextureResource(std::shared_ptr<CTextureResource> r);

		//loads all textures by calling CTextureResource->Init(std::string path)
		virtual void Init(std::string path)override;

		~CTextureContainer();
	};
}


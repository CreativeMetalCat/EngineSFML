#pragma once
#include "Object.h"

#define CLASS_CTEXTURERESOURCE 6
namespace Engine::Resources
{
	class CTextureResource :public CObject
	{
		bool m_smooth = false;
		bool m_repeated = false;

		sf::Texture m_texture;
	public:

		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_CTEXTURERESOURCE;

		virtual int GetClassID()const { return CLASS_CTEXTURERESOURCE; }

		bool GetIsSmooth()const { return m_smooth; }
		bool GetIsRepeated()const { return m_repeated; }

		sf::Texture GetTexture() { return m_texture; }

		CTextureResource(sf::Texture texture,bool smooth, bool repeated, std::string path);
		~CTextureResource();
	};
}
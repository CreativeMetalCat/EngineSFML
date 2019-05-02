#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#define CLASS_CTEXTURERESOURCE 6
namespace Engine::Resources::Materials
{
	class CTextureResource
	{
	protected:
		bool m_smooth = false;
		bool m_repeated = false;

		
	public:
		//to ensure that object will get proper pointer you can use object itself
		sf::Texture m_texture;

		//name of the texture that will be used 
		std::string Name;

		//name of the file that will be loaded
		std::string NameOfFile;

		//ID of class for Casting
		//ID MUST be defined in the beggining of the file with class
		//default is CObject
		static const int ClassID = CLASS_CTEXTURERESOURCE;

		virtual int GetClassID()const { return CLASS_CTEXTURERESOURCE; }

		bool GetIsSmooth()const { return m_smooth; }

		bool GetIsRepeated()const { return m_repeated; }


		sf::Texture& GetTexture() { return m_texture; }

		CTextureResource(sf::Texture texture, bool smooth, bool repeated, std::string path);

		CTextureResource(std::string Name, std::string NameOfFile, bool smooth, bool repeated, std::string path);

		//creates empty object
		static CTextureResource CreateAsNULL();

		void Init(std::string path);

		bool IsNull()const;

		~CTextureResource();
	};
}
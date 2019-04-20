#include "TextureResource.h"


namespace Engine::Resources::Materials
{
	CTextureResource::CTextureResource(sf::Texture texture, bool smooth, bool repeated, std::string path) :
		CObject(path),
		m_texture(texture),
		m_smooth(smooth),
		m_repeated(repeated)
	{
	}

	CTextureResource::CTextureResource(std::string Name, std::string NameOfFile, bool smooth, bool repeated, std::string path):
		CObject(path),
		m_smooth(smooth),
		m_repeated(repeated),
		NameOfFile(NameOfFile),
		Name(Name)
	{
	}

	CTextureResource CTextureResource::CreateAsNULL()
	{
		return CTextureResource(sf::Texture(), NULL, NULL, nullptr);
	}

	void CTextureResource::Init(std::string path)
	{
		if (NameOfFile != "")//if texture is nameless we skip it 
		{
			if (!m_texture.loadFromFile(path + NameOfFile))
			{
				std::cout << "Failed to load texture Name: " << this->Name << " Path to file" << path + NameOfFile << std::endl;;
			}
			m_texture.setSmooth(m_smooth);
			m_texture.setRepeated(m_repeated);
		}
	}

	bool CTextureResource::IsNull() const
	{
		//primitive check
		//if texture has both values as zero it means it is empty
		//wich means that resource is NULL
		return (this->m_texture.getSize().x == 0 && this->m_texture.getSize().y == 0);		
	}




	CTextureResource::~CTextureResource()
	{
	}
}
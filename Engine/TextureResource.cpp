#include "TextureResource.h"


namespace Engine::Resources
{
	CTextureResource::CTextureResource(sf::Texture texture, bool smooth, bool repeated, std::string path) :
		CObject(path),
		m_texture(texture),
		m_smooth(smooth),
		m_repeated(repeated)
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
			m_texture.loadFromFile(path + NameOfFile);
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
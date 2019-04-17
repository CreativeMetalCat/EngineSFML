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


	CTextureResource::~CTextureResource()
	{
	}
}
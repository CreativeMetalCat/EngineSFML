#include "TextureContainer.h"


namespace Engine::Resources
{
	CTextureContainer::CTextureContainer(std::string path) :CObject(path)
	{
	}

	//if texture was not found emtpy texture will be returned NOT NULL
	CTextureResource* CTextureContainer::GetTextureByName(std::string name)
	{

		if (Textures.empty()) { return nullptr; }
		else
		{
			for  (auto var : Textures)
			{
				if (var->Name == name) { return &(*var); }
			}
		}
		// TODO: insert return statement here
	}

	CTextureResource* CTextureContainer::GetTextureByID(size_t id)
	{
		if (Textures.empty()) { return nullptr; }
		else if (id<Textures.size() && id>-1) { return &(*Textures[id]); }
	}

	void CTextureContainer::AddTextureResource(std::shared_ptr<CTextureResource> r)
	{
		this->Textures.push_back(r);
	}


	void CTextureContainer::Init(std::string path)
	{
		if (!Textures.empty())
		{
			for (auto texture : Textures)
			{
				texture->Init(path);
			}
		}
	}

	CTextureContainer::~CTextureContainer()
	{
	}
}

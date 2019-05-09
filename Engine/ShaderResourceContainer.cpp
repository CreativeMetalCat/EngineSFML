#include "ShaderResourceContainer.h"

namespace Engine::Resources::Materials
{

	CShaderContainer::CShaderContainer(std::string path)
	{
	}

	CShaderResource* CShaderContainer::GetShaderByName(std::string name)
	{
		if (Shaders.empty()) { return nullptr; }
		else
		{
			for (auto var : Shaders)
			{
				if (var->Name == name) { return &(*var); }
			}
		}
		return nullptr;
	}

	CShaderResource* CShaderContainer::GetShaderByID(size_t id)
	{
		if (Shaders.empty()) { return nullptr; }
		else if (id<Shaders.size() && id>-1) { return &(*Shaders[id]); }
		return nullptr;
	}

	void CShaderContainer::AddShaderResource(std::shared_ptr<CShaderResource> r)
	{
		this->Shaders.push_back(r);
	}

	void CShaderContainer::Init(std::string path)
	{
		if (!Shaders.empty())
		{
			for (auto Shader : Shaders)
			{
				Shader->Init(path);
			}
		}
	}

	CShaderContainer::~CShaderContainer()
	{
	}
}
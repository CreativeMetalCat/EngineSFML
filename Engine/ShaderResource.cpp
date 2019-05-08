#include "ShaderResource.h"

namespace Engine::Resources::Materials
{

	CShaderResource::CShaderResource(std::string Name, std::string NameOfFileVert, std::string NameOfFileFrag, std::string path):
		Name(Name),
		NameOfFileVert(NameOfFileVert),
		NameOfFileFrag(NameOfFileFrag),
		path(path)
	{
	}

	void CShaderResource::Init(std::string path)
	{
		if (!this->Shader.loadFromFile(path + NameOfFileVert, path + NameOfFileFrag))
		{
			std::cout << "Failed to load shader Name: " << this->Name << " Path to vertex file" << path + NameOfFileVert << " Path to fragment file" << path + NameOfFileFrag << std::endl;;
		}
	}


	CShaderResource::~CShaderResource()
	{
	}
}
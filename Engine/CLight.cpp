#include "CLight.h"


namespace Engine::Light
{
	CLight::CLight(sf::Vector2f Location, Context* WorldContext, std::string path):
		CActor(Location,WorldContext,path)
	{
	}


	CLight::~CLight()
	{
	}
}
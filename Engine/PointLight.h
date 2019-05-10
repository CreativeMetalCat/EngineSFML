#pragma once
#include "CLight.h"
namespace Engine::Light
{
	class CPointLight : public CLight
	{

	public:
		
		static const ELightType LightType = ELightType::Point;

		std::shared_ptr<ltbl::LightPointEmission>PointLight;

		CPointLight(float shadowOverExtendMultiplier, float radius, sf::Color lightColor, sf::Texture &pointLightTexture, sf::Vector2f Size,sf::Vector2f Location, Context* WorldContext, std::string path = "./../");

		~CPointLight();
	};

}
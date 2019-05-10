#include "PointLight.h"

namespace Engine::Light
{

	CPointLight::CPointLight(float shadowOverExtendMultiplier,float radius,sf::Color lightColor,sf::Texture &pointLightTexture,sf::Vector2f Size, sf::Vector2f Location, Context* WorldContext, std::string path):
		CLight(Location,WorldContext,path)
	{
		PointLight = std::make_shared<ltbl::LightPointEmission>();
		PointLight->_emissionSprite.setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
		PointLight->_emissionSprite.setTexture(pointLightTexture);
		PointLight->_emissionSprite.setScale(Size);
		PointLight->_emissionSprite.setColor(lightColor);
		PointLight->_emissionSprite.setPosition(Location);
		PointLight->_sourceRadius = radius;
		PointLight->_shadowOverExtendMultiplier = shadowOverExtendMultiplier;
	}


	CPointLight::~CPointLight()
	{
	}
}
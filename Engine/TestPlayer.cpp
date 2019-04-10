#include "TestPlayer.h"



void CTestPlayer::SetSpriteTexture(sf::Texture & t)
{
}


CTestPlayer::CTestPlayer(sf::Texture&texture, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, std::string path):Character(CollisionShape,Size,Location,path)
{

	this->sprite = sf::Sprite(texture);

	sf::Vector2f scale;
	
	if (texture.getSize().x != 0) { scale.x = Size.x / texture.getSize().x; }

	if (texture.getSize().y != 0) { scale.y = Size.y / texture.getSize().y; }

	this->sprite.setScale(scale);
	
}

void CTestPlayer::Init(std::string path)
{

	sf::Vector2f scale;

	if (this->sprite.getTexture()->getSize().x != 0) { scale.x = Size.x / this->sprite.getTexture()->getSize().x; }

	if (this->sprite.getTexture()->getSize().y != 0) { scale.y = Size.y / this->sprite.getTexture()->getSize().y; }

	this->sprite.setScale(scale);
}




CTestPlayer::~CTestPlayer()
{
}

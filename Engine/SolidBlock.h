#pragma once
#include "Actor.h"

//Actor that has one texture(sprite) and static physical body
//Category: Basic
class CSolidBlock : public Engine::CActor
{
	sf::Sprite sprite;

	sf::Vector2f Size;


public:

	//Shape that will be used for making shadows in game
	sf::ConvexShape ShadowShape;

	//Shape that will be used for the collision
	//THIS SHOULD BE USED ONLY IF POLYGON SHAPE IS NOT WORKING (If this can not be used it's better to use ShadowShape)
	//there is no way to properly check if collision is good for the polygon shape at runtime and change
	sf::FloatRect CollisionRectangle;

	void SetSprite(sf::Sprite s) { this->sprite = s; }

	sf::Sprite GetSprite()const { return sprite; }

	void SetSpriteTexture(sf::Texture & texture);

	virtual void Init(std::string path)override;
	virtual void InitPhysBody(std::string path, cpSpace *&world);

	void Draw(sf::RenderWindow&window)override;

	CSolidBlock(sf::Texture&texture, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, std::string path = "./../");
	~CSolidBlock();
};


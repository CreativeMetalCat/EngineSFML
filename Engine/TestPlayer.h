#pragma once

#include "Character.h"

#define CLASS_CTESTPLAYER 4

//Example player that moves using physics and has one sprite 
//Category: Test-Basic
class CTestPlayer : public Character
{

	sf::Sprite sprite;
public:

	//ID of class for Casting
	//ID MUST be defined in the beggining of the file with class
	//default is CObject
	static const int ClassID = CLASS_CTESTPLAYER;

	int GetClassID()const { return  CLASS_CTESTPLAYER; }

	void SetSprite(sf::Sprite s) { this->sprite = s; }

	sf::Sprite GetSprite()const { return sprite; }

	void SetSpriteTexture(sf::Texture & texture);

	//Size and Location will also be used to define size of the sprite
	//Scales image inside of this funtion
	//AND does the same in the Init
	CTestPlayer(sf::Texture&texture, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, std::string path = "./../");

	virtual void Init(std::string path)override;

	virtual void InitPhysBody(std::string path, cpSpace*& world)override;

	void Draw(sf::RenderWindow& window)override;

	void Update(sf::Time dt);

	//Create LUA class from this for usage in LUA
	static void RegisterClassLUA(lua_State*& L);

	//PATH - Path to main folder and usually used to access scripts
	//Defined by window.lua
	virtual void OnBeginCollision(cpArbiter*& arb, CActor* otherActor);
	
	virtual void OnEndCollision(cpArbiter*& arb, CActor* otherActor);
	~CTestPlayer();
};


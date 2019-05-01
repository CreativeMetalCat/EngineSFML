#pragma once

#include "Character.h"

#ifndef  CLASS_CSOUNDSOURCE
#include "SoundResource.h"
#endif

#ifndef CLASS_TESTPROJECTILE
#include "TestProjectile.h"
#endif // !CLASS_TESTPROJECTILE

#define CLASS_CTESTPLAYER 4

//Example player that moves using physics and has one sprite 
//Category: Test-Basic
class CTestPlayer : public Engine::Character
{

	sf::Sprite m_sprite;

	FMOD::Channel* m_footstep_sound_channel;

	float m_per_foostep_time = 1.f;
	float m_passed_footstep_time = 0.f;

	bool m_moving_left = false;

	bool m_moving_right = false;
public:
	
	bool ControlledByPlayer = false;
	//ID of class for Casting
	//ID MUST be defined in the beggining of the file with class
	//default is CObject
	static const int ClassID = CLASS_CTESTPLAYER;

	int GetClassID()const { return  CLASS_CTESTPLAYER; }

	void SetSprite(sf::Sprite s) { this->m_sprite = s; }

	sf::Sprite GetSprite()const { return m_sprite; }

	void SetSpriteTexture(sf::Texture & texture);

	//Size and Location will also be used to define size of the sprite
	//Scales image inside of this funtion
	//AND does the same in the Init
	CTestPlayer(sf::Sprite sprite, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path = "./../");

	virtual void Init(std::string path)override;

	virtual void InitPhysBody(std::string path, cpSpace*& world)override;

	void Draw(sf::RenderWindow& window)override;

	void Update(sf::Time dt);

	//Create LUA class from this for usage in LUA
	static void RegisterClassLUA(lua_State*& L);

	//creates copy of this play at given location
	//for testing
	static void Duplicate(sf::Sprite sprite, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f newLocation,bool PlayerControlled, Engine::Context* WorldContext,std::string path);

	void Shoot();
	//PATH - Path to main folder and usually used to access scripts
	//Defined by window.lua
	virtual void OnBeginCollision(cpArbiter*& arb, CActor* otherActor);
	
	virtual void OnEndCollision(cpArbiter*& arb, CActor* otherActor);

	virtual void HandleEvent(sf::Event event)override;
	~CTestPlayer();
};


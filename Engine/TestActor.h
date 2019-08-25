#pragma once
#include "Actor.h"

/*Class for testing how good is idead of having ability of adding componentns in runtime*/
class CTestActor :
	public Engine::CActor
{
public:
	CTestActor(sf::Vector2f Location, Engine::Context* WorldContext, std::string path = "./../");

	//Create LUA class from this for usage in LUA
	static void RegisterClassLUA(lua_State*& L);

	virtual void Draw(sf::RenderWindow& window)override;

	std::string TestFunction(void) { return "This is TestActor.This Function Needs to be used check if this class is in lua"; }
	~CTestActor();
};


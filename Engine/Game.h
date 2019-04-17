#pragma once
#include "PhysicalObject.h"
#include "Character.h"
#include "SolidBlock.h"
#include "TestPlayer.h"

using namespace std;
#include <include/lighting/LightSystem.h>
#include <chipmunk.h>

//class that manages all of the operations in game
class Game
{
	//Draws everything on screen
	void Render();

	//Handles events
	void ProccessEvents();

	//Updates every CObject
	void Update(sf::Time dt);


	std::string path;

	std::vector<std::shared_ptr<Engine::CActor>>SceneActors;


	sf::Texture devOrange64_64;

	bool ShowGravityUI = false;

	bool m = false;

	cpSpace*space;

	bool mLeft = false;

	bool mRight = false;

	static cpBool OnBeginCollision(cpArbiter* arb, cpSpace* space, cpDataPointer userData)
	{
		try
		{
			cpBody* bodyA;
			cpBody* bodyB;

			cpArbiterGetBodies(arb, &bodyA, &bodyB);

			static_cast<Engine::CActor*>(cpBodyGetUserData(bodyA))->OnBeginCollision(arb, static_cast<Engine::CActor*>(cpBodyGetUserData(bodyB)));

			static_cast<Engine::CActor*>(cpBodyGetUserData(bodyB))->OnBeginCollision(arb, static_cast<Engine::CActor*>(cpBodyGetUserData(bodyA)));

			//If objects are not sensors postSolve() &  preSolve() must be called
			return cpTrue;
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	static void OnEndCollision(cpArbiter* arb, cpSpace* space, cpDataPointer userData)
	{
		try
		{
			cpBody* bodyA;
			cpBody* bodyB;

			cpArbiterGetBodies(arb, &bodyA, &bodyB);

			static_cast<Engine::CActor*>(cpBodyGetUserData(bodyA))->OnEndCollision(arb, static_cast<Engine::CActor*>(cpBodyGetUserData(bodyB)));

			static_cast<Engine::CActor*>(cpBodyGetUserData(bodyB))->OnEndCollision(arb, static_cast<Engine::CActor*>(cpBodyGetUserData(bodyA)));

			
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
	}
public:
	//Init widnow etc.
	void Init();
	

	void Run();

	//No Get/Set due to SFML restrictions
	sf::RenderWindow window;

	/*
	Path to all files relative to bin
	e.g. ./../ is relative path to the main folder
	. /../scripts is path to folder with lua files
	*/
	Game(std::string WindowName, sf::VideoMode videoMode, std::string path);
	~Game();
};


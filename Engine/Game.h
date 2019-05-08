#pragma once
#include "PhysicalObject.h"
#include "Character.h"
#include "SolidBlock.h"
#include "TestPlayer.h"
#include "SoundContainer.h"

using namespace std;
#include <include/lighting/LightSystem.h>
#include <chipmunk.h>

#ifndef CLASS_CTEXTUREHANDLER
#include "TextureContainer.h"
#endif // !CLASS_CTEXTUREHANDLER

#ifndef _FMOD_HPP
#include <fmod.hpp>
#endif

#ifndef _FMOD_ERRORS_H
#include <fmod_errors.h>
#endif


//class that manages all of the operations in game
class Game
{

	sf::Shader unshadowShader;
	sf::Shader lightOverShapeShader;

	sf::Texture penumbraTexture;

	sf::Texture pointLightTexture;
	//Draws everything on screen
	void Render();

	//Handles events
	void ProccessEvents();

	//Updates every CObject
	void Update(sf::Time dt);


	std::string path;

	//std::vector<std::shared_ptr<Engine::CActor>>SceneActors;

	

	float time = 0.f;

	bool ShowGravityUI = false;

	bool ShowDebugSpawner = false;

	float DebugMass = 100.f;

	bool SpawnPhys = false;

	bool m = false;

	//cpSpace*space;

	bool mLeft = false;

	bool mRight = false;

	

	static cpBool OnBeginCollision(cpArbiter* arb, cpSpace* space, cpDataPointer userData)
	{
		try
		{
			cpBody* bodyA;
			cpBody* bodyB;

			cpArbiterGetBodies(arb, &bodyA, &bodyB);

			if (cpBodyGetUserData(bodyB) != nullptr && cpBodyGetUserData(bodyA) != nullptr)
			{
				static_cast<Engine::CActor*>(cpBodyGetUserData(bodyA))->OnBeginCollision(arb, static_cast<Engine::CActor*>(cpBodyGetUserData(bodyB)));

				static_cast<Engine::CActor*>(cpBodyGetUserData(bodyB))->OnBeginCollision(arb, static_cast<Engine::CActor*>(cpBodyGetUserData(bodyA)));
			}

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


	//FMOD::System* lowLevelSoundSystem = NULL;
	int texture_id = 0;

	bool isUsingMenu = false;
	
	//part of LoadMap
	void UnloadMap();

	//Part of the LoadMap
	void LoadMapFromFile(std::string name);
public:
	std::shared_ptr<Engine::Context>GameContext;

	//array of "Engine"-default sounds 
	//they can be used for testing or something else
	std::unique_ptr < Engine::Resources::Sound::CSoundContainer> Sounds;

	std::unique_ptr<Engine::Resources::Materials::CTextureContainer> TextureResources;

	//Init widnow etc.
	void Init();
	

	void Run();

	//No Get/Set due to SFML restrictions
	sf::RenderWindow window;

	//Clear data for the current map and load new one
	void LoadMap(std::string name);

	/*
	Path to all files relative to bin
	e.g. ./../ is relative path to the main folder
	. /../scripts is path to folder with lua files
	*/
	Game(std::string WindowName, sf::VideoMode videoMode, std::string path);
	~Game();
};


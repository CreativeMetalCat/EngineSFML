#include "Game.h"
#include "PhysicalObject.h"



void Game::Render()
{
	

	window.clear(sf::Color::Blue);
	sf::Font calibri;
	if (calibri.loadFromFile(path + "fonts/calibri.ttf"))
	{
		sf::Text t = sf::Text("Hello", calibri);
		if (!SceneActors.empty())
		{
			t.setPosition(SceneActors.at(0)->As<PhysicalObject*>()->GetActorLocation());
		}
		window.draw(t);
	}
	else
	{
		std::cout << "Failed to load font" << std::endl;
	}
	window.display();

	
}

void Game::ProccessEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.key.code == sf::Keyboard::A&&event.type == sf::Event::EventType::KeyPressed)
		{
			using namespace luabridge;
			if (!SceneActors.empty())
			{
				
				lua_State* L = luaL_newstate();


				std::string d = (path + "scripts/actor.lua");
				try
				{

					luaL_dofile(L, d.c_str());
					luaL_openlibs(L);

					lua_pcall(L, 0, 0, 0);

					//Register Actor in lua
					Actor::RegisterClassLUA(L);

					//Register Vector2 in lua
					getGlobalNamespace(L)
						.beginClass<sf::Vector2f>("Vector2")
						//add x,y and some functions possibly
						.addData<float>("x", &sf::Vector2<float>::x)
						.addData<float>("y", &sf::Vector2<float>::y)
						.addConstructor<void(*) (void)>()
						.endClass();


					LuaRef LuaSetActorLocation = getGlobal(L, "SetActorLocation");
					LuaRef LuaMoveX = getGlobal(L, "MoveX");
					LuaRef LuaAddChild = getGlobal(L, "AddChild");
					LuaRef LuaGetChild = getGlobal(L, "GetChild");



					LuaMoveX(&(*SceneActors.at(0)), -1);
				}
				catch (LuaException e)
				{
					std::cout << e.what() << std::endl;
				}

				catch (std::exception e)
				{
					std::cout << e.what() << std::endl;
				}
			}
			
		}
		if (event.key.code == sf::Keyboard::D&&event.type == sf::Event::EventType::KeyPressed)
		{
			using namespace luabridge;
			if (!SceneActors.empty())
			{

				lua_State* L = luaL_newstate();


				std::string d = (path + "scripts/actor.lua");
				try
				{

					luaL_dofile(L, d.c_str());
					luaL_openlibs(L);

					lua_pcall(L, 0, 0, 0);

					//Register Actor in lua
					Actor::RegisterClassLUA(L);

					//Register Vector2 in lua
					getGlobalNamespace(L)
						.beginClass<sf::Vector2f>("Vector2")
						//add x,y and some functions possibly
						.addData<float>("x", &sf::Vector2<float>::x)
						.addData<float>("y", &sf::Vector2<float>::y)
						.addConstructor<void(*) (void)>()
						.endClass();


					LuaRef LuaSetActorLocation = getGlobal(L, "SetActorLocation");
					LuaRef LuaMoveX = getGlobal(L, "MoveX");
					LuaRef LuaAddChild = getGlobal(L, "AddChild");
					LuaRef LuaGetChild = getGlobal(L, "GetChild");



					LuaMoveX(&(*SceneActors.at(0)), 1);
				}
				catch (LuaException e)
				{
					std::cout << e.what() << std::endl;
				}

				catch (std::exception e)
				{
					std::cout << e.what() << std::endl;
				}
			}
		}
	}
}

void Game::Update(sf::Time dt)
{
	//create lua state  for Game's own scripts
	lua_State* L = luaL_newstate();
}

void Game::Init()
{
	std::shared_ptr<PhysicalObject> po = std::make_shared<PhysicalObject>(sf::Vector2f(0, 0), "Wooden_Crate");
	po->Init(path);
	SceneActors.push_back(po);
	int i = 0;
}

void Game::Run()
{
	sf::Clock clock;

	while (window.isOpen())
	{

		window.clear(sf::Color::Black);
		sf::Time dt = clock.restart();
		ProccessEvents();
		Update(dt);
		window.clear(sf::Color::Black);
		Render();
	}
}

Game::Game(std::string WindowName, sf::VideoMode videoMode,std::string path) :window(videoMode, WindowName),path(path), world(b2Vec2(0.f, -9.8f))
{
	world.SetContactListener(&contactListener);
}


Game::~Game()
{
}

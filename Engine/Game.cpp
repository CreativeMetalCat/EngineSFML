#include "Game.h"
#include "PhysicalObject.h"
#include "Character.h"
#include "SolidBlock.h"
#include "TestPlayer.h"



using namespace std;


void Game::Render()
{
	sf::Sprite LSprite;
	window.clear(sf::Color::White);
	
	
	sf::Shader unshadowShader;
	sf::Shader lightOverShapeShader;



	unshadowShader.loadFromFile(path + "ltbl/resources/unshadowShader.vert", path + "ltbl/resources/unshadowShader.frag");
	lightOverShapeShader.loadFromFile(path + "ltbl/resources/lightOverShapeShader.vert", path + "ltbl/resources/lightOverShapeShader.frag");


	sf::Texture penumbraTexture;
	penumbraTexture.loadFromFile(path+"ltbl/resources/penumbraTexture.png");
	penumbraTexture.setSmooth(true);

	sf::Texture pointLightTexture;
	pointLightTexture.loadFromFile(path + "ltbl/resources/pointLightTexture.png");
	pointLightTexture.setSmooth(true);

	ltbl::LightSystem ls;

	ls.create(sf::FloatRect(-1000.0f, -1000.0f, 1000.0f, 1000.0f), window.getSize(), penumbraTexture, unshadowShader, lightOverShapeShader);

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


	if (!SceneActors.empty())
	{
		for (size_t i = 0; i < SceneActors.size(); i++)
		{


			if (SceneActors.at(i)->As<CSolidBlock*>())
			{

				SceneActors.at(i)->Draw(this->window);
				std::shared_ptr<ltbl::LightShape> lightShape = std::make_shared<ltbl::LightShape>();

				lightShape->_shape = SceneActors.at(i)->As<CSolidBlock*>()->ShadowShape;

				lightShape->_renderLightOverShape = true;
				lightShape->_shape.setPosition(SceneActors.at(i)->GetActorLocation());
				ls.addShape(lightShape);


			}

		}
	}

	sf::ConvexShape shape1;

	
	shape1.setPointCount(4);
	shape1.setPoint(0, sf::Vector2f(0, 0));
	shape1.setPoint(1, sf::Vector2f(80, 0));
	shape1.setPoint(2, sf::Vector2f(80, 80));
	shape1.setPoint(3, sf::Vector2f(0, 80));
	shape1.setPosition(100, 100);
	shape1.setFillColor(sf::Color::Green);
	window.draw(shape1);

	sf::ConvexShape shape2;
	shape2.setPointCount(4);
	shape2.setPoint(0, sf::Vector2f(0, 0));
	shape2.setPoint(1, sf::Vector2f(80, 0));
	shape2.setPoint(2, sf::Vector2f(80, 80));
	shape2.setPoint(3, sf::Vector2f(0, 80));
	shape2.setPosition(100, 100);
	shape2.setFillColor(sf::Color::Green);
	window.draw(shape2);

	sf::RenderStates lightRenderStates;

	std::shared_ptr<ltbl::LightPointEmission> light = std::make_shared<ltbl::LightPointEmission>();

	

	light->_emissionSprite.setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));//������������� �������� ����� � �������� �������� ��������.
	light->_emissionSprite.setTexture(pointLightTexture);// ���� �������� ��������.
	light->_emissionSprite.setScale(sf::Vector2f(10, 10));// ������ ������� ��������.
	light->_emissionSprite.setColor(sf::Color::White);// ���� �����.
	light->_emissionSprite.setPosition(SceneActors.at(0)->As<PhysicalObject*>()->GetActorLocation());//������� �����.
	light->_sourceRadius = 10;//������ ��������� �����.�� ��������� 1.
	light->_shadowOverExtendMultiplier = 1;// ���������� ������������� ����(� ������� ��� ����������� ����). 
	
	ls.addLight(light);


	std::shared_ptr<ltbl::LightPointEmission> light1 = std::make_shared<ltbl::LightPointEmission>();



	light1->_emissionSprite.setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));//������������� �������� ����� � �������� �������� ��������.
	light1->_emissionSprite.setTexture(pointLightTexture);// ���� �������� ��������.
	light1->_emissionSprite.setScale(sf::Vector2f(10, 10));// ������ ������� ��������.
	light1->_emissionSprite.setColor(sf::Color::White);// ���� �����.
	light1->_emissionSprite.setPosition(SceneActors.at(1)->GetActorLocation());//������� �����.
	light1->_sourceRadius = 10;//������ ��������� �����.�� ��������� 1.
	light1->_shadowOverExtendMultiplier = 1;// ���������� ������������� ����(� ������� ��� ����������� ����). 

	ls.addLight(light1);

	std::shared_ptr<ltbl::LightShape> lightShape = std::make_shared<ltbl::LightShape>();
	lightShape->_shape.setPointCount(4);
	lightShape->_shape.setPoint(0, sf::Vector2f(0, 0));
	lightShape->_shape.setPoint(1, sf::Vector2f(80, 0));
	lightShape->_shape.setPoint(2, sf::Vector2f(80, 80));
	lightShape->_shape.setPoint(3, sf::Vector2f(0, 80));
	lightShape->_renderLightOverShape = false;
	lightShape->_shape.setPosition(100, 100);
	ls.addShape(lightShape);

	std::shared_ptr<ltbl::LightShape> lightShape2 = std::make_shared<ltbl::LightShape>();
	lightShape2->_shape.setPointCount(4);
	lightShape2->_shape.setPoint(0, sf::Vector2f(0, 0));
	lightShape2->_shape.setPoint(1, sf::Vector2f(80, 0));
	lightShape2->_shape.setPoint(2, sf::Vector2f(80, 80));
	lightShape2->_shape.setPoint(3, sf::Vector2f(0, 80));
	lightShape2->_renderLightOverShape = false;
	lightShape2->_shape.setPosition(100, 300);
	ls.addShape(lightShape2);

	sf::View view = window.getDefaultView();

	view.setCenter(sf::Vector2f(1024 * 0.5f, 720 * 0.5f));

	ls.render(view, unshadowShader, lightOverShapeShader);

	
	LSprite.setTexture(ls.getLightingTexture());
	lightRenderStates.blendMode = sf::BlendMultiply;
	//window.setView(window.getDefaultView());
	window.setView(view);
	window.draw(LSprite, lightRenderStates);
	
	



	sf::Color bgColor;
	float color[3] = { 0.f, 0.f, 0.f };

	
	
	ImGui::SFML::Render(window);
	window.display();

	
}

void Game::ProccessEvents()
{
	sf::Event event;
	

	while (window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
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

					//Register CActor in lua
					CActor::RegisterClassLUA(L);

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

			SceneActors.at(1)->As<Character*>()->MoveX(-1);

			mLeft = true;
			
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

					//Register CActor in lua
					CActor::RegisterClassLUA(L);

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

			SceneActors.at(1)->As<Character*>()->MoveX(1);	

			mRight= true;
		}
		if(event.key.code == sf::Keyboard::W&&event.type == sf::Event::EventType::KeyPressed)
		{
			SceneActors.at(1)->As<Character*>()->Jump();

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

					//Register CActor in lua
					CActor::RegisterClassLUA(L);

					//Register Vector2 in lua
					getGlobalNamespace(L)
						.beginClass<sf::Vector2f>("Vector2")
						//add x,y and some functions possibly
						.addData<float>("x", &sf::Vector2<float>::x)
						.addData<float>("y", &sf::Vector2<float>::y)
						.addConstructor<void(*) (void)>()
						.endClass();


					LuaRef LuaSetActorLocation = getGlobal(L, "SetActorLocation");
					LuaRef LuaMoveX = getGlobal(L, "MoveY");
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
		if (event.key.code == sf::Keyboard::S&&event.type == sf::Event::EventType::KeyPressed)
		{
			SceneActors.at(1)->As<Character*>()->MoveY(1);
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

					//Register CActor in lua
					CActor::RegisterClassLUA(L);

					//Register Vector2 in lua
					getGlobalNamespace(L)
						.beginClass<sf::Vector2f>("Vector2")
						//add x,y and some functions possibly
						.addData<float>("x", &sf::Vector2<float>::x)
						.addData<float>("y", &sf::Vector2<float>::y)
						.addConstructor<void(*) (void)>()
						.endClass();


					LuaRef LuaSetActorLocation = getGlobal(L, "SetActorLocation");
					LuaRef LuaMoveX = getGlobal(L, "MoveY");
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
		if (event.key.code == sf::Keyboard::Tilde&&event.type == sf::Event::EventType::KeyPressed)
		{
			this->ShowGravityUI = !this->ShowGravityUI;
			
		}
	
		if (event.key.code == sf::Keyboard::D&&event.type == sf::Event::EventType::KeyReleased)
		{
			mRight = false;
		}

		if (event.key.code == sf::Keyboard::D&&event.type == sf::Event::EventType::KeyReleased)
		{
			mLeft = false;
		}

		if (!mRight && !mLeft)
		{
			SceneActors[1]->As<Character*>()->StopXMovement();
		}
	}
}

void Game::Update(sf::Time dt)
{
	//create lua state  for Game's own scripts
	lua_State* L = luaL_newstate();	

	cpSpaceStep(space, 1 / dt.asSeconds());
	
	SceneActors.at(1)->Update(dt);

	
	std::cout << SceneActors.at(1)->GetActorLocation().y << std::endl;
	ImGui::SFML::Update(window, dt);
	if (ShowGravityUI)
	{
		ImGui::Begin("Debug Menu");

		ImGui::BeginChild("Gravity Settings", ImVec2(350, 100));

		cpVect gravity = cpSpaceGetGravity(space);

		float gravX = gravity.x;
		float gravY = gravity.y;
		if (ImGui::DragFloat("Gravity X", &gravX, 0.0001f))
		{
			cpSpaceSetGravity(space, cpv(gravX, gravY));
		}

		if (ImGui::DragFloat("Gravity Y", &gravY, 0.0001f))
		{
			cpSpaceSetGravity(space, cpv(gravX, gravY));
		}

		ImGui::EndChild();

		ImGui::BeginChild("Character Output Data");
		ImGui::Text(std::to_string(SceneActors.at(1)->GetActorLocation().y).c_str());
		ImGui::Text(std::to_string(SceneActors.at(1)->GetActorLocation().x).c_str());
		ImGui::EndChild();

		ImGui::End();
	}
	
}

void Game::Init()
{
	ImGui::CreateContext();
	ImGui::SFML::Init(window);

	this->window.setFramerateLimit(60.f);
	contactListener.path = path;
	

	std::shared_ptr<PhysicalObject> po = std::make_shared<PhysicalObject>(sf::Vector2f(0, 0),path, "Wooden_Crate");
	po->Init(path);
	SceneActors.push_back(po);
	sf::ConvexShape s;
	s.setPointCount(4);
	s.setPoint(0, { 0,0 });
	s.setPoint(1, { 50,0 });
	s.setPoint(2, { 50,50 });
	s.setPoint(3, { 0,50 });

	std::shared_ptr<Character> c = std::make_shared<Character>(s, sf::Vector2f(50,50 ), sf::Vector2f(300, -100), path);
	
	c->InitPhysBody(path,space);
	SceneActors.push_back(c);

	
	if (!devOrange64_64.loadFromFile(path + "textures/dev/dev_orange_64x64.png"))
	{
		std::cout << "Failed to load texture\n";
	}
	sf::ConvexShape dev64_64;
	dev64_64.setPointCount(4);
	dev64_64.setPoint(0, { 0,0 });
	dev64_64.setPoint(1, { 64,0 });
	dev64_64.setPoint(2, { 64,64 });
	dev64_64.setPoint(3, { 0,64 });

	for (int i = 0; i < 19; i++)
	{
		std::shared_ptr<CSolidBlock> sd = std::make_shared<CSolidBlock>(devOrange64_64, dev64_64, sf::Vector2f(64, 64), sf::Vector2f(i*64, 400), path);
		sd->Init(path);
		sd->InitPhysBody(path, this->space);

		SceneActors.push_back(sd);
	}
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
	ImGui::DestroyContext();
}



Game::Game(std::string WindowName, sf::VideoMode videoMode,std::string path) :window(videoMode, WindowName),path(path)
{
	

	// cpVect is a 2D vector and cpv() is a shortcut for initializing them.
	cpVect gravity = cpv(0, 0.000098f);

	// Create an empty space.
	space = cpSpaceNew();
	cpSpaceSetGravity(space, gravity);
	
}


Game::~Game()
{
	cpSpaceFree(space);
}

#include "Game.h"
#include "CPhysicsBox.h"



using namespace std;

#ifndef _RANDOM_
#include <random>

int m_get_random_number(int min, int max)
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(min, max); // define the range

	return distr(eng);
}
#endif // !_RANDOM_



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


	if (!GameContext->SceneActors.empty())
	{
		for (size_t i = 0; i < GameContext->SceneActors.size(); i++)
		{

			GameContext->SceneActors.at(i)->Draw(window);
			if (GameContext->SceneActors.at(i)->As<Engine::CSolidBlock*>())
			{
				std::shared_ptr<ltbl::LightShape> lightShape = std::make_shared<ltbl::LightShape>();

				lightShape->_shape = GameContext->SceneActors.at(i)->As<Engine::CSolidBlock*>()->ShadowShape;

				lightShape->_renderLightOverShape = true;
				lightShape->_shape.setPosition(GameContext->SceneActors.at(i)->GetActorLocation());
				ls.addShape(lightShape);

			}
			else if (GameContext->SceneActors.at(i)->As<CPhysicsBox*>())
			{
				std::shared_ptr<ltbl::LightShape> lightShape = std::make_shared<ltbl::LightShape>();

				lightShape->_shape = GameContext->SceneActors.at(i)->As<CPhysicsBox*>()->ShadowShape;

				lightShape->_renderLightOverShape = true;
				lightShape->_shape.setPosition(GameContext->SceneActors.at(i)->GetActorLocation());
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

	

	light->_emissionSprite.setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));//Устанавливаем источник света в середину текстуры свечения.
	light->_emissionSprite.setTexture(pointLightTexture);// Сама текстура свечения.
	light->_emissionSprite.setScale(sf::Vector2f(10, 10));// Размер области свечения.
	light->_emissionSprite.setColor(sf::Color::White);// Цвет света.
	light->_emissionSprite.setPosition(sf::Vector2f(100,100));//Позиция света.
	light->_sourceRadius = 10;//Радиус источника света.По умолчанию 1.
	light->_shadowOverExtendMultiplier = 1;// Умножитель отбрасываемой тени(в столько раз увеличиться тень). 
	
	ls.addLight(light);


	std::shared_ptr<ltbl::LightPointEmission> light1 = std::make_shared<ltbl::LightPointEmission>();



	light1->_emissionSprite.setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));//Устанавливаем источник света в середину текстуры свечения.
	light1->_emissionSprite.setTexture(pointLightTexture);// Сама текстура свечения.
	light1->_emissionSprite.setScale(sf::Vector2f(10, 10));// Размер области свечения.
	light1->_emissionSprite.setColor(sf::Color::White);// Цвет света.
	light1->_emissionSprite.setPosition(GameContext->SceneActors.at(1)->GetActorLocation());//Позиция света.
	light1->_sourceRadius = 10;//Радиус источника света.По умолчанию 1.
	light1->_shadowOverExtendMultiplier = 1;// Умножитель отбрасываемой тени(в столько раз увеличиться тень). 

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
		try
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.key.code == sf::Keyboard::Tilde && event.type == sf::Event::EventType::KeyPressed)
			{
				this->ShowDebugSpawner = !this->ShowDebugSpawner;
				this->ShowGravityUI = !this->ShowGravityUI;


			}

			if (!GameContext->SceneActors.empty())
			{
				/*for (auto& var : GameContext->SceneActors)
				{
					if (var != nullptr || var.use_count() != 0|| &(*var) != nullptr)
					{	
						var->HandleEvent(event, &(*this->GameContext));
					}
				}*/
				for (size_t i = 0; i < GameContext->SceneActors.size(); i++)
				{
					GameContext->SceneActors.at(i)->HandleEvent(event, &(*this->GameContext));
				}
			}
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

void Game::Update(sf::Time dt)
{

	//create lua state  for Game's own scripts
	lua_State* L = luaL_newstate();

	cpSpaceStep(GameContext->space, 1 / dt.asSeconds());


	if (!GameContext->SceneActors.empty())
	{
		for (size_t i = 0; i < GameContext->SceneActors.size(); i++)
		{
			GameContext->SceneActors.at(i)->Update(dt, &(*this->GameContext));
		}
	}


	ImGui::SFML::Update(window, dt);
	if (ShowGravityUI)
	{
		ImGui::Begin("Debug Menu");

		ImGui::BeginChild("Gravity Settings", ImVec2(350, 100));

		cpVect gravity = cpSpaceGetGravity(GameContext->space);

		float gravX = gravity.x * 100000.f;
		float gravY = gravity.y * 100000.f;
		if (ImGui::DragFloat("Gravity X", &gravX, 0.1f))
		{
			cpSpaceSetGravity(GameContext->space, cpv(gravX / 100000.f, gravY / 100000.f));
		}

		if (ImGui::DragFloat("Gravity Y", &gravY, 0.1f))
		{
			cpSpaceSetGravity(GameContext->space, cpv(gravX / 100000.f, gravY / 100000.f));
		}

		ImGui::EndChild();

		ImGui::BeginChild("Character Output Data");
		ImGui::Text(std::to_string(GameContext->SceneActors.at(1)->GetActorLocation().y).c_str());
		ImGui::Text(std::to_string(GameContext->SceneActors.at(1)->GetActorLocation().x).c_str());
		ImGui::EndChild();
		ImGui::End();


	}
	if (ShowDebugSpawner)
	{
		
		std::vector<sf::FloatRect>rects;
		for (int x = 0; x < window.getSize().x / 64; x++)
		{
			for (int y = 0; y < window.getSize().y / 64; y++)
			{
				rects.push_back(sf::FloatRect(x * 64, y * 64, 64, 64));
			}
		}
		
		
		ImGui::Begin("Object Spawner");
		for (size_t i = 0; i < TextureResources->Textures.size(); i++)
		{
			ImGuiStyle style;
			style.FrameBorderSize = 0.f;
			if (ImGui::ImageButton(sf::Sprite(TextureResources->Textures[i]->m_texture)))
			{
				texture_id = i;
			}			
		}
		ImGui::Checkbox("Spawn Physics?", &this->SpawnPhys);
		//if (this->SpawnPhys) { ImGui::DragFloat("Mass", &this->DebugMass, 0.5f, 0.5f); }
		this->isUsingMenu = ImGui::IsMouseHoveringAnyWindow();
		if (ImGui::IsMouseClicked(sf::Mouse::Button::Left)&&!ImGui::IsMouseHoveringAnyWindow()&&!ImGui::IsAnyItemHovered())
		{
			
			
			if (texture_id > -1 && texture_id < TextureResources->Textures.size())
			{
				if (!SpawnPhys)
				{
					sf::ConvexShape dev64_64;
					dev64_64.setPointCount(4);
					dev64_64.setPoint(0, { 0,0 });
					dev64_64.setPoint(1, { 64,0 });
					dev64_64.setPoint(2, { 64,64 });
					dev64_64.setPoint(3, { 0,64 });

					for (int i = 0; i < rects.size(); i++)
					{
						if (rects[i].contains(sf::Vector2f(ImGui::GetMousePos().x, ImGui::GetMousePos().y)))
						{
							std::shared_ptr<Engine::CSolidBlock> sd = std::make_shared<Engine::CSolidBlock>(sf::Sprite(TextureResources->Textures[texture_id]->m_texture), dev64_64, sf::Vector2f(64, 64), sf::Vector2f(rects[i].left, rects[i].top), &(*this->GameContext), path);
							sd->Init(path, &(*this->GameContext));
							sd->InitPhysBody(path, this->GameContext->space);

							GameContext->SceneActors.push_back(sd);
							break;
						}
					}
				}
				else
				{
					sf::ConvexShape dev64_64;
					dev64_64.setPointCount(4);
					dev64_64.setPoint(0, { 0,0 });
					dev64_64.setPoint(1, { 64,0 });
					dev64_64.setPoint(2, { 64,64 });
					dev64_64.setPoint(3, { 0,64 });

					for (int i = 0; i < rects.size(); i++)
					{
						if (rects[i].contains(sf::Vector2f(ImGui::GetMousePos().x, ImGui::GetMousePos().y)))
						{
							std::shared_ptr<CPhysicsBox> po = std::make_shared<CPhysicsBox>(sf::Sprite(TextureResources->Textures[texture_id]->m_texture), sf::Vector2f(64, 64),sf::Vector2f(rects[i].left, rects[i].top),path, &(*this->GameContext), this->DebugMass, "Wooden_Crate");
							po->Init(path, &(*this->GameContext));
							po->InitPhysBody(path, this->GameContext->space);
							GameContext->SceneActors.push_back(po);
						}
					}
				}
			}
		}

		ImGui::End();
		rects.~vector();
	}
	

	this->GameContext->lowLevelSoundSystem->update();
}

void Game::Init()
{
	try
	{
		ImGui::CreateContext();
		ImGui::SFML::Init(window);

		this->window.setFramerateLimit(60.f);


		
		sf::ConvexShape s;
		s.setPointCount(4);
		s.setPoint(0, { 0,0 });
		s.setPoint(1, { 50,0 });
		s.setPoint(2, { 50,50 });
		s.setPoint(3, { 0,50 });
		
		std::shared_ptr<Engine::Character> c = std::make_shared<Engine::Character>(s, sf::Vector2f(64, 64), sf::Vector2f(200, 100), &(*this->GameContext), path);

		c->InitPhysBody(path, GameContext->space);
		GameContext->SceneActors.push_back(c);

		TextureResources->Init(path);


		sf::ConvexShape dev64_64;
		dev64_64.setPointCount(4);
		dev64_64.setPoint(0, { 0,0 });
		dev64_64.setPoint(1, { 64,0 });
		dev64_64.setPoint(2, { 64,64 });
		dev64_64.setPoint(3, { 0,64 });

		
		std::shared_ptr<CTestPlayer> player = std::make_shared<CTestPlayer>(sf::Sprite(TextureResources->GetTextureByName("dev64_orange")->GetTexture()), s, sf::Vector2f(64, 64), sf::Vector2f(300, 0), &(*this->GameContext), path);
		player->InitPhysBody(path, GameContext->space);
		player->ControlledByPlayer = true;
		GameContext->SceneActors.push_back(player);

		for (int i = 0; i < 19; i++)
		{
			
			std::shared_ptr<Engine::CSolidBlock> sd = std::make_shared<Engine::CSolidBlock>(sf::Sprite(TextureResources->GetTextureByName("dev64_blue")->GetTexture()), dev64_64, sf::Vector2f(64, 64), sf::Vector2f(i * 64, 400), &(*this->GameContext), path);
			sd->Init(path, &(*this->GameContext));
			sd->InitPhysBody(path, this->GameContext->space);

			GameContext->SceneActors.push_back(sd);
		}


		if (!GameContext->SceneActors.empty())
		{
			for (size_t i = 0; i < GameContext->SceneActors.size(); i++)
			{
				//path must be given here due to limitations of the chipmunk2D engine
				GameContext->SceneActors.at(i)->Init(path, &(*this->GameContext));
			}
		}


		if (!GameContext->Sounds->Sounds.empty())
		{
			for (auto var : GameContext->Sounds->Sounds)
			{
				std::string d = path + var->NameOfFile;
				FMOD_RESULT res = GameContext->lowLevelSoundSystem->createSound(d.c_str(), FMOD_2D, 0, &var->m_sound);
				if (res != FMOD_RESULT::FMOD_OK)
				{
					std::cout << FMOD_ErrorString(res) << std::endl;
				}
				else
				{
					
				}
			}
		}
		
		
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
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
	
	GameContext = std::make_shared<Engine::Context>(path);
	
	
	cpCollisionHandler*handler = cpSpaceAddDefaultCollisionHandler(GameContext->space);
	handler->beginFunc = &Game::OnBeginCollision;
	handler->separateFunc = &Game::OnEndCollision;

	TextureResources = std::make_unique<Engine::Resources::Materials::CTextureContainer>(path);

	Sounds = std::make_unique<Engine::Resources::Sound::CSoundContainer>(path);

	
	
}


Game::~Game()
{
	TextureResources.release();
	cpSpaceFree(GameContext->space);
}

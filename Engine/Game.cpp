#include "Game.h"
#include "CPhysicsBox.h"
#include "FuncElevator.h"


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
	

	ltbl::LightSystem ls;
	ls.create(sf::FloatRect(-window.getSize().x, -window.getSize().y , window.getSize().x , window.getSize().y ), window.getSize(), penumbraTexture, unshadowShader, lightOverShapeShader);

	std::vector< std::shared_ptr<ltbl::LightShape>>lightShapes;

	std::vector< std::shared_ptr<ltbl::LightPointEmission>>pointLights;

	if (!GameContext->SceneActors.empty())
	{
		for (size_t i = 0; i < GameContext->SceneActors.size(); i++)
		{

			GameContext->SceneActors.at(i)->Draw(window);
			if (GameContext->SceneActors.at(i)->As<Engine::CSolidBlock*>())
			{
				lightShapes.push_back(std::make_shared<ltbl::LightShape>());

				lightShapes.at(lightShapes.size()-1)->_shape = GameContext->SceneActors.at(i)->As<Engine::CSolidBlock*>()->ShadowShape;

				lightShapes.at(lightShapes.size() - 1)->_renderLightOverShape = true;
				lightShapes.at(lightShapes.size() - 1)->_shape.setPosition(GameContext->SceneActors.at(i)->GetActorLocation());
				ls.addShape(lightShapes.at(lightShapes.size() - 1));

			}
			else if (GameContext->SceneActors.at(i)->As<CPhysicsBox*>())
			{
				lightShapes.push_back(std::make_shared<ltbl::LightShape>());

				lightShapes.at(lightShapes.size() - 1)->_shape = GameContext->SceneActors.at(i)->As<CPhysicsBox*>()->ShadowShape;

				lightShapes.at(lightShapes.size() - 1)->_renderLightOverShape = true;
				lightShapes.at(lightShapes.size() - 1)->_shape.setPosition(GameContext->SceneActors.at(i)->GetActorLocation());
				ls.addShape(lightShapes.at(lightShapes.size() - 1));
			}

		}
	}

	sf::RenderStates lightRenderStates;

	pointLights.push_back(std::make_shared<ltbl::LightPointEmission>());

		

	pointLights.at(pointLights.size() - 1)->_emissionSprite.setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));//Устанавливаем источник света в середину текстуры свечения.
	pointLights.at(pointLights.size() - 1)->_emissionSprite.setTexture(pointLightTexture);// Сама текстура свечения.
	pointLights.at(pointLights.size() - 1)->_emissionSprite.setScale(sf::Vector2f(10, 10));// Размер области свечения.
	pointLights.at(pointLights.size() - 1)->_emissionSprite.setColor(sf::Color::White);// Цвет света.
	pointLights.at(pointLights.size() - 1)->_emissionSprite.setPosition(sf::Vector2f(600,300));//Позиция света.
	pointLights.at(pointLights.size() - 1)->_sourceRadius = 10;//Радиус источника света.По умолчанию 1.
	pointLights.at(pointLights.size() - 1)->_shadowOverExtendMultiplier = 1;// Умножитель отбрасываемой тени(в столько раз увеличиться тень). 
	
	ls.addLight(pointLights.at(pointLights.size() - 1));


	pointLights.push_back(std::make_shared<ltbl::LightPointEmission>());




	pointLights.at(pointLights.size() - 1)->_emissionSprite.setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));//Устанавливаем источник света в середину текстуры свечения.
	pointLights.at(pointLights.size() - 1)->_emissionSprite.setTexture(pointLightTexture);// Сама текстура свечения.
	pointLights.at(pointLights.size() - 1)->_emissionSprite.setScale(sf::Vector2f(10, 10));// Размер области свечения.
	pointLights.at(pointLights.size() - 1)->_emissionSprite.setColor(sf::Color::White);// Цвет света.
	pointLights.at(pointLights.size() - 1)->_emissionSprite.setPosition(GameContext->SceneActors.at(1)->GetActorLocation());//Позиция света.
	pointLights.at(pointLights.size() - 1)->_sourceRadius = 10;//Радиус источника света.По умолчанию 1.
	pointLights.at(pointLights.size() - 1)->_shadowOverExtendMultiplier = 1;// Умножитель отбрасываемой тени(в столько раз увеличиться тень). 

	ls.addLight(pointLights.at(pointLights.size() - 1));


	sf::View view = window.getDefaultView();

	view.setCenter(sf::Vector2f(window.getSize().x * 0.5f, window.getSize().y * 0.5f));
	

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

	for (auto& lightShape : lightShapes)
	{
		ls.removeShape(lightShape);
	}

	for (auto& pointLight : pointLights)
	{
		ls.removeLight(pointLight);
	}
	
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
					GameContext->SceneActors.at(i)->HandleEvent(event);
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

	if (GameContext->MapAction.GetCurrentAction() == Engine::MapLoading::MapAction::Load)
	{
		if (GameContext->MapAction.GetCurrentMapName() != "")
		{
			this->LoadMapFromFile(GameContext->MapAction.GetCurrentMapName());
			GameContext->MapAction.m_loaded = true;
			GameContext->MapAction.m_current_action = Engine::MapLoading::MapAction::None;
		}
	}
	else if(GameContext->MapAction.GetCurrentAction() == Engine::MapLoading::MapAction::UnLoad)
	{
		if (GameContext->MapAction.m_loaded)
		{
			this->UnloadMap();
			GameContext->MapAction.m_current_action = Engine::MapLoading::MapAction::None;
		}
	}
	else if (GameContext->MapAction.GetCurrentAction() == Engine::MapLoading::MapAction::Change)
	{
		if (GameContext->MapAction.GetCurrentMapName() != "")
		{
			this->LoadMap(GameContext->MapAction.GetCurrentMapName());
			GameContext->MapAction.m_current_action = Engine::MapLoading::MapAction::None;
		}
	}

	if (!GameContext->SceneActors.empty())
	{
		for (size_t i = 0; i < GameContext->SceneActors.size(); i++)
		{
			GameContext->SceneActors.at(i)->Update(dt);
			if (GameContext->SceneActors.at(i)->IsPendingKill())
			{
				GameContext->SceneActors.at(i)->Release();
				GameContext->SceneActors.at(i).~shared_ptr();
				this->GameContext->SceneActors.erase
				(
					std::find
					(
						this->GameContext->SceneActors.begin(),
						this->GameContext->SceneActors.end(),
						GameContext->SceneActors.at(i)
					)
				);


			}
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
							sd->Init(path);
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
							po->Init(path);
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

void Game::UnloadMap()
{
	cpSpaceStep(GameContext->space, 0.f);

	if (!GameContext->SceneActors.empty())
	{
		for (size_t i = 0; i < GameContext->SceneActors.size(); i++)
		{

			GameContext->SceneActors.at(i)->Release();
			GameContext->SceneActors.at(i).~shared_ptr();
			this->GameContext->SceneActors.erase
			(
				std::find
				(
					this->GameContext->SceneActors.begin(),
					this->GameContext->SceneActors.end(),
					GameContext->SceneActors.at(i)
				)
			);
		}
	}
	
	GameContext->MapAction.m_loaded = false;
}

void Game::LoadMapFromFile(std::string name)
{
	
	try
	{
		//create lua state that will be used to load map
		lua_State* L = luaL_newstate();

		std::string d = (path + "maps/" + name);

		luaL_openlibs(L);
		luaL_dofile(L, d.c_str());


		lua_pcall(L, 0, LUA_MULTRET, 0);

		luabridge::LuaRef actorsTable = luabridge::getGlobal(L, "map");
		if (!actorsTable.isNil() && actorsTable.isTable())
		{
			//in lua firts index of the array is 1
			for (int i = 1; i <= actorsTable.length(); i++)
			{
				//we store actors as tables in .map files
				if (!actorsTable[i].isNil() && actorsTable[i].isTable())
				{
					std::string textureName;
					sf::ConvexShape cs;
					sf::Vector2f size;
					sf::Vector2f pos;
					
					if (actorsTable[i]["type_id"] == CLASS_SOLIDBLOCK)
					{

						if (!actorsTable[i]["texture_name"].isNil())
						{
							textureName = actorsTable[i]["texture_name"].cast<std::string>();
						}
						if (!actorsTable[i]["shadow_shape"].isNil() && actorsTable[i]["shadow_shape"].isTable())
						{
							cs.setPointCount(actorsTable[i]["shadow_shape"]["point_count"].cast<int>());

							for (int u = 1; u <= actorsTable[i]["shadow_shape"]["point_count"].cast<int>(); u++)
							{
								cs.setPoint
								(u - 1u,
									sf::Vector2f
									(actorsTable[i]["shadow_shape"][u]["x"].cast<int>(),
										actorsTable[i]["shadow_shape"][u]["y"].cast<int>()
									)
								);

							}
						}
						if (!actorsTable[i]["size"].isNil() && actorsTable[i]["size"].isTable())
						{
							size.x = actorsTable[i]["size"]["x"].cast<float>();
							size.y = actorsTable[i]["size"]["y"].cast<float>();
						}
						if (!actorsTable[i]["location"].isNil() && actorsTable[i]["location"].isTable())
						{
							pos.x = actorsTable[i]["location"]["x"].cast<float>();
							pos.y = actorsTable[i]["location"]["y"].cast<float>();
						}

						std::shared_ptr<Engine::CSolidBlock> sd = std::make_shared<Engine::CSolidBlock>(sf::Sprite(TextureResources->GetTextureByName(textureName)->GetTexture()), cs, size, pos, &(*this->GameContext), path);
						sd->Init(path);
						sd->InitPhysBody(path, this->GameContext->space);

						GameContext->SceneActors.push_back(sd);

					}
					else if (actorsTable[i]["type_id"] == CLASS_PHYSICSBOX)
					{

						float mass = 100.f;
						std::string material_name = "";
						std::cout << "Solid" << std::endl;

						if (!actorsTable[i]["texture_name"].isNil())
						{
							textureName = actorsTable[i]["texture_name"].cast<std::string>();
						}
						if (!actorsTable[i]["shadow_shape"].isNil() && actorsTable[i]["shadow_shape"].isTable())
						{
							cs.setPointCount(actorsTable[i]["shadow_shape"]["point_count"].cast<int>());

							for (int u = 1; u <= actorsTable[i]["shadow_shape"]["point_count"].cast<int>(); u++)
							{
								cs.setPoint
								(u - 1u,
									sf::Vector2f
									(actorsTable[i]["shadow_shape"][u]["x"].cast<int>(),
										actorsTable[i]["shadow_shape"][u]["y"].cast<int>()
									)
								);

							}
						}
						if (!actorsTable[i]["size"].isNil() && actorsTable[i]["size"].isTable())
						{
							size.x = actorsTable[i]["size"]["x"].cast<float>();
							size.y = actorsTable[i]["size"]["y"].cast<float>();
						}
						if (!actorsTable[i]["location"].isNil() && actorsTable[i]["location"].isTable())
						{
							pos.x = actorsTable[i]["location"]["x"].cast<float>();
							pos.y = actorsTable[i]["location"]["y"].cast<float>();
						}
						if (!actorsTable[i]["mass"].isNil() && actorsTable[i]["mass"].isNumber())
						{
							mass = actorsTable[i]["mass"].cast<float>();
						}
						if (!actorsTable[i]["material_name"].isNil() && actorsTable[i]["material_name"].isString())
						{
							material_name = actorsTable[i]["material_name"].cast<std::string>();
						}

						std::shared_ptr<CPhysicsBox> sd = std::make_shared<CPhysicsBox>(sf::Sprite(TextureResources->GetTextureByName(textureName)->GetTexture()), size, pos, path, &(*this->GameContext), mass, material_name);
						sd->Init(path);
						sd->InitPhysBody(path, this->GameContext->space);

						GameContext->SceneActors.push_back(sd);

					}
					else if (actorsTable[i]["type_id"] == CLASS_FUNCELEVATOR)
					{
						sf::Vector2f endLocation = sf::Vector2f(0, 0);
						if (!actorsTable[i]["texture_name"].isNil())
						{
							textureName = actorsTable[i]["texture_name"].cast<std::string>();
						}
						if (!actorsTable[i]["shadow_shape"].isNil() && actorsTable[i]["shadow_shape"].isTable())
						{
							cs.setPointCount(actorsTable[i]["shadow_shape"]["point_count"].cast<int>());

							for (int u = 1; u <= actorsTable[i]["shadow_shape"]["point_count"].cast<int>(); u++)
							{
								cs.setPoint
								(u - 1u,
									sf::Vector2f
									(actorsTable[i]["shadow_shape"][u]["x"].cast<int>(),
										actorsTable[i]["shadow_shape"][u]["y"].cast<int>()
									)
								);

							}
						}
						if (!actorsTable[i]["size"].isNil() && actorsTable[i]["size"].isTable())
						{
							size.x = actorsTable[i]["size"]["x"].cast<float>();
							size.y = actorsTable[i]["size"]["y"].cast<float>();
						}
						if (!actorsTable[i]["location"].isNil() && actorsTable[i]["location"].isTable())
						{
							pos.x = actorsTable[i]["location"]["x"].cast<float>();
							pos.y = actorsTable[i]["location"]["y"].cast<float>();
						}
						if (!actorsTable[i]["end_location"].isNil() && actorsTable[i]["end_location"].isTable())
						{
							endLocation.x = actorsTable[i]["end_location"]["x"].cast<float>();
							endLocation.y = actorsTable[i]["end_location"]["y"].cast<float>();
						}

						std::shared_ptr<Test::FuncElevator>elev = std::make_shared<Test::FuncElevator>(endLocation, sf::Sprite(TextureResources->GetTextureByName(textureName)->GetTexture()),size, pos, &(*this->GameContext), path);
						elev->Init(path);
						elev->InitPhysBody(path, GameContext->space);
						GameContext->SceneActors.push_back(elev);
					}
				}
				else
				{
					std::cout << "LUA Error: Found broken filed! Table " << "map" << " File:" << d << std::endl;
				}
			}
		}
		else
		{
			std::cout << "LUA Error: Found broken table! File:" << d << std::endl;
		}

		this->GameContext->MapAction.m_loaded = true;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (luabridge::LuaException e)
	{
		std::cout << e.what() << std::endl;
	}
	//close lua state
	//lua_close(L);
}

void Game::Init()
{
	try
	{
		
		unshadowShader.loadFromFile(path + "ltbl/resources/unshadowShader.vert", path + "ltbl/resources/unshadowShader.frag");
		lightOverShapeShader.loadFromFile(path + "ltbl/resources/lightOverShapeShader.vert", path + "ltbl/resources/lightOverShapeShader.frag");

		penumbraTexture.loadFromFile(path + "ltbl/resources/penumbraTexture.png");
		penumbraTexture.setSmooth(true);


		pointLightTexture.loadFromFile(path + "ltbl/resources/pointLightTexture.png");
		pointLightTexture.setSmooth(true);

		TextureResources->Init(path);
		ImGui::CreateContext();
		ImGui::SFML::Init(window);

		this->window.setFramerateLimit(60.f);

		this->GameContext->MapAction.m_current_action = Engine::MapLoading::MapAction::Load;
		this->GameContext->MapAction.m_name = "test/demo.map";
		this->GameContext->MapAction.m_loaded = false;

		//LoadMap("test/demo.map");
		
		sf::ConvexShape s;
		s.setPointCount(4);
		s.setPoint(0, { 0,0 });
		s.setPoint(1, { 50,0 });
		s.setPoint(2, { 50,50 });
		s.setPoint(3, { 0,50 });
		
		

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
			
			std::shared_ptr<Engine::CSolidBlock> sd = std::make_shared<Engine::CSolidBlock>(sf::Sprite(TextureResources->GetTextureByName("dev64_blue")->GetTexture()), dev64_64, sf::Vector2f(64, 64), sf::Vector2f(i * 64, 384), &(*this->GameContext), path);
			sd->Init(path);
			sd->InitPhysBody(path, this->GameContext->space);

			GameContext->SceneActors.push_back(sd);
		}


		if (!GameContext->SceneActors.empty())
		{
			for (size_t i = 0; i < GameContext->SceneActors.size(); i++)
			{
				//path must be given here due to limitations of the chipmunk2D engine
				GameContext->SceneActors.at(i)->Init(path);
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
	catch (luabridge::LuaException e)
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

void Game::LoadMap(std::string name)
{
	UnloadMap();
	//inbetween actions such as checking if path is valid or if world context is valid
	LoadMapFromFile(name);
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

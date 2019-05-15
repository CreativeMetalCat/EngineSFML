#include "TestPlayer.h"
#include <math.h>  


void CTestPlayer::SetSpriteTexture(sf::Texture & t)
{
}


CTestPlayer::CTestPlayer(sf::Sprite sprite,std::string texture_name, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location,Engine::Context* WorldContext, std::string path) :
	Character(CollisionShape, Size, Location,WorldContext, path),
	m_sprite(std::make_shared<Engine::Sprite>(sprite,texture_name))
{
	
	
	
	sf::Vector2f scale;

	if (m_sprite->GetSprite().getTexture()->getSize().x != 0) { scale.x = Size.x / m_sprite->GetSprite().getTexture()->getSize().x; }

	if (m_sprite->GetSprite().getTexture()->getSize().y != 0) { scale.y = Size.y / m_sprite->GetSprite().getTexture()->getSize().y; }

	m_sprite->GetSprite().setScale(scale);
	Weapon = std::make_shared<Gameplay::Weapon>("scripts/weapons/weapon.lua", this->WorldContext, this->path);
	

	m_point = { 64.f,0.f };
	
}

void CTestPlayer::Init(std::string path)
{
	try
	{
		Weapon->weaponSprite = std::make_shared<Engine::Sprite>(sf::Sprite(WorldContext->TextureResources->GetTextureByName("sniperrifle")->GetTexture()), "sniperrifle");
		
		Weapon->Init(path);
		this->path = path;

		sf::Vector2f scale;

		if (m_sprite->GetSprite().getTexture()->getSize().x != 0) { scale.x = Size.x / m_sprite->GetSprite().getTexture()->getSize().x; }

		if (m_sprite->GetSprite().getTexture()->getSize().y != 0) { scale.y = Size.y / m_sprite->GetSprite().getTexture()->getSize().y; }

		m_sprite->GetSprite().setScale(scale);

		/*if (this->testShader.loadFromFile(path + "shaders/testshader.vert", path + "shaders/testshader.frag"))
		{
			std::cout << "success\n";
		}*/
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	//this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
}

void CTestPlayer::InitPhysBody(std::string path, cpSpace*& world)
{
	try
	{
		//this->Init(path);
		std::vector<cpVect>points;
		for (int i = 0; i < ShadowShape.getPointCount(); i++)
		{
			points.push_back(cpv(ShadowShape.getPoint(i).x, ShadowShape.getPoint(i).x));
		}

		this->Body = cpBodyNew(100.f, cpMomentForBox(100.f, CollisionRectangle.width, CollisionRectangle.height));
		if (this->Body != nullptr)
		{
			
			//perform here actions that can happen only after body init


			shapes.push_back(cpBoxShapeNew(this->GetBody(), CollisionRectangle.width, CollisionRectangle.height, 0));
			
			cpSpaceAddBody(world, this->Body);

			cpBodySetUserData(Body, this);

			cpBodySetPosition(this->Body, cpv(this->GetActorLocation().x , this->GetActorLocation().y));
			
			for (int i = 0; i < shapes.size(); i++)
			{
				if (shapes.at(i) != nullptr)
				{
					cpSpaceAddShape(world, shapes[i]);
					//Prevent from bouncing 
					cpShapeSetElasticity(shapes[i], 0.0f);

					cpShapeSetMass(shapes[i], 100.f);


				}

			}
			this->SetActorLocation(sf::Vector2f(cpBodyGetPosition(Body).x, cpBodyGetPosition(Body).y));
		}
		
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
}

void CTestPlayer::Draw(sf::RenderWindow& window)
{
	/*if (WorldContext->ShaderResources->GetShaderByName("normal") != nullptr)
	{
		WorldContext->ShaderResources->GetShaderByName("normal")->Shader.setUniform("texture",this->m_sprite.getTexture());
		WorldContext->ShaderResources->GetShaderByName("normal")->Shader.setUniform("reflectMap", this->WorldContext->TextureResources->GetTextureByName("dev_64x64_refl")->m_texture);
		WorldContext->ShaderResources->GetShaderByName("normal")->Shader.setUniform("normalMap", this->WorldContext->TextureResources->GetTextureByName("dev_64x64_normal")->m_texture);
		WorldContext->ShaderResources->GetShaderByName("normal")->Shader.setUniform("specularMap", this->WorldContext->TextureResources->GetTextureByName("dev_64x64_spec")->m_texture);
		WorldContext->ShaderResources->GetShaderByName("normal")->Shader.setUniform("reflectOffset", this->Location);

		window.draw(m_sprite, &WorldContext->ShaderResources->GetShaderByName("normal")->Shader);
	}*/
	window.draw(m_sprite->GetSprite());

	if (Weapon != nullptr)
	{
		window.draw(Weapon->weaponSprite->m_sprite);
	}

	sf::VertexArray va;
	va.append(sf::Vertex(m_point + this->Location, sf::Color::Red));
	window.draw(va);
}

void CTestPlayer::Update(sf::Time dt)
{
	Anim->Update(dt);

	Weapon->weaponSprite->m_sprite.setPosition(this->Location);
	Weapon->weaponSprite->m_sprite.setRotation(m_angle);

	m_sprite->m_sprite = Anim->GetSprite();
	m_sprite->SetTextureName(Anim->m_spriteName);
	m_shader_dt += dt.asSeconds();
	
	if (this->Weapon != nullptr)
	{
		this->Weapon->Update(dt);
	}

	

	if (Body != nullptr)
	{

		cpBodySetAngle(Body, 0);
		this->Location.x = cpBodyGetPosition(this->GetBody()).x;
		this->Location.y = cpBodyGetPosition(this->GetBody()).y;

		m_sprite->GetSprite().setPosition(this->GetActorLocation());
	}
	if (m_moving_left || m_moving_right)
	{
		m_passed_footstep_time += dt.asSeconds();
		if (m_passed_footstep_time >= m_per_foostep_time)
		{
			m_passed_footstep_time = 0.f;
			WorldContext->lowLevelSoundSystem->playSound(WorldContext->Sounds->GetSoundByName("footstep_tile2")->m_sound, 0, false, &m_footstep_sound_channel);

			//Duplicate(sf::Vector2f(this->GetActorLocation().x, this->GetActorLocation().y - 100), true);

//-------------------------------------------------------------------------------------------------------------
			using namespace luabridge;
			try
			{
				lua_State* L = luaL_newstate();

				std::string d = (path + "scripts/testplayer/mainscript.lua");

				luaL_dofile(L, d.c_str());
				luaL_openlibs(L);

				lua_pcall(L, 0, 0, 0);

				CTestPlayer::RegisterClassLUA(L);
				
				//Register Vector2 in lua
				luabridge::getGlobalNamespace(L)
					.beginClass<sf::Vector2f>("Vector2")
					//add x,y and some functions possibly
					.addData<float>("x", &sf::Vector2<float>::x)
					.addData<float>("y", &sf::Vector2<float>::y)
					.addConstructor<void(*) (void)>()
					.endClass();

				luabridge::getGlobalNamespace(L)
					.beginClass<sf::Sprite>("Spite")

					.endClass();

				luabridge::getGlobalNamespace(L)
					.beginClass<sf::ConvexShape>("ConvexShape")

					.endClass();

				luabridge::getGlobalNamespace(L)
					.beginClass<Engine::Context>("WorldContext")

					.endClass();


				LuaRef SpawnPlayer = getGlobal(L, "SpawnPlayer");
				if (SpawnPlayer.isFunction())
				{
					//SpawnPlayer(this, this->GetSprite(), this->ShadowShape, this->Size, sf::Vector2f(this->GetActorLocation().x, this->GetActorLocation().y - 100), true, this->WorldContext, path);
				}
			}
			catch (LuaException e)
			{
				std::cout << e.what() << std::endl;
			}
			catch (std::exception e)
			{
				std::cout << e.what() << std::endl;
			}
//--------------------------------------------------------------------------------------------------------------------------

		}
	}

}

void CTestPlayer::OnBeginCollision(cpArbiter*& arb, CActor* otherActor)
{
	/*std::cout << "Body: Y:" << cpBodyGetPosition(this->Body).y << "X: " << cpBodyGetPosition(this->Body).x << std::endl;
	std::cout <<"Feet: Y:"<< cpBodyGetPosition(this->FeetBody).y << "X: "<< cpBodyGetPosition(this->FeetBody).x <<std::endl;*/
	cpBody* bodyA;
	cpBody* bodyB;
	cpArbiterGetBodies(arb, &bodyA, &bodyB);


	using namespace luabridge;
	lua_State* L = luaL_newstate();
	std::string d = (path + "scripts/character.lua");
	try
	{

		int status = luaL_dofile(L, d.c_str());
		if (status != 0)
		{
			fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		}
		luaL_openlibs(L);

		lua_pcall(L, 0, 0, 0);

		//Register this class in lua
		this->RegisterClassLUA(L);

		//register other CActor's class
		otherActor->RegisterClassLUA(L);



		//Register Vector2 in lua
		luabridge::getGlobalNamespace(L)
			.beginClass<sf::Vector2f>("Vector2")
			//add x,y and some functions possibly
			.addData<float>("x", &sf::Vector2<float>::x)
			.addData<float>("y", &sf::Vector2<float>::y)
			.addConstructor<void(*) (void)>()
			.endClass();


		luabridge::getGlobalNamespace(L)
			.beginClass<cpArbiter>("cpArbiter")

			.endClass();

		LuaRef LUAOnBeginCollision = getGlobal(L, "OnBeginCollision");
		if (LUAOnBeginCollision.isFunction())
		{
			LUAOnBeginCollision(this, arb, &(*otherActor));
		}
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


void CTestPlayer::RegisterClassLUA(lua_State*& L)
{
	using namespace luabridge;
	try
	{
		//Register CActor in lua
		luabridge::getGlobalNamespace(L)
			.beginClass<CTestPlayer>("CTestPlayer")
			//.addConstructor<void(*) (sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, std::string path)>()

			.addProperty("Location", &CTestPlayer::GetActorLocation, &CTestPlayer::SetActorLocation)
			.addProperty("PhysBodyInitialized", &CTestPlayer::GetPhysBodyInitialized, &CTestPlayer::SetPhysBodyInitialized)
			.addProperty("AreaId", &CTestPlayer::GetAreaId, &CTestPlayer::SetAreaId)
			.addProperty("MaxVelocity", &CTestPlayer::GetMaxVelocity, &CTestPlayer::SetMaxVelocity)

			.addFunction("AddChildRaw", &CTestPlayer::AddChildRaw)
			.addFunction("GetChild", &CTestPlayer::GetChild)
			.addFunction("MoveX", &CTestPlayer::MoveX)
			.addFunction("ApplyLinearImpulse", &CTestPlayer::ApplyLinearImpulse)
			.addFunction("GetLinearVelocity", &CTestPlayer::GetLinearVelocity)
			.addFunction("GetClassID", &CTestPlayer::GetClassID)

			.addStaticFunction("Duplicate", &CTestPlayer::Duplicate)

			.addData<bool>("IsMovingX", &CTestPlayer::IsMovingX)
			.addData<bool>("IsMovingY", &CTestPlayer::IsMovingY)
			.addData<std::string>("Path",&CTestPlayer::path)
			
			

			.endClass();
	}
	catch (LuaException e)
	{
		std::cout << "Failed to register class in LUA" << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << "Failed to register class in LUA " << e.what() << std::endl;
	}
}

void CTestPlayer::Duplicate(sf::Sprite sprite, std::string texture_name, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f newLocation,bool PlayerControlled, Engine::Context* WorldContext,std::string path)
{
	WorldContext->AddActor
	(
		new CTestPlayer
		(
			sprite,
			texture_name,
			CollisionShape,
			Size,
			newLocation,
			WorldContext,
			path
		)
	);
	WorldContext->SceneActors.at(WorldContext->SceneActors.size() - 1)->Init(path);
	WorldContext->SceneActors.at(WorldContext->SceneActors.size() - 1)->InitPhysBody(path, WorldContext->space);
	WorldContext->SceneActors.at(WorldContext->SceneActors.size() - 1)->As<CTestPlayer*>()->ControlledByPlayer = PlayerControlled;

}

void CTestPlayer::Shoot()
{
	using namespace luabridge;
	lua_State* L = luaL_newstate();
	std::string d = (path + "scripts/testplayer/mainscript.lua");
	try
	{
		Weapon->Shoot(this->m_sprite->GetSprite(), m_point + this->GetActorLocation(), m_angle);
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


void CTestPlayer::OnEndCollision(cpArbiter*& arb, CActor* otherActor)
{
	cpBody* bodyA;
	cpBody* bodyB;
	cpArbiterGetBodies(arb, &bodyA, &bodyB);

	using namespace luabridge;
	lua_State* L = luaL_newstate();
	std::string d = (path + "scripts/character.lua");
	try
	{

		int status = luaL_dofile(L, d.c_str());
		if (status != 0)
		{
			fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		}
		luaL_openlibs(L);

		lua_pcall(L, 0, 0, 0);

		//Register this class in lua
		this->RegisterClassLUA(L);

		//register other CActor's class
		otherActor->RegisterClassLUA(L);

		//Register Vector2 in lua
		luabridge::getGlobalNamespace(L)
			.beginClass<sf::Vector2f>("Vector2")
			//add x,y and some functions possibly
			.addData<float>("x", &sf::Vector2<float>::x)
			.addData<float>("y", &sf::Vector2<float>::y)
			.addConstructor<void(*) (void)>()
			.endClass();

		luabridge::getGlobalNamespace(L)
			.beginClass<cpArbiter>("cpArbiter")

			.endClass();

		LuaRef LUAOnEndCollision = getGlobal(L, "OnEndCollision");
		if (LUAOnEndCollision.isFunction())
		{
			LUAOnEndCollision(this, arb, &(*otherActor));
		}
	}
	catch (LuaException e)
	{
		std::cout << e.what() << std::endl;
	}

}

void CTestPlayer::HandleEvent(sf::Event event)
{
	if (ControlledByPlayer)
	{

		if (event.type == sf::Event::MouseMoved)
		{
			sf::Vector2f mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);

			m_angle = cpvtoangle(cpvsub(cpv(mousePos.x, mousePos.y), cpv(this->GetActorLocation().x, this->GetActorLocation().y)));
			m_angle *= (180 / CP_PI);

			sf::Transform t;

			t.rotate(m_angle, 0, 0);
			m_point = t.transformPoint({ 64.f,0.f });

			/*if (fabsf(m_angle) >= 90)
			{
				Weapon->weaponSprite->m_sprite.setScale(-fabsf(Weapon->weaponSprite->m_sprite.getScale().x), Weapon->weaponSprite->m_sprite.getScale().y);
			}
			if (fabsf(m_angle) <= 90)
			{
				Weapon->weaponSprite->m_sprite.setScale(fabsf(Weapon->weaponSprite->m_sprite.getScale().x), Weapon->weaponSprite->m_sprite.getScale().y);
			}*/
			if (fabsf(m_angle) >= 90)
			{
				Weapon->weaponSprite->m_sprite.setScale(Weapon->weaponSprite->m_sprite.getScale().x, -fabsf(Weapon->weaponSprite->m_sprite.getScale().y));
			}
			if (fabsf(m_angle) <= 90)
			{
				Weapon->weaponSprite->m_sprite.setScale(Weapon->weaponSprite->m_sprite.getScale().x, fabsf(Weapon->weaponSprite->m_sprite.getScale().y));
			}
		}
		if (event.key.code == sf::Keyboard::A && event.type == sf::Event::EventType::KeyPressed)
		{
			
			m_moving_left = true;
			float vel = -1.f - this->GetLinearVelocity().x;
			float impulse = cpBodyGetMass(this->Body) * vel;
			this->ApplyLinearImpulse(cpv(vel, 0), cpv(CollisionRectangle.width / 2, CollisionRectangle.height / 2));
		}
		if (event.key.code == sf::Keyboard::D && event.type == sf::Event::EventType::KeyPressed)
		{
			m_moving_right = true;
			float vel = 1.f - this->GetLinearVelocity().x;
			float impulse = cpBodyGetMass(this->Body) * vel;
			this->ApplyLinearImpulse(cpv(vel, 0), cpv(CollisionRectangle.width / 2, CollisionRectangle.height / 2));

		}
		if (event.key.code == sf::Keyboard::W && event.type == sf::Event::EventType::KeyPressed)
		{
			this->Jump();
		}

		if (event.key.code == sf::Keyboard::W && event.type == sf::Event::EventType::KeyPressed)
		{
			//Duplicate(this->GetSprite(),this->ShadowShape,this->Size,sf::Vector2f(this->GetActorLocation().x, this->GetActorLocation().y - 100), true,this->WorldContext,path);
		}

		if (event.key.code == sf::Keyboard::D && event.type == sf::Event::EventType::KeyReleased)
		{
			if (m_moving_right)
			{
				m_moving_right = false;
				cpBodySetVelocity(this->Body, cpv(cpBodyGetVelocity(this->Body).x *0.01f /*m_isOnTheGround ? 0.f : 0.05f*/, cpBodyGetVelocity(this->Body).y));
			}
		}

		if (event.key.code == sf::Keyboard::A && event.type == sf::Event::EventType::KeyReleased)
		{
			if (m_moving_left)
			{
				m_moving_left = false;
				cpBodySetVelocity(this->Body, cpv(cpBodyGetVelocity(this->Body).x*0.01f /*m_isOnTheGround ? 0.f : 0.05f*/, cpBodyGetVelocity(this->Body).y));
			}
			
		}
		
		if (event.key.code == sf::Keyboard::Space && event.type == sf::Event::EventType::KeyPressed)
		{
			Shoot();
		}
	}
}

CTestPlayer::~CTestPlayer()
{
}

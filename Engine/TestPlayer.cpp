#include "TestPlayer.h"



void CTestPlayer::SetSpriteTexture(sf::Texture & t)
{
}


CTestPlayer::CTestPlayer(sf::Sprite sprite, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, std::string path) :
	Character(CollisionShape, Size, Location, path),
	m_sprite(sprite)
{


	sf::Vector2f scale;

	if (m_sprite.getTexture()->getSize().x != 0) { scale.x = Size.x / m_sprite.getTexture()->getSize().x; }

	if (m_sprite.getTexture()->getSize().y != 0) { scale.y = Size.y / m_sprite.getTexture()->getSize().y; }

	this->m_sprite.setScale(scale);

}

void CTestPlayer::Init(std::string path, Engine::Context* context)
{

	this->path = path;

	sf::Vector2f scale;

	if (this->m_sprite.getTexture()->getSize().x != 0) { scale.x = Size.x / this->m_sprite.getTexture()->getSize().x; }

	if (this->m_sprite.getTexture()->getSize().y != 0) { scale.y = Size.y / this->m_sprite.getTexture()->getSize().y; }

	this->m_sprite.setScale(scale);

	//this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
}

void CTestPlayer::InitPhysBody(std::string path, cpSpace*& world)
{
	try
	{
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
					cpShapeSetCollisionType(shapes[i], (int)(this));
					//Prevent from bouncing 
					cpShapeSetElasticity(shapes[i], 0.0f);
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
	window.draw(m_sprite);
}

void CTestPlayer::Update(sf::Time dt, Engine::Context* context)
{
	if (Body != nullptr)
	{

		cpBodySetAngle(Body, 0);
		this->Location.x = cpBodyGetPosition(this->GetBody()).x;
		this->Location.y = cpBodyGetPosition(this->GetBody()).y;

		m_sprite.setPosition(this->GetActorLocation());
	}

	if (m_moving_left || m_moving_right)
	{
		m_passed_footstep_time += dt.asSeconds();
		if (m_passed_footstep_time >= m_per_foostep_time)
		{
			m_passed_footstep_time = 0.f;
			context->lowLevelSoundSystem->playSound(context->Sounds->GetSoundByName("footstep_tile2")->m_sound, 0, false, &m_footstep_sound_channel);
		}
	}
}

void CTestPlayer::OnBeginCollision(cpArbiter*& arb, CActor* otherActor)
{
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

		//Register b2Fixture in lua
		getGlobalNamespace(L)
			.beginClass<b2Fixture>("b2Fixture")
			.endClass();

		//Register Vector2 in lua
		getGlobalNamespace(L)
			.beginClass<sf::Vector2f>("Vector2")
			//add x,y and some functions possibly
			.addData<float>("x", &sf::Vector2<float>::x)
			.addData<float>("y", &sf::Vector2<float>::y)
			.addConstructor<void(*) (void)>()
			.endClass();


		getGlobalNamespace(L)
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
		getGlobalNamespace(L)
			.beginClass<CTestPlayer>("CTestPlayer")
			//.addConstructor<void(*) (sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, std::string path)>()

			.addProperty("Location", &CTestPlayer::GetActorLocation, &CTestPlayer::SetActorLocation)
			.addProperty("PhysBodyInitialized", &CTestPlayer::GetPhysBodyInitialized, &CTestPlayer::SetPhysBodyInitialized)
			.addProperty("AreaId", &CTestPlayer::GetAreaId, &CTestPlayer::SetAreaId)
			.addProperty("MaxVelocity", &CTestPlayer::GetMaxVelocity, &CTestPlayer::SetMaxVelocity)

			.addFunction("AddChildRaw", &CTestPlayer::AddChildRaw)
			.addFunction("GetChild", &CTestPlayer::GetChild)
			.addFunction("MoveX", &CTestPlayer::MoveX)
			.addFunction("MoveY", &CTestPlayer::MoveY)
			.addFunction("ApplyLinearImpulse", &CTestPlayer::ApplyLinearImpulse)
			.addFunction("GetLinearVelocity", &CTestPlayer::GetLinearVelocity)

			.addData<bool>("IsMovingX", &CTestPlayer::IsMovingX)
			.addData<bool>("IsMovingY", &CTestPlayer::IsMovingY)
			.addFunction("GetClassID", &CTestPlayer::GetClassID)

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

void CTestPlayer::OnEndCollision(cpArbiter*& arb, CActor* otherActor)
{
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
		getGlobalNamespace(L)
			.beginClass<sf::Vector2f>("Vector2")
			//add x,y and some functions possibly
			.addData<float>("x", &sf::Vector2<float>::x)
			.addData<float>("y", &sf::Vector2<float>::y)
			.addConstructor<void(*) (void)>()
			.endClass();

		getGlobalNamespace(L)
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

void CTestPlayer::HandleEvent(sf::Event event, Engine::Context* context)
{
	if (ControlledByPlayer)
	{
		if (event.key.code == sf::Keyboard::A && event.type == sf::Event::EventType::KeyPressed)
		{
			m_moving_left = true;
			this->MoveX(-1);
		}
		if (event.key.code == sf::Keyboard::D && event.type == sf::Event::EventType::KeyPressed)
		{
			m_moving_right = true;
			this->MoveX(1);

		}
		if (event.key.code == sf::Keyboard::W && event.type == sf::Event::EventType::KeyPressed)
		{
			this->Jump();
		}

		if (event.key.code == sf::Keyboard::W && event.type == sf::Event::EventType::KeyPressed)
		{
			std::cout << 0 << std::endl;
			context->AddActor
			(
				new CTestPlayer
				(
					this->m_sprite,
					this->ShadowShape,
					this->Size,
					sf::Vector2f(this->Location.x, this->Location.y - 100),
					this->path
				)
			);
			context->SceneActors.at(context->SceneActors.size() - 1)->Init(path, context);
			context->SceneActors.at(context->SceneActors.size() - 1)->InitPhysBody(path, context->space);

		}

		if (event.key.code == sf::Keyboard::D && event.type == sf::Event::EventType::KeyReleased)
		{
			m_moving_right = false;
		}

		if (event.key.code == sf::Keyboard::A && event.type == sf::Event::EventType::KeyReleased)
		{
			m_moving_left = false;
		}
	}

}

CTestPlayer::~CTestPlayer()
{
}

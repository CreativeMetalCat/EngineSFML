#include "TestPlayer.h"



void CTestPlayer::SetSpriteTexture(sf::Texture & t)
{
}


CTestPlayer::CTestPlayer(sf::Texture&texture, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, std::string path):Character(CollisionShape,Size,Location,path)
{

	this->sprite = sf::Sprite(texture);

	sf::Vector2f scale;
	
	if (texture.getSize().x != 0) { scale.x = Size.x / texture.getSize().x; }

	if (texture.getSize().y != 0) { scale.y = Size.y / texture.getSize().y; }

	this->sprite.setScale(scale);
	
}

void CTestPlayer::Init(std::string path)
{

	this->path = path;

	sf::Vector2f scale;

	if (this->sprite.getTexture()->getSize().x != 0) { scale.x = Size.x / this->sprite.getTexture()->getSize().x; }

	if (this->sprite.getTexture()->getSize().y != 0) { scale.y = Size.y / this->sprite.getTexture()->getSize().y; }

	this->sprite.setScale(scale);

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
	window.draw(sprite);
}

void CTestPlayer::Update(sf::Time dt)
{
	if (Body != nullptr)
	{
		
		cpBodySetAngle(Body, 0);
		this->Location.x = cpBodyGetPosition(this->GetBody()).x;
		this->Location.y = cpBodyGetPosition(this->GetBody()).y;

		sprite.setPosition(this->GetActorLocation());
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
CTestPlayer::~CTestPlayer()
{
}

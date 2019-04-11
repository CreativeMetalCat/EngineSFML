#include "Character.h"

#include "DynamicBodyRaycastCallback.h"

void Character::MoveX(float value)
{
	

	float vel = (value*this->GetMaxVelocity().x) - this->GetLinearVelocity().x;
	float impulse = this->GetBody()->GetMass()*vel;
	this->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0), true);
	
}

void Character::Jump()
{

	this->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0, this->GetBody()->GetMass() * 500), true);
}

void Character::MoveY(float value)
{
	using namespace luabridge;


	lua_State* L = luaL_newstate();


	std::string d = (path + "scripts/" + this->MovementScriptFileName);
	try
	{




		int status = luaL_dofile(L, d.c_str());
		if (status != 0)
		{
			fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		}


		luaL_openlibs(L);

		lua_pcall(L, 0, 0, 0);
		//Register CActor in lua
		Character::RegisterClassLUA(L);

		//Register Vector2 in lua
		getGlobalNamespace(L)
			.beginClass<sf::Vector2f>("Vector2")
			//add x,y and some functions possibly
			.addData<float>("x", &sf::Vector2<float>::x)
			.addData<float>("y", &sf::Vector2<float>::y)
			.addConstructor<void(*) (void)>()
			.endClass();

		getGlobalNamespace(L)
			.beginClass<b2Vec2>("b2Vector")
			.addData<float>("x", &b2Vec2::x)
			.addData<float>("y", &b2Vec2::y)
			.addConstructor<void(*) (void)>()
			.endClass();

		getGlobalNamespace(L)
			.beginClass<b2Body>("b2Body")
			.addFunction("GetLinearVelocity", &b2Body::GetLinearVelocity)
			.addFunction("GetMass", &b2Body::GetMass)
			.addFunction("ApplyImpulse", &b2Body::ApplyLinearImpulseToCenter)
			.endClass();


		LuaRef LuaSetActorLocation = getGlobal(L, "SetActorLocation");
		auto d = this->GetBody();
		LuaRef LuaMoveX = getGlobal(L, "MoveY");

		if (!LuaMoveX.isNil())
		{
			LuaMoveX(this->As<Character*>(), value);
		}
		else
		{
			throw std::exception("Failed to find function: Move");
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

void Character::Init(std::string path)
{
	
	
	
}

void Character::InitPhysBody(std::string path, b2World & world)
{
	b2BodyDef defP;
	defP.type = b2BodyType::b2_dynamicBody;
	defP.position.Set(Location.x + Size.x / 2, Location.y + Size.y / 2);
	

	this->Body = world.CreateBody(&defP);

	b2PolygonShape shape;

	
	

	/*if (CollisionShape.getPointCount() > 0)
	{
		shape.m_count = CollisionShape.getPointCount();
		for (int i = 0; i < CollisionShape.getPointCount(); i++)
		{
			shape.m_vertices[i].Set(CollisionShape.getPoint(i).x, CollisionShape.getPoint(i).y);
		}

	}*/
	shape.SetAsBox(CollisionRectangle.width / 2, CollisionRectangle.height / 2);
	
	

	
	b2EdgeShape smootingShape;
	smootingShape.Set
	(
		b2Vec2(CollisionRectangle.left, CollisionRectangle.height + CollisionRectangle.top),
		b2Vec2(CollisionRectangle.left+CollisionRectangle.width, CollisionRectangle.height + CollisionRectangle.top)
	);
	smootingShape.m_hasVertex0 = true;
	smootingShape.m_hasVertex3 = true;

	smootingShape.m_vertex0 = b2Vec2(CollisionRectangle.left - 3, CollisionRectangle.height + CollisionRectangle.top - 3);
	smootingShape.m_vertex3 = b2Vec2(CollisionRectangle.left + CollisionRectangle.width + 3, CollisionRectangle.height + CollisionRectangle.top - 3);

	
	


	b2FixtureDef TriggerFixtureP;
	TriggerFixtureP.density = 1.f;
	TriggerFixtureP.shape = &shape;
	TriggerFixtureP.isSensor = false;
	TriggerFixtureP.friction = 0.0f;
	
	

	//this->Body->SetBullet(true);

	
	this->Body->CreateFixture(&TriggerFixtureP);

	this->Body->SetUserData(this);
	
}

void Character::RegisterClassLUA(lua_State *& L)
{
	using namespace luabridge;
	try
	{
		//Register CActor in lua
		getGlobalNamespace(L)
			.beginClass<Character>("Character")
			//.addConstructor<void(*) (sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, std::string path)>()

			.addProperty("Location", &Character::GetActorLocation, &Character::SetActorLocation)
			.addProperty("PhysBodyInitialized", &Character::GetPhysBodyInitialized, &Character::SetPhysBodyInitialized)
			.addProperty("AreaId", &Character::GetAreaId, &Character::SetAreaId)
			.addProperty("MaxVelocity",&Character::GetMaxVelocity,&Character::SetMaxVelocity)

			.addFunction("AddChildRaw", &Character::AddChildRaw)
			.addFunction("GetChild", &Character::GetChild)
			.addFunction("MoveX",&Character::MoveX)
			.addFunction("MoveY",&Character::MoveY)
			.addFunction("ApplyLinearImpulse",&Character::ApplyLinearImpulse)
			.addFunction("GetLinearVelocity",&Character::GetLinearVelocity)

			.addData<bool>("IsMovingX",&Character::IsMovingX)
			.addData<bool>("IsMovingY", &Character::IsMovingY)
			
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

void Character::Update(sf::Time dt)
{
	if (Body != nullptr)
	{

		this->Location.x = Body->GetPosition().x;
		this->Location.y = Body->GetPosition().y;
	}
}

Character::Character(sf::ConvexShape CollisionShape, sf::Vector2f Size,sf::Vector2f Location, std::string path):CActor(Location,path),Size(Size),ShadowShape(CollisionShape)
{
	if (CollisionShape.getPointCount() > 8)
	{
		std::cout << "Waring: Characters collision point count is greater than limit of the physics engine. This can result in unexpected behaviour" << std::endl;
	}

	CollisionRectangle.width = Size.x;
	CollisionRectangle.height = Size.y;
}


Character::~Character()
{
}

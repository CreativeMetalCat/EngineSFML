#include "Character.h"

#include "DynamicBodyRaycastCallback.h"

void Character::MoveX(float value)
{
	

	float vel = (value*this->GetMaxVelocity().x)- this->GetLinearVelocity().x;
	float impulse = cpBodyGetMass(this->Body)* vel/100;
	this->ApplyLinearImpulse(cpv(vel, 0), cpv(CollisionRectangle.width/2, CollisionRectangle.height/2));
}

void Character::Jump()
{
	this->ApplyLinearImpulse(cpv(0, -10), cpv(0, 0));
}

void Character::StopXMovement()
{
	//cpBodySetVelocity(this->GetBody(), cpv(cpBodyGetVelocity(GetBody()).x*0.01f, cpBodyGetVelocity(GetBody()).y));
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

void Character::InitPhysBody(std::string path, cpSpace *&world)
{
	try
	{
		std::vector<cpVect>points;
		for (int i = 0; i < ShadowShape.getPointCount(); i++)
		{
			points.push_back(cpv(ShadowShape.getPoint(i).x, ShadowShape.getPoint(i).x));
		}

		this->Body = cpBodyNew(100.f,cpMomentForBox(100.f,CollisionRectangle.width,CollisionRectangle.height));
		if (this->Body != nullptr)
		{
			//perform here actions that can happen only after body init

			
			shapes.push_back(cpBoxShapeNew(this->GetBody(), CollisionRectangle.width, CollisionRectangle.height, 0/*(sqrt(CollisionRectangle.width*CollisionRectangle.width + CollisionRectangle.height*CollisionRectangle.height) / 2)*/));

			cpSpaceAddBody(world, this->Body);

			cpBodySetUserData(Body, this);

			cpBodySetPosition(this->Body, cpv(this->GetActorLocation().x, this->GetActorLocation().y));

			for (int i = 0; i < shapes.size(); i++)
			{
				if (shapes.at(i) != nullptr)
				{
					cpSpaceAddShape(world, shapes[i]);
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
		cpBodySetAngle(Body, 0);
		this->Location.x = cpBodyGetPosition(this->GetBody()).x;
		this->Location.y = cpBodyGetPosition(this->GetBody()).y;		
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

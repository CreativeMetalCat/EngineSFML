#include "CTrigger.h"

namespace Engine
{

	CTrigger::CTrigger(std::string ScriptFileName, sf::Vector2f Size, sf::Vector2f Location, Context* WorldContext, std::string path):
		CActor(Location,WorldContext,path),
		Size(Size)
	{
		this->ScriptFileName = ScriptFileName;
	}

	void CTrigger::Init(std::string path)
	{
	}

	void CTrigger::InitPhysBody(std::string path, cpSpace*& world)
	{
		try
		{
			std::vector<cpVect>points;
			for (int i = 0; i < ShadowShape.getPointCount(); i++)
			{
				points.push_back(cpv(ShadowShape.getPoint(i).x, ShadowShape.getPoint(i).x));
			}

			this->Body = cpBodyNewKinematic();
			
			if (this->Body != nullptr)
			{
				//perform here actions that can happen only after body init


				shapes.push_back(cpBoxShapeNew(this->GetBody(),Size.x, Size.y, 0));

				cpSpaceAddBody(world, this->Body);

				cpBodySetUserData(Body, this);

				cpBodySetPosition(this->Body, cpv(this->GetActorLocation().x, this->GetActorLocation().y));

				for (int i = 0; i < shapes.size(); i++)
				{
					if (shapes.at(i) != nullptr)
					{
						cpShapeSetSensor(shapes.at(i), true);

						cpSpaceAddShape(world, shapes[i]);
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

	void CTrigger::OnBeginCollision(cpArbiter*& arb, CActor* otherActor)
	{
		using namespace luabridge;
		lua_State* L = luaL_newstate();
		std::string d = (path + ScriptFileName);
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

	void CTrigger::OnEndCollision(cpArbiter*& arb, CActor* otherActor)
	{
	}


	void CTrigger::RegisterClassLUA(lua_State*& L)
	{
		using namespace luabridge;
		try
		{
			//Register CActor in lua
			getGlobalNamespace(L)
				.beginClass<CTrigger>("CTrigger")
				//.addConstructor<void(*) (sf::Vector2f)>()

				.addProperty("Location", &CTrigger::GetActorLocation, &CTrigger::SetActorLocation)
				.addProperty("PhysBodyInitialized", &CTrigger::GetPhysBodyInitialized, &CTrigger::SetPhysBodyInitialized)
				.addProperty("AreaId", &CTrigger::GetAreaId, &CTrigger::SetAreaId)

				.addFunction("AddChildRaw", &CTrigger::AddChildRaw)
				.addFunction("GetChild", &CTrigger::GetChild)
				.addFunction("GetBody", &CTrigger::GetBody)
				.addFunction("ApplyLinearImpulse", &CTrigger::ApplyLinearImpulseToZero)
				.addFunction("GetClassID", &CTrigger::GetClassID)
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

	CTrigger::~CTrigger()
	{
	}
}
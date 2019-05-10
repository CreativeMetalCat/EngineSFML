#include "ElevatorTrigger.h"
#include "FuncElevator.h"
#include "TestPlayer.h"

	CElevatorTrigger::CElevatorTrigger(std::string targetName, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path):
		CTrigger("",Size,Location,WorldContext,path),
		targetName(targetName)
	{
	}

	void CElevatorTrigger::Init(std::string path)
	{
	}

	void CElevatorTrigger::InitPhysBody(std::string path, cpSpace*& world)
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


				shapes.push_back(cpBoxShapeNew(this->GetBody(), Size.x, Size.y, 0));

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


	void CElevatorTrigger::OnBeginCollision(cpArbiter*& arb, CActor* otherActor)
	{
		//CLASS_CTESTPLAYER is 4
		if (otherActor->GetClassID() == 4)
		{
			for (auto& actor : WorldContext->SceneActors)
			{
				//CLASS_FUNCELEVATOR is 22
				if (actor->Name == this->targetName && actor->GetClassID() == CLASS_FUNCELEVATOR)
				{
					actor->As<Test::FuncElevator*>()->StartMovement();
					break;
				}
			}
		}
	}

	void CElevatorTrigger::OnEndCollision(cpArbiter*& arb, CActor* otherActor)
	{
		//CLASS_CTESTPLAYER is 4
		if (otherActor->ClassID == 4)
		{
			for (auto& actor : WorldContext->SceneActors)
			{
				//CLASS_FUNCELEVATOR is 22
				if (actor->Name == this->targetName && actor->ClassID == CLASS_FUNCELEVATOR)
				{
					actor->As<Test::FuncElevator*>()->StopMovement();
					break;
				}
			}
		}
	}

	void CElevatorTrigger::RegisterClassLUA(lua_State*& L)
	{
		using namespace luabridge;
		try
		{
			//Register CActor in lua
			getGlobalNamespace(L)
				.beginClass<CElevatorTrigger>("CElevatorTrigger")
				//.addConstructor<void(*) (sf::Vector2f)>()

				.addProperty("Location", &CElevatorTrigger::GetActorLocation, &CElevatorTrigger::SetActorLocation)
				.addProperty("PhysBodyInitialized", &CElevatorTrigger::GetPhysBodyInitialized, &CElevatorTrigger::SetPhysBodyInitialized)
				.addProperty("AreaId", &CElevatorTrigger::GetAreaId, &CElevatorTrigger::SetAreaId)

				.addFunction("AddChildRaw", &CElevatorTrigger::AddChildRaw)
				.addFunction("GetChild", &CElevatorTrigger::GetChild)
				.addFunction("GetBody", &CElevatorTrigger::GetBody)
				.addFunction("ApplyLinearImpulse", &CElevatorTrigger::ApplyLinearImpulseToZero)
				.addFunction("GetClassID", &CElevatorTrigger::GetClassID)
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

	CElevatorTrigger::~CElevatorTrigger()
	{
	}

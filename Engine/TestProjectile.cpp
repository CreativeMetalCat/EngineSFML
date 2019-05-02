#include "TestProjectile.h"

namespace Test
{

	TestProjectile::TestProjectile(sf::Sprite sprite, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path):
		Engine::CProjectile(Location,WorldContext,path),
		m_sprite(sprite),
		Size(Size)
	{
		CollisionRectangle.height = Size.y;
		CollisionRectangle.width = Size.x;
	}


	void TestProjectile::Init(std::string path)
	{

		this->path = path;

		sf::Vector2f scale;

		if (this->m_sprite.getTexture()->getSize().x != 0) { scale.x = Size.x / this->m_sprite.getTexture()->getSize().x; }

		if (this->m_sprite.getTexture()->getSize().y != 0) { scale.y = Size.y / this->m_sprite.getTexture()->getSize().y; }

		this->m_sprite.setScale(scale);

		//this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
	}

	void TestProjectile::InitPhysBody(std::string path, cpSpace*& world)
	{
		try
		{

			this->Body = cpBodyNew(100.f, cpMomentForBox(100.f, CollisionRectangle.width, CollisionRectangle.height));
			if (this->Body != nullptr)
			{
				//perform here actions that can happen only after body init


				shapes.push_back(cpBoxShapeNew(this->GetBody(), CollisionRectangle.width, CollisionRectangle.height, 0));

				cpSpaceAddBody(world, this->Body);

				cpBodySetUserData(Body, this);

				cpBodySetPosition(this->Body, cpv(this->GetActorLocation().x, this->GetActorLocation().y));

				for (int i = 0; i < shapes.size(); i++)
				{
					if (shapes.at(i) != nullptr)
					{
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

	void TestProjectile::RegisterClassLUA(lua_State*& L)
	{

		using namespace luabridge;
		try
		{
			//Register CActor in lua
			getGlobalNamespace(L)
				.beginClass<TestProjectile>("TestProjectile")
				//.addConstructor<void(*) (sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, std::string path)>()

				.addProperty("Location", &TestProjectile::GetActorLocation, &TestProjectile::SetActorLocation)
				.addProperty("PhysBodyInitialized", &TestProjectile::GetPhysBodyInitialized, &TestProjectile::SetPhysBodyInitialized)
				.addProperty("AreaId", &TestProjectile::GetAreaId, &TestProjectile::SetAreaId)


				.addFunction("AddChildRaw", &TestProjectile::AddChildRaw)
				.addFunction("GetChild", &TestProjectile::GetChild)
				.addFunction("ApplyLinearImpulse", &TestProjectile::ApplyLinearImpulse)
				.addFunction("GetLinearVelocity", &TestProjectile::GetLinearVelocity)
				.addFunction("GetClassID", &TestProjectile::GetClassID)

				.addStaticFunction("Create", &TestProjectile::CreateInWorld)

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

	void TestProjectile::Draw(sf::RenderWindow& window)
	{
		window.draw(m_sprite);
	}

	void TestProjectile::CreateInWorld(sf::Vector2f spawnImpulse, sf::Vector2f spawnImpulseLocalLocation, sf::Sprite sprite, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path)
	{
		if (WorldContext != nullptr)
		{
			WorldContext->AddActor
			(
				new TestProjectile
				(
					sprite,
					Size,
					Location,
					WorldContext,
					path
				)
			);

			WorldContext->SceneActors.at(WorldContext->SceneActors.size() - 1)->Init(path);
			WorldContext->SceneActors.at(WorldContext->SceneActors.size() - 1)->InitPhysBody(path, WorldContext->space);

			WorldContext->SceneActors.at(WorldContext->SceneActors.size() - 1)->setLifeTime(3.f);

			if (!(spawnImpulse.x == 0.f && spawnImpulse.y == 0.f))
			{
				cpBodyApplyImpulseAtLocalPoint(WorldContext->SceneActors.at
				(
					WorldContext->SceneActors.size() - 1)->Body,
					cpv(spawnImpulse.x, spawnImpulse.y),
					cpv(spawnImpulseLocalLocation.x, spawnImpulseLocalLocation.y)
				);
			}
		}
	}

	void TestProjectile::Update(sf::Time dt)
	{
		m_lived_time += dt.asSeconds();

		if (Body != nullptr)
		{

			cpBodySetAngle(Body, 0);
			this->Location.x = cpBodyGetPosition(this->GetBody()).x;
			this->Location.y = cpBodyGetPosition(this->GetBody()).y;

			m_sprite.setPosition(this->GetActorLocation());
		}
	}

	void TestProjectile::Release()
	{
		cpSpaceRemoveBody(WorldContext->space, this->Body);
		for (auto& shape : this->shapes)
		{
			cpSpaceRemoveShape(WorldContext->space, shape);
			cpShapeDestroy(shape);
			cpShapeFree(shape);
		}
		
		cpBodyDestroy(this->Body);
		cpBodyFree(this->Body);
		
	}

	TestProjectile::~TestProjectile()
	{
	}
}
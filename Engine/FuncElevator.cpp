#include "FuncElevator.h"

namespace Test
{
	FuncElevator::FuncElevator(sf::Vector2f EndLocation, sf::Sprite sprite, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path) :
		CActor(Location, WorldContext, path),
		Size(Size),
		m_sprite(sprite),
		EndLocation(EndLocation),
		StartLocation(Location)
	{

		CollisionRectangle.width = Size.x;
		CollisionRectangle.height = Size.y;
	}

	void FuncElevator::Init(std::string path)
	{
		this->path = path;

		sf::Vector2f scale;

		if (this->m_sprite.getTexture()->getSize().x != 0) { scale.x = Size.x / this->m_sprite.getTexture()->getSize().x; }

		if (this->m_sprite.getTexture()->getSize().y != 0) { scale.y = Size.y / this->m_sprite.getTexture()->getSize().y; }

		this->m_sprite.setScale(scale);


		//this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
	}

	void FuncElevator::InitPhysBody(std::string path, cpSpace*& world)
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
				//this->StartMovement();
			}
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	void FuncElevator::StartMovement()
	{
		if (m_arrived)
		{
			m_arrived = false;
			m_moving = true;

			if (roundf(GetActorLocation().y) == roundf(EndLocation.y) && roundf(GetActorLocation().x) == roundf(EndLocation.x))
			{
				//if elevator has already arrived to end location we go back
				cpBodySetVelocity(this->Body, cpv(cos(cpvtoangle(cpv((this->Location - EndLocation).x, (this->Location - EndLocation).y))) * Speed, sin(cpvtoangle(cpv((this->Location - EndLocation).x, (this->Location - EndLocation).y))) * Speed));
				this->m_moving_back = true;
			}
			else if (roundf(GetActorLocation().y) == roundf(StartLocation.y) && roundf(GetActorLocation().x) == roundf(StartLocation.x))
			{
				cpBodySetVelocity(this->Body, cpv(cos(cpvtoangle(cpv((EndLocation - this->Location).x, (EndLocation - this->Location).y))) * Speed, sin(cpvtoangle(cpv((EndLocation - this->Location).x, (EndLocation - this->Location).y))) * Speed));
				this->m_moving_back = false;
			}

			m_passed_sound_time = 0.f;
			WorldContext->lowLevelSoundSystem->playSound(WorldContext->Sounds->GetSoundByName("drawbridge_move1")->m_sound, 0, false, &m_movement_sound_channel);
		}
		else
		{
			m_arrived = false;
			m_moving = true;
			cpBodySetVelocity(this->Body, cpv(cos(cpvtoangle(cpv((EndLocation - this->Location).x, (EndLocation - this->Location).y))) * Speed, sin(cpvtoangle(cpv((EndLocation - this->Location).x, (EndLocation - this->Location).y))) * Speed));
			m_passed_sound_time = 0.f;
			WorldContext->lowLevelSoundSystem->playSound(WorldContext->Sounds->GetSoundByName("drawbridge_move1")->m_sound, 0, false, &m_movement_sound_channel);
		}
	}

	void FuncElevator::StopMovement()
	{
		cpBodySetVelocity(this->Body, cpv(0, 0));
		this->m_moving = false;
		m_movement_sound_channel->stop();
		WorldContext->lowLevelSoundSystem->playSound(WorldContext->Sounds->GetSoundByName("drawbridge_stop1")->m_sound, 0, false, &m_stop_sound_channel);
	}


	void FuncElevator::Draw(sf::RenderWindow & window)
	{
		window.draw(m_sprite);

		//sf::Vector2f f = sf::Vector2f(cpBodyGetCenterOfGravity(this->Body).x, cpBodyGetCenterOfGravity(this->Body).y);
		sf::VertexArray va;
		va.append(sf::Vertex(sf::Vector2f(cpBodyGetCenterOfGravity(this->Body).x, cpBodyGetCenterOfGravity(this->Body).y), sf::Color::Red));
		window.draw(va);
	}

	void FuncElevator::Update(sf::Time dt)
	{


		if (!m_moving_back)
		{
			if (roundf(GetActorLocation().y) == roundf(EndLocation.y) && roundf(GetActorLocation().x) == roundf(EndLocation.x))
			{
				if (!m_arrived)
				{
					m_arrived = true;
					StopMovement();
				}
			}
		}
		else
		{
			if (roundf(GetActorLocation().y) == roundf(StartLocation.y) && roundf(GetActorLocation().x) == roundf(StartLocation.x))
			{
				if (!m_arrived)
				{
					m_arrived = true;
					StopMovement();
				}
			}
		}
		if (!m_arrived)
		{
			if (Body != nullptr)
			{



				this->Location.x = cpBodyGetPosition(this->GetBody()).x;
				this->Location.y = cpBodyGetPosition(this->GetBody()).y;

				m_sprite.setPosition(this->GetActorLocation());
				this->m_sprite.setOrigin(sf::Vector2f(this->CollisionRectangle.width / 2, this->CollisionRectangle.height / 2));
				//rotation actions
				this->m_sprite.setRotation(cpBodyGetAngle(this->Body));
				this->m_sprite.setOrigin({ 0.f,0.f });

			}
			m_passed_sound_time += dt.asSeconds();
			if (m_passed_sound_time >= m_sound_time)
			{
				m_passed_sound_time = 0.f;
				WorldContext->lowLevelSoundSystem->playSound(WorldContext->Sounds->GetSoundByName("drawbridge_move1")->m_sound, 0, false, &m_movement_sound_channel);
			}

		}
	}

	FuncElevator::~FuncElevator()
	{
	}
}
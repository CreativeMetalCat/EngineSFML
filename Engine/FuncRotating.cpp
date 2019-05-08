#include "FuncRotating.h"


namespace Test
{
	FuncRotating::FuncRotating(bool Clockwise, float StartAngle, float EndAngle, sf::Sprite sprite, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path):
		CActor(Location, WorldContext, path),
		Size(Size),
		m_sprite(sprite),
		Clockwise(Clockwise),
		StartAngle(StartAngle),
		EndAngle(EndAngle)
	{
		CollisionRectangle.width = Size.x;
		CollisionRectangle.height = Size.y;
	}

	void FuncRotating::Init(std::string path)
	{
		this->path = path;

		sf::Vector2f scale;

		if (this->m_sprite.getTexture()->getSize().x != 0) { scale.x = Size.x / this->m_sprite.getTexture()->getSize().x; }

		if (this->m_sprite.getTexture()->getSize().y != 0) { scale.y = Size.y / this->m_sprite.getTexture()->getSize().y; }

		this->m_sprite.setScale(scale);


		//this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
	}

	void FuncRotating::InitPhysBody(std::string path, cpSpace*& world)
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

	void FuncRotating::StartMovement()
	{
		if (cpBodyGetAngle(this->Body) == StartAngle)
		{

		}
		else if (cpBodyGetAngle(this->Body) == EndAngle)
		{

		}
	}

	void FuncRotating::StopMovement()
	{
		this->m_moving = false;
		m_movement_sound_channel->stop();
		WorldContext->lowLevelSoundSystem->playSound(WorldContext->Sounds->GetSoundByName("drawbridge_stop1")->m_sound, 0, false, &m_stop_sound_channel);
	}

	void FuncRotating::Draw(sf::RenderWindow& window)
	{
		window.draw(m_sprite);
	}

	void FuncRotating::Update(sf::Time dt)
	{
		if (cpBodyGetAngle(this->Body)==this->EndAngle)
		{
			if (!m_arrived)
			{
				m_arrived = true;
				StopMovement();
			}
		}
		if (!m_arrived)
		{
			if (Body != nullptr)
			{
				cpBodySetAngle(Body, 2 * CP_PI * 0.1f + cpBodyGetAngle(this->Body));

				//cpBodySetAngle(Body, 0);
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

	FuncRotating::~FuncRotating()
	{
	}
}
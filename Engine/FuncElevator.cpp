#include "FuncElevator.h"

namespace Test
{
	FuncElevator::FuncElevator(sf::Sprite sprite, sf::Vector2f Size, sf::Vector2f Location, Engine::Context* WorldContext, std::string path):
		CActor(Location, WorldContext, path),
		Size(Size),
		m_sprite(sprite)
	{

		
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
				cpBodySetVelocity(this->Body, cpv(0, -0.01f));
			}
		}
		catch (std::exception e)
		{
			std::cout << e.what() << std::endl;
		}
	}


	void FuncElevator::Draw(sf::RenderWindow& window)
	{
		window.draw(m_sprite);
	}

	void FuncElevator::Update(sf::Time dt)
	{

		if (Body != nullptr)
		{
		

			cpBodySetAngle(Body, 0);
			this->Location.x = cpBodyGetPosition(this->GetBody()).x;
			this->Location.y = cpBodyGetPosition(this->GetBody()).y;

			m_sprite.setPosition(this->GetActorLocation());

		}	
		m_passed_sound_time += dt.asSeconds();
		if (m_passed_sound_time >= m_sound_time)
		{
			m_passed_sound_time = 0.f;
			WorldContext->lowLevelSoundSystem->playSound(WorldContext->Sounds->GetSoundByName("garage_move1")->m_sound, 0, false, &m_movement_sound_channel);
		}
	}

	FuncElevator::~FuncElevator()
	{
	}
}
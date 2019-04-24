#include "SolidBlock.h"

namespace Engine
{
	void CSolidBlock::SetSpriteTexture(sf::Texture& texture)
	{

	}

	void CSolidBlock::Init(std::string path, Context* context)
	{

		sf::Vector2f scale;

		if (this->m_sprite.getTexture()->getSize().x != 0) { scale.x = Size.x / this->m_sprite.getTexture()->getSize().x; }

		if (this->m_sprite.getTexture()->getSize().y != 0) { scale.y = Size.y / this->m_sprite.getTexture()->getSize().y; }

		this->m_sprite.setScale(scale);
		this->m_sprite.setPosition(this->GetActorLocation());
	}

	void CSolidBlock::InitPhysBody(std::string path, cpSpace*& world)
	{
		this->path = path;
		try
		{
			std::vector<cpVect>points;
			for (int i = 0; i < ShadowShape.getPointCount(); i++)
			{
				points.push_back(cpv(ShadowShape.getPoint(i).x, ShadowShape.getPoint(i).x));
			}
			this->Body = cpBodyNewStatic();
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

	void CSolidBlock::Draw(sf::RenderWindow& window)
	{
		window.draw(m_sprite);
	}

	CSolidBlock::CSolidBlock(sf::Sprite sprite, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, Context* WorldContext, std::string path) :
		CActor(Location,WorldContext ,path),
		Size(Size),
		m_sprite(sprite)
	{
		//this->sprite.setScale(scale);
		this->ShadowShape = CollisionShape;
		CollisionRectangle.width = Size.x;
		CollisionRectangle.height = Size.y;
	}




	CSolidBlock::~CSolidBlock()
	{
	}
}
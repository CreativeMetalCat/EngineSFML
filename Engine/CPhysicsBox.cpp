#include "CPhysicsBox.h"

void CPhysicsBox::Init(std::string path, Engine::Context* context)
{
	this->path = path;

	sf::Vector2f scale;

	if (this->m_sprite.getTexture()->getSize().x != 0) { scale.x = Size.x / this->m_sprite.getTexture()->getSize().x; }

	if (this->m_sprite.getTexture()->getSize().y != 0) { scale.y = Size.y / this->m_sprite.getTexture()->getSize().y; }

	this->m_sprite.setScale(scale);

	ShadowShape.setPointCount(4);

	ShadowShape.setPoint(0, sf::Vector2f(0, 0));
	ShadowShape.setPoint(1, sf::Vector2f(0, CollisionRectangle.height));
	ShadowShape.setPoint(2, sf::Vector2f(CollisionRectangle.width,CollisionRectangle.height));
	ShadowShape.setPoint(3, sf::Vector2f(0, CollisionRectangle.height));
}

void CPhysicsBox::Update(sf::Time, Engine::Context* context)
{
	if (Body != nullptr)
	{

		cpBodySetAngle(Body, 0);
		this->Location.x = cpBodyGetPosition(this->GetBody()).x;
		this->Location.y = cpBodyGetPosition(this->GetBody()).y;

		m_sprite.setPosition(this->GetActorLocation());
	}
}

void CPhysicsBox::Draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}

void CPhysicsBox::InitPhysBody(std::string path, cpSpace*& world)
{
	this->path = path;
	try
	{
		this->Body = cpBodyNew(this->GetMass(), cpMomentForBox(this->GetMass(), CollisionRectangle.width, CollisionRectangle.height));
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

CPhysicsBox::CPhysicsBox(sf::Sprite sprite, sf::Vector2f Size, sf::Vector2f position, std::string path, float Mass, std::string MaterialName)
	:Engine::CPhysicalObject(position,path,Mass,MaterialName),
	Size(Size),
	m_sprite(sprite)
{
	this->CollisionRectangle.width = Size.x;
	this->CollisionRectangle.height = Size.y;
}

CPhysicsBox::~CPhysicsBox()
{
}

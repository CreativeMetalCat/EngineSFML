#include "SolidBlock.h"

void CSolidBlock::SetSpriteTexture(sf::Texture & texture)
{

}

void CSolidBlock::Init(std::string path)
{

	sf::Vector2f scale;

	if (this->sprite.getTexture()->getSize().x != 0) { scale.x = Size.x / this->sprite.getTexture()->getSize().x; }

	if (this->sprite.getTexture()->getSize().y != 0) { scale.y = Size.y / this->sprite.getTexture()->getSize().y; }

	this->sprite.setScale(scale);
	this->sprite.setPosition(this->GetActorLocation());
}

void CSolidBlock::InitPhysBody(std::string path, cpSpace *&world)
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

void CSolidBlock::Draw(sf::RenderWindow & window)
{
	window.draw(sprite);
}

CSolidBlock::CSolidBlock(sf::Texture&texture, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, std::string path ):CActor(Location,path),Size(Size), ShadowShape(CollisionShape)
{
	this->sprite = sf::Sprite(texture);
	//this->sprite.setScale(scale);

	CollisionRectangle.width = Size.x;
	CollisionRectangle.height = Size.y;
}




CSolidBlock::~CSolidBlock()
{
}

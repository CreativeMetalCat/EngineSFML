#include "TestPlayer.h"



void CTestPlayer::SetSpriteTexture(sf::Texture & t)
{
}


CTestPlayer::CTestPlayer(sf::Texture&texture, sf::ConvexShape CollisionShape, sf::Vector2f Size, sf::Vector2f Location, std::string path):Character(CollisionShape,Size,Location,path)
{

	this->sprite = sf::Sprite(texture);

	sf::Vector2f scale;
	
	if (texture.getSize().x != 0) { scale.x = Size.x / texture.getSize().x; }

	if (texture.getSize().y != 0) { scale.y = Size.y / texture.getSize().y; }

	this->sprite.setScale(scale);
	
}

void CTestPlayer::Init(std::string path)
{

	this->path = path;

	sf::Vector2f scale;

	if (this->sprite.getTexture()->getSize().x != 0) { scale.x = Size.x / this->sprite.getTexture()->getSize().x; }

	if (this->sprite.getTexture()->getSize().y != 0) { scale.y = Size.y / this->sprite.getTexture()->getSize().y; }

	this->sprite.setScale(scale);

	//this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
}

void CTestPlayer::InitPhysBody(std::string path, cpSpace*& world)
{
	try
	{
		std::vector<cpVect>points;
		for (int i = 0; i < ShadowShape.getPointCount(); i++)
		{
			points.push_back(cpv(ShadowShape.getPoint(i).x, ShadowShape.getPoint(i).x));
		}

		this->Body = cpBodyNew(100.f, cpMomentForBox(100.f, CollisionRectangle.width, CollisionRectangle.height));
		if (this->Body != nullptr)
		{
			//perform here actions that can happen only after body init


			shapes.push_back(cpBoxShapeNew(this->GetBody(), CollisionRectangle.width, CollisionRectangle.height, 0));
			
			cpSpaceAddBody(world, this->Body);

			cpBodySetUserData(Body, this);

			cpBodySetPosition(this->Body, cpv(this->GetActorLocation().x , this->GetActorLocation().y));

			for (int i = 0; i < shapes.size(); i++)
			{
				if (shapes.at(i) != nullptr)
				{
					cpSpaceAddShape(world, shapes[i]);
					cpShapeSetCollisionType(shapes[i], (int)(this));
				}

			}
			cpCollisionHandler* h = cpSpaceAddWildcardHandler(world, (int)(this));
			

			this->SetActorLocation(sf::Vector2f(cpBodyGetPosition(Body).x, cpBodyGetPosition(Body).y));
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
}

void CTestPlayer::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void CTestPlayer::Update(sf::Time dt)
{
	if (Body != nullptr)
	{
		
		cpBodySetAngle(Body, 0);
		this->Location.x = cpBodyGetPosition(this->GetBody()).x;
		this->Location.y = cpBodyGetPosition(this->GetBody()).y;

		sprite.setPosition(this->GetActorLocation());
	}
}




CTestPlayer::~CTestPlayer()
{
}

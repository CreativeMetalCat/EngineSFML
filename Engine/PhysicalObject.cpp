#include "PhysicalObject.h"


namespace Engine
{
	void CPhysicalObject::Init(std::string PATH, Context* context)
	{
		PhysMaterial->Init(PATH,context);
	}

	void CPhysicalObject::InitPhysBody(std::string path, cpSpace*& world)
	{
		this->path = path;
		try
		{
			this->Body = cpBodyNew(mass,cpMomentForBox(mass, CollisionRectangle.width, CollisionRectangle.height));
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

	CPhysicalObject::CPhysicalObject(sf::Vector2f position, std::string MaterialName, Context* WorldContext, float Mass, std::string path) :
		CActor(position,WorldContext, path),
		MaterialName(MaterialName),
		mass(Mass)
	{
		PhysMaterial = std::make_shared<PhysicalMaterial>(MaterialName, WorldContext,path);
	}


	CPhysicalObject::~CPhysicalObject()
	{
	}
}
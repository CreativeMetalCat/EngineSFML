#include "Projectile.h"

namespace Engine
{

	CProjectile::CProjectile(sf::Vector2f Location, Context* WorldContext, std::string path):
		CActor(Location,WorldContext,path)
	{
	}


	void CProjectile::CreateInWorld(sf::Vector2f spawnImpulse,sf::Vector2f spawnImpulseLocalLocation,sf::Vector2f Location, Context* WorldContext, std::string path)
	{
		if (WorldContext != nullptr)
		{
			WorldContext->AddActor
			(
				new CProjectile
				(
				Location,
				WorldContext,
				path
				)
			);

			WorldContext->SceneActors.at(WorldContext->SceneActors.size() - 1)->Init(path);
			WorldContext->SceneActors.at(WorldContext->SceneActors.size() - 1)->InitPhysBody(path, WorldContext->space);
			if (spawnImpulse.x != 0.f && spawnImpulse.y != 0.f)
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

	CProjectile::~CProjectile()
	{
	}
}
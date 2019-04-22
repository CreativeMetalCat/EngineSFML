#pragma once
#include "Actor.h"
#include "PhysicalMaterial.h"

namespace Engine
{
	//Basic CObject that simulates physics
	//Unlike all other CActor's children it is supossed to have functionality to play sounds on hit etc.
	class CPhysicalObject : public CActor
	{
	protected:
		std::string MaterialName = "";

		float mass = 1.f;

		bool isMovingOnX = false;

		std::shared_ptr<PhysicalMaterial> PhysMaterial;
	public:

		bool GetIsMovingOnX()const { return isMovingOnX; }

		float GetMass()const { return mass; }

		void SetMass(float m) { mass = m; }

		std::string GetMaterialName()const { return MaterialName; }

		void SetMaterialName(std::string MaterialName = "") { this->MaterialName = MaterialName; }

		void Init(std::string path, Context* context)override;

		virtual void InitPhysBody(std::string path, cpSpace*& world);

		CPhysicalObject(sf::Vector2f position, std::string path, float Mass = 100.f, std::string MaterialName = "");
		~CPhysicalObject();
	};
}

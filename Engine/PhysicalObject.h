#pragma once
#include "Actor.h"
#include "PhysicalMaterial.h"

namespace Engine
{
	//Basic CObject that simulates physics
	//Unlike all other CActor's children it is supossed to have functionality to play sounds on hit etc.
	class PhysicalObject : public CActor
	{
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

		void Init(std::string path)override;

		PhysicalObject(sf::Vector2f position, std::string path, std::string MaterialName = "");
		~PhysicalObject();
	};
}

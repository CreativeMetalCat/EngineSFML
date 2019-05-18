#pragma once
#ifndef SFML_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#endif

namespace Engine::Animation::Skeletal
{
	struct Bone
	{
	public:
		sf::Vector2f Location;

		sf::Vector2f Scale;

		float Rotation;

		std::string Name;

		std::string ParentName = "root";

		Bone(sf::Vector2f Location, sf::Vector2f Scale, float Rotation, std::string Name, std::string ParentName) :Location(Location), Scale(Scale), Rotation(Rotation), Name(Name), ParentName(ParentName) {}
	};


	struct BoneAnimation
	{
		
	public:
		float PassedTime = 0.f;

		unsigned int currentId;

		std::vector<float> Duration;

		std::string BoneName;

		std::vector<sf::Vector2f> EndLocation;

		std::vector<sf::Vector2f> EndScale;

		std::vector<float> EndRotattion;

		BoneAnimation(std::string BoneName, std::vector<sf::Vector2f> EndLocation, std::vector<float> EndRotattion, std::vector<float> Duration, std::vector<sf::Vector2f> EndScale = { { 1.f,1.f } }) :Duration(Duration), BoneName(BoneName), EndLocation(EndLocation), EndRotattion(EndRotattion), EndScale(EndScale) {}

		void Update(sf::Time dt);
	};

	class CSkeletalMesh
	{
	public:

		std::string Name;

		std::vector<Bone>Bones;

		float Duration = 0.f;

		CSkeletalMesh();
		
		~CSkeletalMesh();
	};
}


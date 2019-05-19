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
	protected:
		sf::Vector2f m_direction = sf::Vector2f(0, 0);
		
		float m_pixelPerSecondSpeed = 0.f;
	public:
		sf::Vector2f TransformVector = sf::Vector2f(0, 0);

		float PassedTime = 0.f;

		unsigned int currentId = 0;

		std::vector<float> Duration;

		std::string BoneName;

		std::vector<sf::Vector2f> EndLocation;

		std::vector<sf::Vector2f> EndScale;

		std::vector<float> EndRotation;

		BoneAnimation(std::string BoneName, std::vector<sf::Vector2f> EndLocation, std::vector<float> EndRotation, std::vector<float> Duration, std::vector<sf::Vector2f> EndScale = { { 1.f,1.f } }) :Duration(Duration), BoneName(BoneName), EndLocation(EndLocation), EndRotation(EndRotation), EndScale(EndScale) {}

		void Start(Bone& bone);

		void Update(sf::Time dt, Bone& bone);
	};

	class CSkeletalMeshAnimation
	{
	public:
		std::vector<BoneAnimation>BoneAnimations;

		std::string Name;

		float Duration;

		CSkeletalMeshAnimation(std::vector<BoneAnimation>BoneAnimations, std::string Name, float Duration) :
			BoneAnimations(BoneAnimations),
			Name(Name),
			Duration(Duration)
		{}

		void Start(std::vector<Bone>& Bones);

		void Update(sf::Time dt, std::vector<Bone>& Bones);
	};

	class CSkeletalMesh
	{
	public:

		std::string Name;

		std::vector<Bone>Bones;

		CSkeletalMesh(std::string Name,std::vector<Bone>Bones);
		
		~CSkeletalMesh();
	};
}


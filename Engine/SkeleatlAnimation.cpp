#include "SkeleatlAnimation.h"

namespace Engine::Animation::Skeletal
{


	CSkeletalMesh::CSkeletalMesh()
	{
	}


	CSkeletalMesh::~CSkeletalMesh()
	{
		Bones.~vector();
	}
	void BoneAnimation::Update(sf::Time dt)
	{
		PassedTime += dt.asSeconds();
		if (PassedTime > Duration[currentId])
		{ 
			if (currentId + 1u < Duration.size()) { currentId++; }		
		}
	}
}
#include "SkeletalAnimation.h"
#ifndef CHIPMUNK_H
#include <chipmunk.h>
#endif

namespace Engine::Animation::Skeletal
{


	CSkeletalMesh::CSkeletalMesh(std::string Name, std::vector<Bone> Bones) :Name(Name), Bones(Bones)
	{
	}


	CSkeletalMesh::~CSkeletalMesh()
	{
		Bones.~vector();
	}
	void BoneAnimation::Start(Bone& bone)
	{
		this->TransformVector = EndLocation[currentId] - bone.Location;
		m_pixelPerSecondSpeed = Duration[currentId] / cpvlength(cpv((cpFloat)this->TransformVector.x, (cpFloat)this->TransformVector.y));

		m_direction.x = cosf(cpvtoangle(cpv((cpFloat)this->TransformVector.x, (cpFloat)this->TransformVector.y)));
		m_direction.y = sinf(cpvtoangle(cpv((cpFloat)this->TransformVector.x, (cpFloat)this->TransformVector.y)));
	}
	void BoneAnimation::Update(sf::Time dt, Bone& bone)
	{
		PassedTime += dt.asSeconds();
		if (PassedTime > Duration[currentId])
		{
			bone.Location = EndLocation[currentId];
			if (currentId + 1u < Duration.size())
			{
				currentId++;
				this->TransformVector = EndLocation[currentId] - bone.Location;
				m_pixelPerSecondSpeed = cpvlength(cpv((cpFloat)this->TransformVector.x, (cpFloat)this->TransformVector.y)) / Duration[currentId];

				m_direction.x = cosf(cpvtoangle(cpv((cpFloat)this->TransformVector.x, (cpFloat)this->TransformVector.y)));
				m_direction.y = sinf(cpvtoangle(cpv((cpFloat)this->TransformVector.x, (cpFloat)this->TransformVector.y)));

				PassedTime = 0.f;
			}			
		}
		if (!(roundf(this->EndLocation[currentId].x) == roundf(bone.Location.x) && roundf(this->EndLocation[currentId].y) == roundf(bone.Location.y)))
		{
			bone.Location += (m_pixelPerSecondSpeed * dt.asSeconds())* m_direction;
		}
	}
	void CSkeletalMeshAnimation::Start(std::vector<Bone>& Bones)
	{
		for (auto& boneAnim : this->BoneAnimations)
		{
			for (auto& bone : Bones)
			{
				if (boneAnim.BoneName == bone.Name)
				{
					//because we start animation id must be 0
					boneAnim.Start(bone);
				}
			}
		}
	}
	void CSkeletalMeshAnimation::Update(sf::Time dt, std::vector<Bone>& Bones)
	{
		for (auto& anim : this->BoneAnimations)
		{
			for (auto& bone : Bones)
			{
				if (bone.Name == anim.BoneName)
				{
					anim.Update(dt,bone);
					break;
				}
			}
			
		}
	}
}
#pragma once

#ifndef BOX2D_H
#include <Box2D.h>
#endif

class DynamicBodyRaycastCallback : public b2RayCastCallback
{
public:
	bool m_hit;
	b2Vec2 m_point;
	b2Vec2 m_normal;
	b2Body* m_body;

	DynamicBodyRaycastCallback();
	~DynamicBodyRaycastCallback();

	

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)override
	{
		b2Body* body = fixture->GetBody();
		void* userData = body->GetUserData();
		

		m_hit = true;
		m_point = point;
		m_normal = normal;
		m_body = body;

		return fraction;
	}

	
};


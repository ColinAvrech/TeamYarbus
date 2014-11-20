/******************************************************************************/
/*!
\file   PointCollider.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ComponentInclude.h"
#include "IncludeForAllCollision.h"
#include "EventSystem.h"
#include "CollisionRepeats.h"
#include "Resolution.h"
#include "CollisionEvent.h"

namespace Framework
{
	//serialize, initialize, update
	void PointCollider::Serialize(Serializer::DataNode* data)
	{
		return;
	}

	void PointCollider::Initialize()
	{
		gameObject->ShapeCollider = this;
	}
	void PointCollider::Update()
	{

	}

	//collision
	void PointCollider::DetectCollision(CircleCollider* c)
	{
		/*vec3 ppos = getPosition();
		vec3 cpos = c->getPosition();
		float rad = c->GetRadius();
		if (Physics::CirclevsPoint(rad, cpos, ppos))
		{
			CollisionEvent collision;
			collision.OtherObject = c->gameObject;
			collision.normal = cpos - ppos;
			glm::normalize(collision.normal);
		}*/
	}

	void PointCollider::DetectCollision(LineCollider* l)
	{
		/*vec3 pos = getPosition();
		if (Physics::PointvsLine(pos, l))
		{
			CollisionEvent collision;
			collision.OtherObject = l->gameObject;
			collision.normal = l->normalVec;
			glm::normalize(collision.normal);
		}*/
	}
	DefineComponentName(PointCollider);
}
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

namespace Framework
{
	//serialize, initialize, update
	void PointCollider::Serialize()
	{
		return;
	}

	void PointCollider::Initialize()
	{
    gameObject->PointCollider = this;
	}
	void PointCollider::Update()
	{
		
	}

	//collision
	void PointCollider::DetectCircle(CircleCollider* c)
	{
		glm::vec2 ppos = getPosition();
		glm::vec2 cpos = c->getPosition();
		float rad = c->GetRadius();
		if (Physics::CirclevsPoint(rad, cpos, ppos))
		{
      CollisionEvent collision;
			collision.OtherObject = c->gameObject;
			collision.normal = cpos - ppos;
			glm::normalize(collision.normal);
		}
	}

	void PointCollider::DetectLine(LineCollider* l)
	{
		glm::vec2 pos = getPosition();
		if (Physics::PointvsLine(pos, *l))
		{
      CollisionEvent collision;
			collision.OtherObject = l->gameObject;
			collision.normal = l->normalVec;
			glm::normalize(collision.normal);
		}
	}
	DefineComponentName(PointCollider);
}
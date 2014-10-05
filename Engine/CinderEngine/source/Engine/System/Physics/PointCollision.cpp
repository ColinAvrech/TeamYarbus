/******************************************************************************/
/*!
\file   PointCollision.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ShapeCollider.h"
#include "CollisionRepeats.h"
#include "CollisionEvent.h"
#include "EventSystem.h"
#include "ObjectSystem.h"

namespace Framework
{
	void PointCollider::DetectCircle(CircleCollider* c)
	{
		glm::vec2 ppos = getPosition();
		glm::vec2 cpos = c->getPosition();
		float rad = c->GetRadius();
		if (Physics::CirclevsPoint(rad, cpos, ppos))
		{
			std::string ColEvent = std::string("COLLISION");
			CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
			collision->OtherObject = c->Base;
			collision->normal = cpos - ppos;
			glm::normalize(collision->normal);
			collision->DispatchEvent();
		}
	}

	void PointCollider::DetectLine(LineCollider* l)
	{
		glm::vec2 pos = getPosition();
		if (Physics::PointvsLine(pos, *l))
		{
			std::string ColEvent = std::string("COLLISION");
			CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
			collision->OtherObject = l->Base;
			collision->normal = l->GetNormal();
			glm::normalize(collision->normal);
			collision->DispatchEvent();
		}
	}
}

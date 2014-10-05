/******************************************************************************/
/*!
\file   CircleCollision.cpp
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
	void CircleCollider::DetectCircle(CircleCollider* c)
	{
		// not counting offset
		glm::vec2 pos;
		pos.x = static_cast<Transform*>(Base->Transform)->position.x;
		pos.y = static_cast<Transform*>(Base->Transform)->position.y;
		glm::vec2 cpos;
		cpos.x = static_cast<Transform*>(c->Base->Transform)->position.x;
		cpos.y = static_cast<Transform*>(c->Base->Transform)->position.y;
		float rad = GetRadius() + c->GetRadius();
		float dist = Physics::Distance(pos, cpos);
		if (rad >= dist)
		{
			std::string ColEvent = std::string("COLLISION");
			CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
			collision->OtherObject = c->Base;
			collision->normal = pos - cpos;
			glm::normalize(collision->normal);
			collision->DispatchEvent();
			std::cout << "COLLISION" << std::endl;
		}
	}

	//repeat
	void CircleCollider::DetectPoint(PointCollider* p)
	{
		glm::vec2 ppos = p->getPosition();
		glm::vec2 pos = getPosition();
		float rad = GetRadius();
		if (Physics::CirclevsPoint(rad, pos, ppos))
		{
			std::string ColEvent = std::string("COLLISION");
			CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
			collision->OtherObject = p->Base;
			collision->normal = pos - ppos;
			glm::normalize(collision->normal);
			collision->DispatchEvent();
		}
	}

	void CircleCollider::DetectLine(LineCollider* l)
	{
		float rad = GetRadius();
		glm::vec2 pos = getPosition();
		if (Physics::CirclevsLine(rad, pos, *l))
		{
			std::string ColEvent = std::string("COLLISION");
			CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
			collision->OtherObject = l->Base;
			collision->normal = l->GetNormal();
			glm::normalize(collision->normal);
			collision->DispatchEvent();
		}
	}
} //Framework

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
#include "Resolution.h"
#include "ObjectSystem.h"

namespace Framework
{
	void CircleCollider::DetectCircle(CircleCollider* c)
	{
//<<<<<<< HEAD
		// not counting offset
		glm::vec2 pos;
		pos.x = static_cast<Transform*>(gameObject->Transform)->GetPosition ().x;
		pos.y = static_cast<Transform*>(gameObject->Transform)->GetPosition ().y;
		glm::vec2 cpos;
		cpos.x = static_cast<Transform*>(c->gameObject->Transform)->GetPosition ().x;
		cpos.y = static_cast<Transform*>(c->gameObject->Transform)->GetPosition ().y;
		float rad = GetRadius() + c->GetRadius();
		float dist = Physics::Distance(pos, cpos);
		if (rad >= dist)
		{
			std::string ColEvent = std::string("CIRCLECOLLISION");
			CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
			if (collision)
			{
				collision->OtherObject = c->gameObject;
				collision->normal = pos - cpos;
				glm::normalize(collision->normal);
				collision->DispatchEvent();
				std::cout << "COLLISION" << std::endl;
			}
		}
//=======
    // CODE IS BREAKING
    // FIX
		//// not counting offset
		//glm::vec2 pos;
		//pos.x = static_cast<Transform*>(gameObject->Transform)->GetPosition ().x;
		//pos.y = static_cast<Transform*>(gameObject->Transform)->GetPosition ().y;
		//glm::vec2 cpos;
		//cpos.x = static_cast<Transform*>(c->gameObject->Transform)->GetPosition ().x;
		//cpos.y = static_cast<Transform*>(c->gameObject->Transform)->GetPosition ().y;
		//float rad = GetRadius() + c->GetRadius();
		//float dist = Physics::Distance(pos, cpos);
		//if (rad >= dist)
		//{
		//	std::string ColEvent = std::string("COLLISION");

    //////////////////////////////////////////////////////////////////////////
    // CODE BREAKS HERE
		//	CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
    //////////////////////////////////////////////////////////////////////////
		//	collision->OtherObject = c->gameObject;
		//	collision->normal = pos - cpos;
		//	glm::normalize(collision->normal);
		//	collision->DispatchEvent();
		//	std::cout << "COLLISION" << std::endl;
		//}
//>>>>>>> origin/master
	}

	//repeat
	void CircleCollider::DetectPoint(PointCollider* p)
	{
		glm::vec2 ppos = p->getPosition();
		glm::vec2 pos = getPosition();
		float rad = GetRadius();
		if (Physics::CirclevsPoint(rad, pos, ppos))
		{
////<<<<<<< HEAD
//      //std::string ColEvent = CollisionEventName(p->gameObject->GameObjectID);
//			std::string ColEvent = std::string("POINTCOLLISION");
////=======
			std::string ColEvent = std::string("COLLISION");
//>>>>>>> origin/master
			CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
			collision->OtherObject = p->gameObject;
			collision->normal = pos - ppos;
			glm::normalize(collision->normal);
			collision->DispatchEvent();
		}
	}

	void CircleCollider::DetectLine(LineCollider* l)
	{
		float rad = GetRadius();
		glm::vec2 pos;
		pos.x = static_cast<Transform*>(gameObject->Transform)->GetPosition().x;
		pos.y = static_cast<Transform*>(gameObject->Transform)->GetPosition().y;
		if (Physics::CirclevsLine(rad, pos, *l))
		{
//<<<<<<< HEAD
			std::string ColEvent = std::string("LINECOLLISION");
			CollisionEvent* collision = new CollisionEvent(ColEvent);
			if (collision)
			{
				collision->OtherObject = l->gameObject;
				collision->thisObject = this->gameObject;
				collision->normal = l->normalVec;
				collision->normal = glm::normalize(collision->normal);
				collision->DispatchEvent();
			}
			Physics::Resolve(collision);
////=======
//			std::string ColEvent = std::string("COLLISION");
//			CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
//			collision->OtherObject = l->gameObject;
//			//collision->normal = l->GetNormal();
//			glm::normalize(collision->normal);
//			collision->DispatchEvent();
////>>>>>>> origin/master
		}
	}
} //Framework

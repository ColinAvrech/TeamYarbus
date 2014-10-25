/******************************************************************************/
/*!
\file   CircleCollider.cpp
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
	void CircleCollider::Serialize()
	{
		return;
	}

	void CircleCollider::Initialize()
	{
    gameObject->CircleCollider = this;
	}

	void CircleCollider::Update()
	{

	}

	//collision detection
	void CircleCollider::DetectCircle(CircleCollider* c)
	{
		// not counting offset
		glm::vec2 pos;
		pos.x = static_cast<Transform*>(gameObject->Transform)->GetPosition().x;
		pos.y = static_cast<Transform*>(gameObject->Transform)->GetPosition().y;
		glm::vec2 cpos;
		cpos.x = static_cast<Transform*>(c->gameObject->Transform)->GetPosition().x;
		cpos.y = static_cast<Transform*>(c->gameObject->Transform)->GetPosition().y;
		float rad = GetRadius() + c->GetRadius();
		float dist = Physics::Distance(pos, cpos);
		if (rad >= dist)
		{
      CollisionEvent collision;
			//if (collision)
			//{
      //  //Trigger Event
			//}
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
      CollisionEvent collision;
			collision.OtherObject = p->gameObject;
			collision.normal = pos - ppos;
			glm::normalize(collision.normal);
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
			CollisionEvent collision;
			collision.OtherObject = l->gameObject;
			collision.thisObject = this->gameObject;
			collision.normal = l->normalVec;
			collision.normal = glm::normalize(collision.normal);

			Physics::Resolve(&collision);
		}
	}

	DefineComponentName(CircleCollider);
}
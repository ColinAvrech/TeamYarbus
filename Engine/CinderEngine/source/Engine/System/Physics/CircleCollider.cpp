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
#include "CollisionEvent.h"
#include "PhysicsSystem.h"
#include "JSONSerializer.h"

namespace Framework
{
	//serialize, initialize, update
	void CircleCollider::Serialize(Serializer::DataNode* data)
	{
		Serializer::DataNode* temp;
		temp = data->FindElement(data, "Radius");
		temp->GetValue(&radius);

		//Material Properties
		temp = data->FindElement(data, "MaterialName");
		std::string name;
		temp->GetValue(&name);

		SerializeMaterial(name.c_str());
	}

	void CircleCollider::Initialize()
	{
		gameObject->ShapeCollider = this;
		Physics::PHYSICSSYSTEM->CircleColliders.push_back(gameObject);
	}

	void CircleCollider::Update()
	{

	}

	void CircleCollider::DetectCollision(SplineCollider* s)
	{
		std::vector <LineCollider*> splineArray = s->Get_Colliders();
		for (auto it = splineArray.begin(); it != splineArray.end(); ++it)
		{
			DetectCollision(*it);
		}
	}

	//collision detection
	void CircleCollider::DetectCollision(CircleCollider* c)
	{
		// not counting offset
		glm::vec3 pos;
		pos.x = (gameObject->Transform)->GetPosition().x;
		pos.y = (gameObject->Transform)->GetPosition().y;
		glm::vec3 cpos;
		cpos.x = (c->gameObject->Transform)->GetPosition().x;
		cpos.y = (c->gameObject->Transform)->GetPosition().y;
		float rad = GetRadius() + c->GetRadius();
		float dist = Physics::Distance(pos, cpos);
		if (rad >= dist)
		{
			glm::vec3 normalVec = gameObject->Transform->GetPosition() -
				c->gameObject->Transform->GetPosition();
			float penetration = rad - dist;
			glm::normalize(normalVec);
			BodyContact contact(gameObject->RigidBody, c->gameObject->RigidBody,
				normalVec, penetration);
			Physics::PHYSICSSYSTEM->addContact(&contact);

			//CollisionEvent collision;
			// EVENTSYSTEM->TriggerEvent(CollisionEventName(gameObject->GameObjectID), collision);
		}
	}

	//repeat
	void CircleCollider::DetectCollision(PointCollider* p)
	{
		glm::vec3 ppos = p->getPosition();
		glm::vec3 pos = getPosition();
		float rad = GetRadius();

		if (Physics::CirclevsPoint(rad, pos, ppos))
		{
			glm::vec3 normalVec = gameObject->Transform->GetPosition() -
				p->gameObject->Transform->GetPosition();
			float penetration = rad - Physics::Distance(gameObject->Transform->GetPosition(),
				p->gameObject->Transform->GetPosition());
			glm::normalize(normalVec);

			BodyContact contact(gameObject->RigidBody, p->gameObject->RigidBody,
				normalVec, penetration);
			//CollisionEvent collision;
			//collision.OtherObject = p->gameObject;
			//collision.normal = pos - ppos;
			//glm::normalize(collision.normal);
		}
	}

	void CircleCollider::DetectCollision(LineCollider* l)
	{
		//float rad = GetRadius();
		float rad = radius;
		float penetration;
		glm::vec3 pos;
		pos.x = gameObject->Transform->GetPosition().x;
		pos.y = gameObject->Transform->GetPosition().y;
		if (gameObject->RigidBody)
		{
			pos.x += gameObject->RigidBody->vel.x * 0.016f;
			pos.y += gameObject->RigidBody->vel.y * 0.016f;
		}

		if (((pos.x > l->p1.x && pos.x > l->p2.x) || (pos.x < l->p1.x && pos.x < l->p2.x)) &&
			((pos.y > l->p1.y && pos.y > l->p2.y) || (pos.y < l->p1.y && pos.y < l->p2.y)))
			return;

		penetration = Physics::CirclevsLine(rad, pos, l);
		if (penetration >= 0)
		{
			BodyContact contact(gameObject->RigidBody, nullptr,
				l->normalVec, penetration);
			Physics::PHYSICSSYSTEM->addContact(&contact);
			//CollisionEvent collision;
			//collision.Penetration = penetration;
			//collision.OtherObject = l->gameObject;
			//collision.thisObject = this->gameObject;
			//collision.normal = l->normalVec;
			//collision.normal = glm::normalize(collision.normal);
		}
	}

	DefineComponentName(CircleCollider);
}

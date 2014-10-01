#include "ColliderShape.h"
#include "Collision.h"
#include "ObjectSystem.h"

namespace Framework
{
	namespace Physics
	{
		bool Circle::DetectCircle(Circle c)
		{
			/*static_cast<Transform*>(Base->AddComponent(RigidBody::Name))->

			Manifold m;
			m.massA = this->gameObject->GetComponent("RigidBody");*/
			//send message

			// not counting offset
			Vec2 pos = getPosition();
			Vec2 cpos = c.getPosition();
			float rad = GetRadius() + c.GetRadius();
			float dist = Distance(pos, cpos);
			return rad >= dist;
		}

		bool Circle::DetectPoint(Vec2 p)
		{
			Vec2 pos = getPosition();
			float rad = GetRadius();
			return CirclevsPoint(rad, pos, p);

		}
		bool Circle::DetectLine(LineSegment l)
		{
			float rad = GetRadius();
			Vec2 pos = getPosition();
			return CirclevsLine(rad, pos, l);
		}
	} //Physics
} //Framework

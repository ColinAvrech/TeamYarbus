#include "ColliderShape.h"
#include "Collision.h"
#include "CollisionEvent.h"
#include "EventSystem.h"
#include "ObjectSystem.h"

namespace Framework
{
	namespace Physics
	{
		void Point::DetectCircle(Circle c)
		{
			Vec2 ppos = getPosition();
			Vec2 cpos = c.getPosition();
			float rad = c.GetRadius();
			if (CirclevsPoint(rad, cpos, ppos))
			{
				std::string ColEvent = std::string("COLLISION");
				CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
				collision->OtherObject = c.Base;
				collision->normal = cpos - ppos;
				collision->normal.normalize();
				collision->DispatchEvent();
			}
		}

		void Point::DetectLine(LineSegment l)
		{
			Vec2 pos = getPosition();
			if (PointvsLine(pos, l))
			{
				std::string ColEvent = std::string("COLLISION");
				CollisionEvent* collision = (CollisionEvent*)EVENTSYSTEM->GetEvent(ColEvent);
				collision->OtherObject = l.Base;
				collision->normal = l.GetNormal();
				collision->normal.normalize();
				collision->DispatchEvent();
			}
		}
	}
}

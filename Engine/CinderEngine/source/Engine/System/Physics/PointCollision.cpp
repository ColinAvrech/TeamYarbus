#include "ColliderShape.h"
#include "Collision.h"

namespace Framework
{
	namespace Physics
	{
		bool Point::DetectCircle(Circle c)
		{
			Vec2 ppos = getPosition();
			Vec2 cpos = c.getPosition();
			float rad = c.GetRadius();
			return CirclevsPoint(rad, cpos, ppos);
		}

		bool Point::DetectLine(LineSegment l)
		{
			Vec2 pos = getPosition();
			return PointvsLine(pos, l);
		}
	}
}

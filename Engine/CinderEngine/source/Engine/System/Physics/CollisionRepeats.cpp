#include "Collision.h"
#include "ColliderShape.h"
#include "PhysicsLibrary.h"

namespace Framework
{
	namespace Physics
	{
		bool CirclevsLine(float rad, const Vec2& pos, LineSegment& l)
		{
			//Make sure buildlinesegment (private memb fuct) composes the right normals
			float ldotProduct = l.GetP1DotNormal();
			Vec2 normVec = l.GetNormal();
			normVec.normalize();
			float dist = ldotProduct - DotProduct(normVec, pos);

			return rad >= dist;
		}

		bool CirclevsPoint(const float rad, const Vec2& cpos, const Vec2& ppos)
		{
			return rad >= Distance(cpos, ppos);
		}

		bool PointvsLine(const Vec2& pos, LineSegment& l)
		{
			float ldotProduct = l.GetP1DotNormal();
			Vec2 normVec = l.GetNormal();
			normVec.normalize();
			float dist = ldotProduct - DotProduct(normVec, pos);

			return dist <= 0;
		}
	}
}

#pragma once

#include "ColliderShape.h"

namespace Framework
{
	namespace Physics
	{
		bool CirclevsLine(float rad, const Vec2& pos, LineSegment& l);
		bool CirclevsPoint(const float rad, const Vec2& cpos, const Vec2& ppos);
		bool PointvsLine(const Vec2& pos, LineSegment& l);
	}
}

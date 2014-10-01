
#pragma once

#include "Vec2.h"

namespace Framework
{
	namespace Physics
	{
		float Distance(const Vec2& pos1, const Vec2& pos2);
		float DotProduct(const Vec2& vec1, const Vec2& vec2);
		Vec2 applyAccel(Vec2 accel, double dt);
		Vec2 getAccel(Vec2& force, double mass);
		Vec2 getReflection(Vec2& normal, Vec2& velocity);
	} //Physics
} //Framework

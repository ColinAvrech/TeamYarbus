/******************************************************************************/
/*!
\file   CollisionRepeats.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once

#include "IncludeForAllCollision.h"

namespace Framework
{
	namespace Physics
	{
		float CirclevsLine(const float rad, vec3& pos, LineCollider* l);
		bool CirclevsPoint(const float rad, vec3& cpos, vec3& ppos);
		bool PointvsLine(vec3& pos, LineCollider& l);
	}
}

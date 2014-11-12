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
		float CirclevsLine(const float rad, glm::vec3& pos, LineCollider* l);
		bool CirclevsPoint(const float rad, glm::vec3& cpos, glm::vec3& ppos);
		bool PointvsLine(glm::vec3& pos, LineCollider& l);
	}
}

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
		float CirclevsLine(const float rad, glm::vec2& pos, LineCollider& l);
		bool CirclevsPoint(const float rad, glm::vec2& cpos, glm::vec2& ppos);
		bool PointvsLine(glm::vec2& pos, LineCollider& l);
	}
}

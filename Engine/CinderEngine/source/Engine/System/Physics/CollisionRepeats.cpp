/******************************************************************************/
/*!
\file   CollisionRepeats.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "CollisionRepeats.h"
#include "PhysicsLibrary.h"

namespace Framework
{
	namespace Physics
	{
		float CirclevsLine(const float rad, glm::vec2& pos, LineCollider& l)
		{
			//Make sure buildlinesegment (private memb fuct) composes the right normals
			//float penetration;
			float ldotProduct = l.p1dotNormal;
			glm::vec2 normVec = l.normalVec;
			normVec = glm::normalize(normVec);
			float dist = ldotProduct - DotProduct(normVec, pos);
			if (dist < 0)
				dist = -dist;

			//penetration
			return rad - dist;
		}

		//change these to return float -- penetration
		bool CirclevsPoint(const float rad, glm::vec2& cpos, glm::vec2& ppos)
		{
			return rad >= Distance(cpos, ppos);
		}

		bool PointvsLine(glm::vec2& pos, LineCollider& l)
		{
			float ldotProduct = l.p1dotNormal;
			glm::vec2 normVec = l.normalVec;
			glm::normalize(normVec);
			float dist = ldotProduct - DotProduct(normVec, pos);

			return dist <= 0;
		}
	}
}
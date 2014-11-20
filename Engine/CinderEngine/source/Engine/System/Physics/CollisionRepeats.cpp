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
		float CirclevsLine(const float rad, vec3& pos, LineCollider* l)
		{
			//float penetration;
			float ldotProduct = l->p1dotNormal;
			vec3 normVec = l->normalVec;
			normVec = glm::normalize(normVec);
			float dist = ldotProduct - DotProduct(normVec, pos);

			/*
			//Direction vector
			vec3 dirVec;
			dirVec.x = l->p2.x - l->p1.x;
			dirVec.y = l->p2.x - l->p1.y;

			//Vector from center sphere to ray start
			vec3 csrt;
			csrt.x = l->p1.x - pos.x;
			csrt.y = l->p1.y - pos.y;

			//quadratic formula
			float a = Physics::DotProduct(dirVec, dirVec);
			float b = 2 * Physics::DotProduct(csrt, dirVec);
			float c = Physics::DotProduct(csrt, dirVec) - (rad * rad);

			float discriminant = b * b - 4 * a * c;

			if (discriminant < 0)
			{
				//no intersection
				//still return float penetration
			}
			else
			{
				//ray did not completely miss sphere -- there is a solution
				discriminant = sqrt(discriminant);

				float t1 = (-b - discriminant) / (2 * a);
				float t2 = (-b + discriminant) / (2 * a);


				if (t1 >= 0 && t1 <= 1)
				{
					//t1 is the intersection & is closer than t2
					return 
				}
			}
		*/
			if (dist < 0)
				dist = -dist;

			//penetration
			return rad - dist;
		}

		//change these to return float -- penetration
		bool CirclevsPoint(const float rad, vec3& cpos, vec3& ppos)
		{
			return rad >= Distance(cpos, ppos);
		}

		bool PointvsLine(vec3& pos, LineCollider& l)
		{
			float ldotProduct = l.p1dotNormal;
			vec3 normVec = l.normalVec;
			glm::normalize(normVec);
			float dist = ldotProduct - DotProduct(normVec, pos);

			return dist <= 0;
		}
	}
}
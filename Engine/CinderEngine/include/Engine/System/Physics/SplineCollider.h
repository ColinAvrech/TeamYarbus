/******************************************************************************/
/*!
\file   LineCollider.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ShapeCollider.h"

#pragma once
namespace Framework
{
	class PointCollider;
	class CircleCollider;

	class SplineCollider : public ShapeCollider
	{
	public:
		const static std::string Name;
		float p1dotNormal;
		glm::vec2 normalVec;
		glm::vec2 p1; //points
		glm::vec2 p2;

		SplineCollider(GameObject *obj)
		{
			gameObject = obj;
		}

		~SplineCollider(){};

		void Initialize();
		void Serialize();
		void Update();

		// Getters
		//glm::vec2 GetNormal(void) { return normalVec; }
		//float GetP1DotNormal(void) { return p1dotNormal; }

		// Setters
		void setP1(glm::vec2 passer) { p1 = passer; }
		void setP2(glm::vec2 passer) { p2 = passer; }

		// Private member functions
		void DetectCircle(CircleCollider* c);  //if player is a circle
		void DetectPoint(PointCollider* p);	  //if player is a point

	private:
		// Private data

	};
}
/******************************************************************************/
/*!
\file   LineCollider.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ShapeCollider.h"


namespace Framework
{
	class PointCollider;
	class CircleCollider;

	class LineCollider : public ShapeCollider
	{
	public:
		const static std::string Name;
		float p1dotNormal;
		glm::vec2 normalVec;
		glm::vec2 p1; //points
		glm::vec2 p2;

		LineCollider (GameObject * obj)
		{
			gameObject = obj;
		}

		LineCollider(){};

		~LineCollider(){};

		void Initialize();
		void Serialize(Serializer::DataNode* data);
		void Update();

		// Getters
		//glm::vec2 GetNormal(void) { return normalVec; }
		//float GetP1DotNormal(void) { return p1dotNormal; }

		// Setters
		void setP1(glm::vec2 passer) { p1 = passer; }
		void setP2(glm::vec2 passer) { p2 = passer; }

		// Private member functions
		void DetectCollision(CircleCollider* c);  //if player is a circle
		void DetectCollision(PointCollider* p);	  //if player is a point
		void DetectCollision(LineCollider* p);

	private:
		// Private data

	};
}
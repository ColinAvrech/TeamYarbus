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
		const static string Name;
		float p1dotNormal;
		vec3 normalVec;
		vec3 p1; //points
		vec3 p2;

    LineCollider (vec3 _p1, vec3 _p2);

		LineCollider (GameObject * obj)
		{
			gameObject = obj;
		}

		LineCollider(){};

		virtual ~LineCollider(){};

		virtual void Initialize();
		virtual void Initialize (vec3 _p1, vec3 _p2);
		virtual void Serialize(Serializer::DataNode* data);
		void Update();

		// Getters
		//vec3 GetNormal(void) { return normalVec; }
		//float GetP1DotNormal(void) { return p1dotNormal; }

		// Setters
		void setP1(vec3 passer) { p1 = passer; }
		void setP2(vec3 passer) { p2 = passer; }

		// Private member functions
		void DetectCollision(CircleCollider* c);  //if player is a circle
		void DetectCollision(PointCollider* p);	  //if player is a point
		void DetectCollision(LineCollider* p);

	private:
		// Private data

	};
}
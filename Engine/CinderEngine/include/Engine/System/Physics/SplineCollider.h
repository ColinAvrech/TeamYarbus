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

	class SplineCollider : public ShapeCollider
	{
	public:
		//ZilchDeclareDerivedType(SplineCollider, ShapeCollider);
		const static string Name;
		float p1dotNormal;
		vec3 normalVec;
		vec3 p1; //points
		vec3 p2;

    SplineCollider (){}
		SplineCollider(GameObject *obj)
		{
			gameObject = obj;
		}

		virtual ~SplineCollider(){};

		virtual void Initialize();
		virtual void Serialize(Serializer::DataNode*);
		void Update();

    void AddLineCollider (vec2 _p1, vec2 _p2);
    void AddLineCollider (std::vector <std::pair <vec2, vec2>>& lines);
		// Getters
		//vec2 GetNormal(void) { return normalVec; }
		//float GetP1DotNormal(void) { return p1dotNormal; }

    std::vector <LineCollider*>& Get_Colliders ()
    {
      return colliders;
    }

		// Setters
		void setP1(vec3 passer) { p1 = passer; }
		void setP2(vec3 passer) { p2 = passer; }

		// Private member functions
		void DetectCircle(CircleCollider* c);  //if player is a circle
		void DetectPoint(PointCollider* p);	  //if player is a point

    virtual void DetectCollision (CircleCollider * circle);
    virtual void DetectCollision (LineCollider * line);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // Debug Draw
    virtual void Draw ();
    //////////////////////////////////////////////////////////////////////////

	private:
		// Private data

    std::vector <LineCollider*> colliders;

	};
}
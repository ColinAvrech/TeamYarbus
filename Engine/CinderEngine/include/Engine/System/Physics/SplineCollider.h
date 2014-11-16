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
		const static string Name;
		float p1dotNormal;
		glm::vec3 normalVec;
		glm::vec3 p1; //points
		glm::vec3 p2;

    SplineCollider (){}
		SplineCollider(GameObject *obj)
		{
			gameObject = obj;
		}

		virtual ~SplineCollider(){};

		virtual void Initialize();
		virtual void Serialize(Serializer::DataNode*);
		void Update();

    void AddLineCollider (glm::vec2 _p1, glm::vec2 _p2);
    void AddLineCollider (std::vector <std::pair <glm::vec2, glm::vec2>>& lines);
		// Getters
		//glm::vec2 GetNormal(void) { return normalVec; }
		//float GetP1DotNormal(void) { return p1dotNormal; }

    std::vector <LineCollider*>& Get_Colliders ()
    {
      return colliders;
    }

		// Setters
		void setP1(glm::vec3 passer) { p1 = passer; }
		void setP2(glm::vec3 passer) { p2 = passer; }

		// Private member functions
		void DetectCircle(CircleCollider* c);  //if player is a circle
		void DetectPoint(PointCollider* p);	  //if player is a point

    virtual void DetectCollision (CircleCollider * circle);
    virtual void DetectCollision (LineCollider * line);

	private:
		// Private data

    std::vector <LineCollider*> colliders;

	};
}
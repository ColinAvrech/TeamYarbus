/******************************************************************************/
/*!
\file   ShapeCollider.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include "Common.h"
#include "Vec2.h"
#include "RigidBody.h"
#include "PhysicsLibrary.h"
#include "GraphicsCommon.h"

namespace Framework
{
	class ShapeCollider : public Component
	{
	public:
    const static std::string Name;
		// Getters
		glm::vec2 getPosition(void){
			return position;
		}
		glm::vec2 getOffset(void){
			return offset;
		}

	private:
		// Private data
		struct MaterialProperties
		{
			std::string name;
			float density;
			float restitution;
			float friction;
		};
		glm::vec2 position;
		glm::vec2 offset;
		bool ghost;
		bool sendsEvents;
	};
	////////////////////////////////////////////////////////////

  //Forward Declaration
  class PointCollider;
  class LineCollider;

  class CircleCollider : public ShapeCollider
  {
  public:
    const static std::string Name;
	CircleCollider(GameObject* obj) : radius(0.1f)
	{
		gameObject = obj;
	}

	~CircleCollider();

    /*!Telegraph that the component is active*/
    void Initalize ()
    {
      //PHYSICSSYSTEM->AddLineCollisder(this);
    }

    void Serialize ()
    {
    }

    // Getters
    float GetRadius (void){
      return radius;
    };

    void DetectCircle (CircleCollider* c);
    void DetectPoint (PointCollider* p);
    void DetectLine (LineCollider* l);

  private:
    // Private data
    float radius;
  };


	class PointCollider : public ShapeCollider
	{
	public:
		const static std::string Name;
		PointCollider(GameObject *obj);

		~PointCollider();

		/*!Telegraph that the component is active*/
		void Initalize()
		{
			//PHYSICSSYSTEM->AddLineCollisder(this);
		}

		void Serialize()
		{
		}

		void DetectCircle(CircleCollider* c);
		void DetectLine(LineCollider* l);
	};
	////////////////////////////////////////////////////////////

	// forward Declairation
	//class PointCollider;

	class LineCollider : public ShapeCollider
	{
	public:
		const static std::string Name;
		LineCollider(GameObject* obj);

		~LineCollider();

		// Public member functions - Overriden
		/*!Telegraph that the component is active*/
		void Initalize()
		{
			//PHYSICSSYSTEM->AddLineCollisder(this);
		}

		void Serialize()
		{
		}

		// Getters
		glm::vec2 GetNormal(void) { return normalVec; }
		float GetP1DotNormal(void) { return p1dotNormal; }

		// Private member functions
		void DetectCircle(CircleCollider* c);  //if player is a circle
		void DetectPoint(PointCollider* p);	  //if player is a point

	private:
		// Private data
		glm::vec2 p1; //points
		glm::vec2 p2;
		glm::vec2 normalVec;
		float p1dotNormal;
	};
	////////////////////////////////////////////////////////////
} //Framework


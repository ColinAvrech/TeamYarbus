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
#include "Events.h"

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

    void setOffset (glm::vec2 _offset)
    {
      offset = _offset;
    }

	protected:
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
    CircleCollider () { radius = 0.5f; }
	  CircleCollider(GameObject* obj) : radius(0.1f)
	  {
		  gameObject = obj;
	  }

	  ~CircleCollider() {};

    /*!Telegraph that the component is active*/
    virtual void Initialize ();
    //{
    //  //PHYSICSSYSTEM->AddLineCollisder(this);
    //}

    void Serialize (Serializer::DataNode* data)
    {
      //////////////////////////////////////////////////////////////////////////
      // Radius       : float
      // Offset       : glm::vec2
      // Ghost        : bool
      // Sends Events : bool
      //////////////////////////////////////////////////////////////////////////

      // Radius
      radius = data->value_.Float_;
      data = data->next;
      // Offset
      for (unsigned i = 0; i < data->value_.VecN_->size (); ++i)
        offset [i] = data->value_.VecN_->at (i);
      data = data->next;
      // Ghost?
      ghost = data->value_.Bool_;
      data = data->next;
      // Send Events?
      sendsEvents = data->value_.Bool_;
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
		PointCollider(GameObject *obj)
		{
			gameObject = obj;
		}

		~PointCollider(){};

		/*!Telegraph that the component is active*/
		void Initialize();

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
		float p1dotNormal;
		glm::vec2 normalVec;
		glm::vec2 p1; //points
		glm::vec2 p2;

		LineCollider(GameObject *obj)
		{
			gameObject = obj;
		}

		~LineCollider(){};

		// Public member functions - Overriden
		/*!Telegraph that the component is active*/
		void Initialize();

		void Serialize()
		{
		}

		// Getters
		//glm::vec2 GetNormal(void) { return normalVec; }
		//float GetP1DotNormal(void) { return p1dotNormal; }

		// Setters
		void setP1(glm::vec2 passer) { p1 = passer; }
		void setP2(glm::vec2 passer) { p2 = passer; }

		// Private member functions
		void DetectCircle(CircleCollider* c);  //if player is a circle
		void DetectPoint(PointCollider* p);	  //if player is a point

    // TODO GET NORMAL
    glm::vec2 GetNormal () { return glm::vec2 (0); }

	private:
		// Private data
		
	};
	////////////////////////////////////////////////////////////
} //Framework


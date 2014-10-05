/******************************************************************************/
/*!
\file   Shape.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The base shape class, and its derived classes: point, line & circle.
*/
/******************************************************************************/

#pragma once

#include "Common.h"
#include "Vec2.h"
#include "RigidBody.h"
#include "PhysicsLibrary.h"

namespace Framework
{
  namespace Physics
  {
    //contains collision data necessary for resolution
    //base class of event system - include in collision.cpp

    //////////////////////////////////////////////////////////////////////////
    // Collider Interface
    //////////////////////////////////////////////////////////////////////////

    class Shape : public Component
    {
    public:
      // Getters
      inline Vec2 getPosition(void){
        return position;
      }
      inline Vec2 getOffset(void){
        return offset;
      }
      inline Vec2 getVelocity(void){
        return velocity;
      }

      const static std::string Name;

    private:
      // Private data
      struct MaterialProperties
      {
        std::string name;
        float density;
        float restitution;
        float friction;
      };
      Vec2 position;
      Vec2 velocity;
      Vec2 offset;
      bool ghost;
      bool sendsEvents;
    };
    //////////////////////////////////////////////////////////////////////////

    //forward Declariation
    class Circle;
    class LineSegment;

    class Point : public Shape
    {
    public:
      Point()
      {}
      ~Point()
      {}

      /*!Telegraph that the component is active*/
      void Initalize()
      {
        //PHYSICSSYSTEM->AddLineCollisder(this);
      }

      void Serialize()
      {
      }

    private:
      // Private data
      void DetectCircle(Circle c);
      void DetectLine(LineSegment l);

      // Private member functions
      /*Vec2 ResolvePointvsCircle(Circle c);
      Vec2 ResolvePointvsLine(Vec2 Line);*/
    };
    //////////////////////////////////////////////////////////////////////////

    // forward Declairation
    class Point;

    //////////////////////////////////////////////////////////////////////////
    // Line Collider
    //////////////////////////////////////////////////////////////////////////

    class LineSegment : public Shape
    {
    public:
      LineSegment()
      {}
      ~LineSegment()
      {}

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
      inline Vec2 GetNormal(void)
      {
        return normalVec;
      }
      inline float GetP1DotNormal(void)
      {
        return p1dotNormal;
      }

    private:
      // Private data
      Vec2 p1; //points
      Vec2 p2;
      Vec2 normalVec;
      float p1dotNormal;

      // Private member functions
      void DetectCircle(Circle c);  //if player is a circle
      void DetectPoint(Point p);	  //if player is a point
      /*Vec2 ResolveLinevsCircle(Circle c);
      Vec2 ResolveLinevsPoint(Point p);*/
    };

    //////////////////////////////////////////////////////////////////////////
    // Circle Collider
    //////////////////////////////////////////////////////////////////////////

    class Circle : public Shape
    {
    public:
      Circle(GameObject* obj) : radius(1.0f)
      {
        gameObject = obj;
      }
      ~Circle()
      {}

      /*!Telegraph that the component is active*/
      virtual void Initalize ();
      virtual void Serialize ();

      // Getters
      inline float GetRadius(void)
      {
        return radius;
      }

      void DetectCircle(Circle* c);
      void DetectPoint(Point p);
      void DetectLine(LineSegment l);
      void SetRadius (float radius_);
    private:
      // Private data
      float radius;

      // Private member functions

      /*
      Vec2 ResolveCirclevsCircle(Circle c);
      Vec2 ResolveCirclevsPoint(Point p);
      Vec2 ResolveCirclevsLine(LineSegment l);
      */
    };
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

  } //Physics
} //Framework

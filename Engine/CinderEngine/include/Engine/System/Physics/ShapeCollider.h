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
  class DebugRenderer;
	class CircleCollider;
	class LineCollider;
	class ShapeCollider : public Component
	{
	public:
	//ZilchDeclareDerivedType(ShapeCollider, Component);
    const static string Name;
    ShapeCollider ();
    ~ShapeCollider ();
    void SerializeMaterial (const char* name);
    int numOfShapes;
		// Getters
		vec3 getPosition(void){
			return position;
		}
		vec3 getOffset(void){
			return offset;
		}
		float getDensity(void)
		{
			return material.density;
		}

		virtual void DetectCollision(CircleCollider * circle) = 0;
		virtual void DetectCollision(LineCollider * line) = 0;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // Debug Draw
    static DebugRenderer* dr;
    virtual void Draw ();
    //////////////////////////////////////////////////////////////////////////


	private:
		// Private data
		struct MaterialProperties
		{
			string name;
			float density;
			float restitution;
			float staticfriction;
			float dynamicfriction;
		};
		MaterialProperties material;
		vec3 position;
		vec3 offset;
		bool ghost;
		bool sendsEvents;
	};
} //Framework


/******************************************************************************/
/*!
\file   RigidBody2D.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#ifndef _RIGID_BODY_2D_H
#define _RIGID_BODY_2D_H

#include "MathExtensionLibrary.h"
#include "Component.h"

#include "MetaCreator.h"
#include "Material.h"

namespace Framework
{
	class ShapeCollider2D;

	class RigidBody2D : public Framework::Component
	{
	public:
		META_DECLARE( RigidBody2D );

    const static std::string Name;
    RigidBody2D();
    ~RigidBody2D ();
	
	  virtual void Serialize (Framework::Serializer::DataNode* data);
	  virtual void Initialize ();
	
	  void ApplyForce( const vec2& f );
    void ComputeMass();

	  void ApplyImpulse( const vec2& impulse, const vec2& contactVector )
	  {
	    velocity += invMass * impulse;
	    angularVelocity += invI * Cross( contactVector, impulse );
	  }

    void SetMaterial(string newMatName);
    void SetDensity(const float& newDensity);
    float GetArea();

	  void SetStatic( void )
	  {
	    I = 0.0f;
	    invI = 0.0f;
	    mass = 0.0f;
	    invMass = 0.0f;
	  }
	
	  void SetOrient( float radians );

	  vec2 velocity;
	  vec2 maxVelocity;
    vec2 acceleration;

	  float angularVelocity;
	  float torque;
	  float orient; // radians
	
	  vec2 force;
	
	  // Set by shape
	  float I;  // moment of inertia
	  float invI; // inverse inertia
	  float mass;  // mass
	  float invMass; // inverse masee
	
    bool isStatic;
    string matName;
    Material* mat;

	  // Store a color in RGB format
	  float r, g, b;
	};
}

META_DEFINE( Framework::RigidBody2D, RigidBody2D )
{
	TAG( "Component" );

  MEMBER(velocity);
  MEMBER(acceleration);
}

#endif // _RIGID_BODY_2D_H
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

namespace Framework
{
	class ShapeCollider2D;

	class RigidBody2D : public Framework::Component
	{
	public:
		META_DECLARE( RigidBody2D );

	  const static std::string Name;
	  RigidBody2D(ShapeCollider2D *shape_, float x, float y);
    ~RigidBody2D ();
    RigidBody2D (){}
	
	  virtual void Serialize (Framework::Serializer::DataNode* data);
    void SerializeMaterial (const char* name);
	  virtual void Initialize ();
	
	  void ApplyForce( const Vector2& f );
	
	  void ApplyImpulse( const Vector2& impulse, const Vector2& contactVector )
	  {
	    velocity += im * impulse;
	    angularVelocity += iI * Cross( contactVector, impulse );
	  }
	
	  void SetStatic( void )
	  {
	    I = 0.0f;
	    iI = 0.0f;
	    m = 0.0f;
	    im = 0.0f;
	  }
	
	  void SetOrient( float radians );

	  Vector2 position;
	  Vector2 velocity;
	  Vector2 maxVelocity;
	
	  float angularVelocity;
	  float torque;
	  float orient; // radians
	
	  Vector2 force;
	
	  // Set by shape
	  float I;  // moment of inertia
	  float iI; // inverse inertia
	  float m;  // mass
	  float im; // inverse masee
	
	  float staticFriction;
	  float dynamicFriction;
	  float restitution;
	
	  // Shape interface
	  ShapeCollider2D *shape;
	
	  // Store a color in RGB format
	  float r, g, b;
	};
}

META_DEFINE( Framework::RigidBody2D, RigidBody2D )
{
	MEMBER( position );
}

#endif // _RIGID_BODY_2D_H
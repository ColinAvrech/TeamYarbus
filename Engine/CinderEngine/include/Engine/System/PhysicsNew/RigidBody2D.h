#ifndef _RIGID_BODY_2D_H
#define _RIGID_BODY_2D_H

#include "MathExtensionLibrary.h"
#include "Component.h"

namespace Framework
{
	class ShapeCollider2D;

	class RigidBody2D : public Framework::Component
	{
	public:
	  const static std::string Name;
	  RigidBody2D(ShapeCollider2D *shape_, float x, float y);
	  RigidBody2D ();
	
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


#endif // _RIGID_BODY_2D_H
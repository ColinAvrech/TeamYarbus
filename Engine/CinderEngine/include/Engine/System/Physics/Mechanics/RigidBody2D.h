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
	  RigidBody2D(ShapeCollider2D *shape_, float x, float y);
    ~RigidBody2D ();
    RigidBody2D();
	
	  virtual void Serialize (Framework::Serializer::DataNode* data);
	  virtual void Initialize ();
	
	  void ApplyForce( const vec3& f );
    void ComputeMass();

	  void ApplyImpulse( const vec3& impulse, const vec3& contactVector )
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

	  vec3 position;
	  vec3 velocity;
	  vec3 maxVelocity;
    vec3 acceleration;

	  float angularVelocity;
	  float torque;
	  float orient; // radians
	
	  vec3 force;
	
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

	MEMBER( position );
}

#endif // _RIGID_BODY_2D_H
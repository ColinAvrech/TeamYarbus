#include "GameObject.h"
#include "RigidBody2D.h"
#include "Collider2D.h"

namespace Framework
{
  DefineComponentName (ShapeCollider2D);
  DefineComponentName (CircleCollider2D);
  DefineComponentName (PolygonCollider2D);
  DefineComponentName (RigidBody2D);


	RigidBody2D::RigidBody2D( ShapeCollider2D* shape_, float x, float y )
	  : shape( shape_->Clone( ) )
	{
    gameObject = nullptr;
	  shape->rigidBody = this;
	  position.Set( (float)x, (float)y );
	  velocity.Set( 0, 0 );
	  angularVelocity = 0;
	  torque = 0;
	  orient = MyRandom( -PI, PI );
	  force.Set( 0, 0 );
	  staticFriction = 1.0f;
	  dynamicFriction = 1.0f;
	  restitution = 0.2f;
	  shape->ComputeMass( 1000.0f );
	  r = MyRandom( 0.2f, 1.0f );
	  g = MyRandom( 0.2f, 1.0f );
	  b = MyRandom( 0.2f, 1.0f );
	}
	
	RigidBody2D::RigidBody2D ()
	{
	
	}
	
	
	void RigidBody2D::SetOrient( float radians )
	{
	  orient = radians;
	  shape->SetOrient( radians );
	}
	
	void RigidBody2D::Serialize (Framework::Serializer::DataNode* data)
	{
	
	}
	
	void RigidBody2D::Initialize ()
	{}
}


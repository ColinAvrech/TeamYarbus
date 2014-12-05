/******************************************************************************/
/*!
\file   RigidBody2D.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "GameObject.h"
#include "RigidBody2D.h"
#include "Collider2D.h"

namespace Framework
{
  DefineComponentName (ShapeCollider2D);
  DefineComponentName (CircleCollider2D);
  DefineComponentName (PolygonCollider2D);
  DefineComponentName (RigidBody2D);


	RigidBody2D::RigidBody2D(ShapeCollider2D* shape_, float x, float y)
	{
    gameObject = nullptr;
    shape = shape_;
	  shape_->rigidBody = this;
	  position.Set( (float)x, (float)y );
	  velocity.Set( 0, 0 );
	  maxVelocity.Set(10, 10);
	  angularVelocity = 0;
	  torque = 0;
	  orient = MyRandom( -PI, PI );
	  force.Set( 0, 0 );
	  staticFriction = 0.4f;
	  dynamicFriction = 0.4f;
	  restitution = 0.2f;
    if (shape->gameObject == nullptr)
      shape->ComputeMass( 1.0f );
	  r = MyRandom( 0.2f, 1.0f );
	  g = MyRandom( 0.2f, 1.0f );
	  b = MyRandom( 0.2f, 1.0f );
	}
	
	RigidBody2D::~RigidBody2D ()
	{
    if (gameObject != nullptr)
    {
      gameObject->RigidBody2D = nullptr;
    }
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

  void RigidBody2D::ApplyForce (const Vector2& f)
  {
    if (force.y < 0.1f)
      force += f;

    //std::cout << force.y << "\n";
  }

}


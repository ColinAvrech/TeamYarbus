/******************************************************************************/
/*!
\file   RigidBody.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#include "RigidBody.h"
#include "ComponentInclude.h"
#include "GameObject.h"
#include "IncludeForAllCollision.h"
#include "PhysicsLibrary.h"
#include "PhysicsSystem.h"

namespace Framework
{
  //Destructor
  RigidBody::~RigidBody()
  {
    gameObject->RigidBody = nullptr;
  }

  void RigidBody::Initialize()
  {
		//get transform to write results to

		//get starting position

		//add this body to body list
		Physics::PHYSICSSYSTEM->Bodies.push_back(this->gameObject);

		gameObject->RigidBody = this;
  }

  void RigidBody::Serialize(Serializer::DataNode* data)
  {
    //DynamicState
    bool Static, Kinematic;
    Serializer::DataNode* temp = data->FindElement(data, "Static");
    temp->GetValue(&Static);
    temp = data->FindElement(data, "Kinematic");
    temp->GetValue(&Kinematic);
    if (Static && !Kinematic)
      state = DynamicState::Static;
    else if (Kinematic && !Static)
      state = DynamicState::Kinematic;
    else if (!Kinematic && !Static)
      state = DynamicState::Dynamic;

    //Asleep
    bool AllowSleep;
    temp->FindElement(data, "AllowSleep");
    temp->GetValue(&AllowSleep);
    if (AllowSleep)
      allowSleep = true;
    else
      allowSleep = false;

    //Velocity
    glm::vec3 Velocity;
    temp = data->FindElement(data, "Velocity");
    temp->GetValue(&Velocity);
    vel.x = Velocity.x;
    vel.y = Velocity.y;

    //Angular Velocity
    glm::vec3 AngVel;
    temp = data->FindElement(data, "AngularVelocity");
    temp->GetValue(&AngVel);
    angVel.x = AngVel.x;
    angVel.y = AngVel.y;

    //Rotation
    bool RotationLocked;
    temp = data->FindElement(data, "RotationLocked");
    temp->GetValue(&RotationLocked);
    if (RotationLocked)
      rotationLocked = true;
    else
      rotationLocked = false;
  }
  
  //TODO: MASS CHANGING EVENT
  // Connect w/ eventsystem
  void RigidBody::Update()
  {
    //RigidBody::getMass();
    //float Mass = mass;
    
  }

  float RigidBody::calculateMass()
  {
    //density * volume
    if (gameObject->CircleCollider)
    {
      mass = (4 / 3) * 3.14f * gameObject->CircleCollider->radius;
    }
    else// if (gameObject->LineCollider)
    {
      mass = 10;
    }
    return mass;
  }

  void RigidBody::Integrate(float dt_, GameObject * obj)
  {
    gameObject = obj;
    
		if (obj->RigidBody->state == RigidBody::Static)
			return;
    //update position
		prevPos = pos;
		pos = pos + vel * dt_;
    
    //determine acceleration
		glm::vec2 acceleration = Physics::applyNetForce(accumulatedForce, invMass) + Physics::Constant::g;

    //integrate velocity
		vel = vel + acceleration * dt_;

		//dampen -- might not need this

		//Clamp velMax
		if (glm::dot(vel, vel) > Physics::PHYSICSSYSTEM->maxVelSq)
		{
			glm::normalize(vel);
			vel = vel * Physics::PHYSICSSYSTEM->maxVel;
		}

		//clear force
		accumulatedForce = glm::vec2(0, 0);
  }

  DefineComponentName(RigidBody);
}

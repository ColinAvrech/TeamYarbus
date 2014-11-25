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
	ZilchDefineType(RigidBody, CinderZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
		ZilchBindMethod(ApplyForce);

	}
	//Destructor
	RigidBody::~RigidBody()
	{
		gameObject->RigidBody = nullptr;
	}

	void RigidBody::Initialize()
	{
		mass = 1.0f;
		invMass = 1 / mass;
		friction = 0.0f;
		restitution = 0.0f;
		accumulatedForce = vec3(0.0f, 0.0f, 0.0f);

		//get starting position
		prevPos = gameObject->Transform->GetPosition();

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
		vec3 Velocity;
		temp = data->FindElement(data, "Velocity");
		temp->GetValue(&Velocity);
		vel.x = Velocity.x;
		vel.y = Velocity.y;

		//Angular Velocity
		vec3 AngVel;
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
		if (this->gameObject->ShapeCollider)
		{
			vec3 scale = gameObject->Transform->GetScale();
			float density = gameObject->ShapeCollider->getDensity();
			mass = (scale.x * scale.y * scale.z) * density;
			return mass;
		}
		else
			return 0;
	}

	void RigidBody::Integrate(float dt_)
	{
		pos = gameObject->Transform->GetPosition();
		if (this->state == RigidBody::Static)
			return;

    vec3 acceleration = Physics::applyNetForce(accumulatedForce, invMass);
		//determine acceleration
    if (!onGround)
		  acceleration += vec3(0, -9.8f, 0);


		//integrate velocity
		vel = vel + acceleration * dt_;

		//dampen -- might not need this

		//Clamp velMax
		if (glm::dot(vel, vel) > Physics::PHYSICSSYSTEM->maxVelSq)
		{
			glm::normalize(vel);
			vel = vel * Physics::PHYSICSSYSTEM->maxVel;
		}
		//update position
		pos = pos + vel * dt_;

		prevPos = pos;

		gameObject->Transform->Translate(vel.x * dt_, vel.y * dt_, vel.z * dt_);
		//clear force
		accumulatedForce = vec3(0.0f, 0.0f, 0.0f);
	}

	void RigidBody::AddForce(vec3 force)
	{
		accumulatedForce += force;
	}

	void RigidBody::ApplyForce(Zilch::Real3 force)
	{
		accumulatedForce += vec3(float(force.x), float(force.y), float(force.z));
	}

	DefineComponentName(RigidBody);
}

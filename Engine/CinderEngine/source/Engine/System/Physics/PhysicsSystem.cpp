/******************************************************************************/
/*!
\file   PhysicsSystem.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "PhysicsSystem.h"
#include "ObjectSystem.h"

namespace Framework
{
	namespace Physics
	{
		extern PhysicsSystem *PHYSICSSYSTEM = NULL;
		bool PhysicsSystem::Initialize()
		{
			PHYSICSSYSTEM = this;
			maxVel = 20.0f;
			maxVelSq = maxVel * maxVel;
			penetrationEpsilon = .2f;
			penetrationResolvePercent = .8f;
			accumulator = .0f;
			Contacts.Reset();
			//gravityDirection = { 0, -1 };
			std::cout << "PhysicsSystem Initialized." << std::endl;
			return true;
		}

		void PhysicsSystem::addContact(BodyContact* contact)
		{
			Contacts.addContact(contact);
		}

		void PhysicsSystem::IntegrateBodies(const float dt)
		{
			//MAKE ITERATE THRU RIGIDBODS
			for (auto it = Bodies.begin(); it != Bodies.end(); ++it)
			{
				(*it)->RigidBody->Integrate(dt);
			}
		}

		void PhysicsSystem::DetectContacts(float dt)
		{
			for (auto it = CircleColliders.begin(); it != CircleColliders.end(); ++it)
			{
				//circle to circle
				for (auto jt = it + 1; jt != CircleColliders.end(); ++jt)
				{
					static_cast<CircleCollider*>((*it)->ShapeCollider)->
					DetectCollision(static_cast<CircleCollider*>((*jt)->ShapeCollider));
				}

				//circle to line
				for (auto jt = LineColliders.begin(); jt != LineColliders.end(); ++jt)
				{
					static_cast<CircleCollider*>((*it)->ShapeCollider)->
						DetectCollision(static_cast<LineCollider*>((*jt)->ShapeCollider));
				}

				for (auto jt = SplineColliders.begin(); jt != SplineColliders.end(); ++jt)
				{
					static_cast<CircleCollider*>((*it)->ShapeCollider)->
						DetectCollision(*jt);
				}
			}
		}

		void PhysicsSystem::Step(const float dt)
		{
			IntegrateBodies(dt);

			Contacts.Reset();

			DetectContacts(dt);

			Contacts.ResolveContacts(dt);


		}

		// DETERMINISTIC - Physics will run the same on every system
		void PhysicsSystem::Update(const float &dt)
		{
			const float TimeStep = 1.0f / 60.0f;

			if (!StepModeActive)
			{
				accumulator += dt;
				accumulator = std::min(accumulator, TimeStep * 5);
				if (accumulator > TimeStep)
				{
					accumulator -= TimeStep;
					Step(TimeStep);
				}
			}
			else
			{
				accumulator = 0.0f;
				if (AdvanceStep)
				{
					Step(TimeStep);
				//	AdvanceStep = false;
				}
			}

			/*
			if (DebugDrawingActive)
				DebugDraw();
				*/
		}
	}
}
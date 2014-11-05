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
			//gravityDirection = { 0, -1 };
			std::cout << "PhysicsSystem Initialized." << std::endl;
			return true;
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
			//BodyIterator bodyA = Bodies.begin();
			//BodyIterator lastBody = Bodies.last(); //end - 1

			//Broad phase should be added this is N^2
			for (auto bodyA = Bodies.begin(); bodyA != Bodies.end(); ++bodyA)
			{
				auto bodyB = bodyA;
				++bodyB;
				for (; bodyB != Bodies.end(); ++bodyB)
				{
					//Do not collide static bodies with other static bodies
					if ((*bodyA)->RigidBody->state != RigidBody::DynamicState::Static
						|| (*bodyB)->RigidBody->state != RigidBody::DynamicState::Static)
					{
						//Collsion.GenerateContacts((bodyA)->BodyShape, (bodyA)->Position, (bodyB)->BodyShape, (bodyB)->Position, &Contacts);
					}
				}
			}
		}

		void PhysicsSystem::Step(const float dt)
		{
			IntegrateBodies(dt);

			Contacts.Reset();


		}

		// DETERMINISTIC - Physics will run the same on every system
		void PhysicsSystem::Update(const double dt)
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
					AdvanceStep = false;
				}
			}

			/*
			if (DebugDrawingActive)
				DebugDraw();
				*/
		}
	}
}
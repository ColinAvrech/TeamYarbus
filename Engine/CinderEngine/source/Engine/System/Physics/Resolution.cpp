#include "ShapeCollider.h"
#include "PhysicsLibrary.h"
#include "Resolution.h"
#include "RigidBody.h"

namespace Framework
{
	namespace Physics
	{
		void Resolve(CollisionEvent* pre)
		{
			CollisionEvent* post = pre;
			RigidBody::DynamicState stateA = pre->thisObject->RigidBody->state;
			RigidBody::DynamicState stateB = pre->OtherObject->RigidBody->state;

			if (stateA == RigidBody::Static || stateB == RigidBody::Static)
			{
				ResolveStatic(pre, post, stateA, stateB);
			}
			else
			{
				ResolveDynamic(pre, post);
			}
		}

		void ResolveStatic(CollisionEvent* pre, CollisionEvent* post,
						   RigidBody::DynamicState stateA, RigidBody::DynamicState stateB)
		{
			*post = *pre;
			glm::vec2 velA = post->thisObject->RigidBody->vel;
			glm::vec2 velB = post->OtherObject->RigidBody->vel;

			if (stateA == RigidBody::Static)
			{
				velB = getReflection(pre->normal, velB);
			}
			else
			{
				velA = getReflection(pre->normal, velA);
			}
		}

		void ResolveDynamic(CollisionEvent* pre, CollisionEvent* post)
		{
			*post = *pre;
			float massA = pre->thisObject->RigidBody->getMass();
			float massB = pre->OtherObject->RigidBody->getMass();

			glm::vec2 velA = pre->thisObject->RigidBody->vel;
			glm::vec2 velB = pre->OtherObject->RigidBody->vel;

			float bunch11 = (massA - massB) / (massA + massB);
			float bunch12 = (2 * massB) / (massA + massB);
			post->thisObject->RigidBody->vel.x = bunch11 * velA.x + bunch12 * velB.x;
			post->thisObject->RigidBody->vel.y = bunch11 * velA.y + bunch12 * velB.y;

			float bunch21 = (massB - massA) / (massA + massB);
			float bunch22 = (2 * massA) / (massA + massB);
			post->OtherObject->RigidBody->vel.x = bunch21 * velA.x + bunch22 * velB.x;
			post->OtherObject->RigidBody->vel.y = bunch21 * velA.y + bunch22 * velB.y;
		}
	} //Physics
} //Framework


#pragma once
#include "RigidBody.h"
#include "CollisionEvent.h"
namespace Framework
{
	namespace Physics
	{
		void Resolve(CollisionEvent* pre);
		void ResolveStatic(CollisionEvent* pre, CollisionEvent* post, 
						   RigidBody::DynamicState stateA, RigidBody::DynamicState stateB);
		void ResolveDynamic(CollisionEvent* pre, CollisionEvent* post);


	}
}
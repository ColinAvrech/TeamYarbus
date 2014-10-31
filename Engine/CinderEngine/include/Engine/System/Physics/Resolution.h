

#pragma once
namespace Framework
{
  //Forward Declarations
  class RigidBody;
  class CollisionEvent;

	namespace Physics
	{
		void Resolve(CollisionEvent* pre);
		void ResolveStatic(CollisionEvent* pre, CollisionEvent* post, 
						   RigidBody::DynamicState stateA, RigidBody::DynamicState stateB);
		void ResolveDynamic(CollisionEvent* pre, CollisionEvent* post);


	}
}
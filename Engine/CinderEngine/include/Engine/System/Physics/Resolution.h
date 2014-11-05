

#pragma once
#include "RigidBody.h"
#include "CollisionEvent.h"
#include "CollisionSuccess.h"
namespace Framework
{
	//set of contacts to be resolved
	class ContactSet
	{
	private:
		static const int maxContacts = 256;
		unsigned numOfContacts;

	public:
		BodyContact * getNextContact();
		BodyContact contactArray[maxContacts];
		void Resolve(CollisionEvent* pre);
		void ResolveStatic(CollisionEvent* pre, CollisionEvent* post,
			RigidBody::DynamicState stateA, RigidBody::DynamicState stateB);
		void ResolveDynamic(CollisionEvent* pre, CollisionEvent* post);
		void Reset();
	};
}
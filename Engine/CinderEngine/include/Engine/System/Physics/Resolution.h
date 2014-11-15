/******************************************************************************/
/*!
\file   Resolution.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
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

		void ResolvePositions(const double dt);
		void ResolveVelocities(const double dt);

	public:
		ContactSet(){};
		//BodyContact * getNextContact();
		BodyContact contactArray[maxContacts];
		void addContact(BodyContact * contact);
		void ResolveContacts(const double dt);/*
		void ResolveStatic(CollisionEvent* pre, CollisionEvent* post,
			RigidBody::DynamicState stateA, RigidBody::DynamicState stateB);
		void ResolveDynamic(CollisionEvent* pre, CollisionEvent* post);*/
		void Reset();
	};
}
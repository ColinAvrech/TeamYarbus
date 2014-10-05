/******************************************************************************/
/*!
\file   RigidBody.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include "Vec2.h"
#include "Component.h"

namespace Framework
{
	class RigidBody : public Component
	{
	public:
		RigidBody(){};
		~RigidBody(){};

		Vec2 getVel(void)
		{
			return vel;
		};

		enum DynamicState
		{
			Static,    //unmoving
			Kinematic, //moving position, velocity acceleration
			Dynamic    //moving with kinematic & all forces considered
		};

		// The non-base component uses DefineComponentName macro to name component
		const static std::string Name;

	private:
		Vec2 vel;
		Vec2 angVel;
		float mass;
		bool allowSleep;
		bool rotationLocked;
		DynamicState state;
	};
} //Framework
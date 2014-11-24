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
#include "Common.h"
#include "glm.hpp"

namespace Framework
{
	class RigidBody : public Component
	{
	public:
		ZilchDeclareBaseType(RigidBody, Zilch::TypeCopyMode::ReferenceType);
		// The non-base component uses DefineComponentName macro to name component
		enum DynamicState
		{
			Static,    //unmoving
			Kinematic, //moving position, velocity acceleration
			Dynamic    //moving with kinematic & all forces considered
		};

		const static string Name;
		vec3 vel;
		vec3 angVel;

		vec3 accumulatedForce;
		bool allowSleep;
    bool onGround;
		bool rotationLocked;
		DynamicState state;

		float getMass()
		{
			return mass;
		}

		float getInvMass()
		{
			return invMass;
		}

		RigidBody(GameObject * obj)
		{
			gameObject = obj;
		}

    RigidBody(){}

		virtual ~RigidBody();

		/*!Telegraph that the component is active*/
		void Initialize() override;
		void Serialize(Serializer::DataNode* data) override;
		void Update();
		void AddForce(vec3 force);
		void ApplyForce(Zilch::Real3 force);
		void Integrate(const float dt);
		void SetPosition(vec3);
		void SetVelocity(vec3);

		void setMass(float Mass) { mass = Mass; }
		float calculateMass(void);

	private:
		vec3 pos, prevPos;
		float mass;
		float invMass;
		float restitution;
		float friction;
		//wat
		float damping;
	};
} //Framework
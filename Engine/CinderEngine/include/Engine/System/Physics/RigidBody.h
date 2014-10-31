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
		// The non-base component uses DefineComponentName macro to name component
		float mass;
		enum DynamicState
		{
			Static,    //unmoving
			Kinematic, //moving position, velocity acceleration
			Dynamic    //moving with kinematic & all forces considered
		};

		const static std::string Name;
		bool allowSleep;
		bool rotationLocked;
		DynamicState state;
		glm::vec2 vel;
		glm::vec2 angVel;


		RigidBody() : mass(.5)
    {
    };

		virtual ~RigidBody();

		/*!Telegraph that the component is active*/
		void Initialize() override;
		void Serialize(Serializer::DataNode* data) override;
		void Update();

		float getMass(void)
		{
			return mass;
		}

		float getInvMass(void)
		{
			return invMass;
		}

		void setMass(float Mass) { mass = Mass; }
		float calculateMass(void);
		void Integrate(float _dt, GameObject * obj);

	private:
		
		float invMass;

	};
} //Framework
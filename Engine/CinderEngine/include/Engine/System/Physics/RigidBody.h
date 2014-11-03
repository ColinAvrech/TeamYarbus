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
		enum DynamicState
		{
			Static,    //unmoving
			Kinematic, //moving position, velocity acceleration
			Dynamic    //moving with kinematic & all forces considered
		};

		const static std::string Name;
		glm::vec2 pos;
		glm::vec2 prevPos;
		glm::vec2 vel;
		glm::vec2 angVel;
		glm::vec2 accumulatedForce;
		bool allowSleep;
		bool rotationLocked;
		float mass;
		float invMass;
		DynamicState state;
		float restitution;
		float friction;
		//wat
		float damping;

		RigidBody* next;
		RigidBody* prev;

		RigidBody(GameObject * obj)
		{
			gameObject = obj;
		};

		virtual ~RigidBody();

		/*!Telegraph that the component is active*/
		void Initialize() override;
		void Serialize(Serializer::DataNode* data) override;
		void Update();
		void AddForce(glm::vec2 force);
		void Integrate(const float dt, GameObject * obj);
		void SetPosition(glm::vec2);
		void SetVelocity(glm::vec2);

		void setMass(float Mass) { mass = Mass; }
		float calculateMass(void);

	private:
	};
} //Framework
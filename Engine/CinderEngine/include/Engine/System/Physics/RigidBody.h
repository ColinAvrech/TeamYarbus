
#pragma once

#include "Vec2.h"
#include "Component.h"

namespace Framework
{
	namespace Physics
	{
		class RigidBody : public Component
		{
		public:
			Vec2 getVel(void){
				return vel;
			};
			enum DynamicState
			{
				Kinematic,
				Static,
				Dynamic
			};

		private:
			Vec2 vel;
			Vec2 angVel;
			float mass;
			bool allowSleep;
			bool rotationLocked;
			DynamicState state;
		};
	} //Physics
} //Framework
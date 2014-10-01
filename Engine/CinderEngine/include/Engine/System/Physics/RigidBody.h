
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
			RigidBody() : vel(1, 0){}
		
			Vec2 getVel(void){
				return vel;
			};
			enum DynamicState
			{
				Kinematic,
				Static,
				Dynamic
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
	} //Physics
} //Framework
/******************************************************************************/
/*!
\file   PhysicsSystem.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "PhysicsSystem.h"

namespace Framework
{
	namespace Physics
	{
		bool PhysicsSystem::Initialize()
		{
			std::cout << "PhysicsSystem Initialized." << std::endl;
			return true;
		}

		// Called every frame
		void PhysicsSystem::Update(const double dt)
		{
			//const float currentTime = GetCurrentTime();
			//accumulator += currentTime - frameStart;
			//frameStart = currentTime;
		
		}
		//{
		//	UpdateCollision();
		//	UpdateResolution();
		//	UpdateForces();
		//}
	}
}
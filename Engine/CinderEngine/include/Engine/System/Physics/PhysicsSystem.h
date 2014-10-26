/******************************************************************************/
/*!
\file   PhysicsSystem.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once

#include "BaseSystem.h"
#include "Common.h"
#include "glm.hpp"

namespace Framework
{
	namespace Physics
	{
		/*---------------------------------------------------------------------------
		// Class
		---------------------------------------------------------------------------*/
		class PhysicsSystem : public BaseSystem
		{
		public:
			glm::vec2 gravityDirection;
			// Returns name of System
			const std::string GetName(){ return "PhysicsSystem"; }

			/*-----------------------------------------------------------------------
			// Constructor / Destructor
			-----------------------------------------------------------------------*/
			PhysicsSystem(){};
			~PhysicsSystem(){};

			/*-----------------------------------------------------------------------
			// Public Variables
			-----------------------------------------------------------------------*/

			/*---------------------------------------------------------------------*/
			// Public Structs
			/*---------------------------------------------------------------------*/

			/*-----------------------------------------------------------------------
			// Public Functions
			-----------------------------------------------------------------------*/
			//!Initialize the system
			//this is where I will be setting up physics components
			bool Initialize(void);

			// Called every frame
			void Update(const double dt);
			//void UpdateCollision(void);
			//void UpdateResolution(void);
			//void UpdateForces(void);


		private:
			float accumlator;
			float frameStart;
			float CurrentTime;
			

		};
		extern PhysicsSystem *PHYSICSSYSTEM;
	}
}
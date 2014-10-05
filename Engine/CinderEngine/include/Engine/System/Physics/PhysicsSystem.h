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

			// Returns name of System
			const std::string GetName(){ return "PhysicsSystem"; }
		private:


		};
		extern PhysicsSystem *PHYSICSSYSTEM;
	}
}
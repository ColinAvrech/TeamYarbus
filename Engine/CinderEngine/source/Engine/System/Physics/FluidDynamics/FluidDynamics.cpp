/******************************************************************************/
/*!
\file   Thermodynamics.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "FluidDynamics.h"


#define GRID_Y_SIZE 64

namespace Framework
{
  namespace Physics
  {
    //!Null untill the ObjectSystem has been created
    FluidDynamicsSystem * FLUIDDYNAMICS = NULL;
    
    //Constructor
    FluidDynamicsSystem::FluidDynamicsSystem()
    {
      FLUIDDYNAMICS = this;
    }

    //Destructor
    FluidDynamicsSystem::~FluidDynamicsSystem()
    {
      fluid_bodies.clear();
      //ReleaseThreads();
    }

    /*-----------------------------------------------------------------------
    // Public Functions
    -----------------------------------------------------------------------*/

    bool FluidDynamicsSystem::Initialize()
    {
      std::cout << "FluidDynamics Initialized." << std::endl;
      return true;
    }

    // Called every frame
    void FluidDynamicsSystem::Update(const float& dt)
    {
      
    }

    void FluidDynamicsSystem::Draw()
    {
      
    } //draw function

  }//namespace Physics
}//namespace Framework

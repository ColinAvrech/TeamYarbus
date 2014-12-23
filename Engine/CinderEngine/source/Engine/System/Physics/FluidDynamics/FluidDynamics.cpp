/******************************************************************************/
/*!
\file   Thermodynamics.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "FluidDynamics.h"
#include "FluidBody.h"


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
      Clear();
    }

    void FluidDynamicsSystem::Clear()
    {
      fluid_bodies.clear();
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
      for (auto i : fluid_bodies)
      {
        i->Update(dt);
      }
    }

    bool FluidDynamicsSystem::UpdatesOnPaused()
    {
      return false;
    }

    void FluidDynamicsSystem::AddFluid(FluidBody* fluid)
    {
      this->fluid_bodies.push_back(fluid);
    }

    void FluidDynamicsSystem::Draw()
    {
      
    } //draw function

  }//namespace Physics
}//namespace Framework

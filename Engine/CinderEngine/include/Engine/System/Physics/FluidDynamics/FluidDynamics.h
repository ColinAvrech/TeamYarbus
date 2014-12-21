/*****************************************************************************/
/*!
\file   Thermodynamics.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The thermodynamics system controls the flow of fluids around the level.
*/
/*****************************************************************************/

#pragma once


/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes


#include "BaseSystem.h"
#include <unordered_map>
#include "FluidSolver.h"
#include "FireSystem.h"
#include "Terrain2D.h"
#include "FireStarter.h"
#include "GUIText.h"
#include "GridManager.h"

#pragma endregion


namespace Framework
{
  namespace Physics
  {
    //Forward declarations
    class FluidBody;

    /*---------------------------------------------------------------------------
    // Class
    ---------------------------------------------------------------------------*/
    class FluidDynamicsSystem : public BaseSystem
    {
    public:

      /*-----------------------------------------------------------------------
      // Constructor
      -----------------------------------------------------------------------*/
      FluidDynamicsSystem();

      //!Initialize the system
      bool Initialize();

      // Called every frame
      void Update(const float& dt);

      //Draw water bodies
      void Draw();

      /*-----------------------------------------------------------------------
      // Destructor
      -----------------------------------------------------------------------*/
      ~FluidDynamicsSystem();

    private:
      std::vector<FluidBody*> fluid_bodies;

    }; //class FluidDynamics

    extern FluidDynamicsSystem *FLUIDDYNAMICS;
  } //namespace physics
} //namespace framework

//-----------------------------------------------------------------------------

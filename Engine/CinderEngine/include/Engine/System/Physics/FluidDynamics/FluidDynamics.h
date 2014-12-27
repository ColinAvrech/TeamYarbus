/*****************************************************************************/
/*!
\file   FluidDynamics.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Fluid physics manager.
*/
/*****************************************************************************/

#pragma once


/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#pragma endregion


namespace Framework
{
  //Forward declarations
  class FluidBody;


  namespace Physics
  {
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
      virtual bool Initialize();

      // Called every frame
      virtual void Update(const float& dt);

      virtual bool UpdatesOnPaused();

      virtual const string GetName() { return "FluidDynamicsSystem"; }

      void AddFluid(FluidBody* fluid);

      //Draw water bodies
      void Draw();

      void Clear();

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

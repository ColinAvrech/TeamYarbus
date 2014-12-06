/******************************************************************************/
/*!
\file   Fluid_Physics_Engine.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_ENGINE_H
#define	_FLUID_PHYSICS_ENGINE_H


#include "Cinder_Engine_Data_Types.h"
#include "Math_Vector2D.h"
#include "Fluid_Physics_Object.h"
#include "Temporary_Collision.h"
#include "Fluid_Physics_Particle.h"
#include "Fluid_Physics_Particle_Array.h"
#include "Fluid_Particle_Position_Loader.h"
#include "Fluid_SPH_Elastic.h"
#include "Fluid_Physics_Integration.h"
#include "Fluid_Physics_Integration_Predict.h"
#include "Test_Mouse.h"
#include "Fluid_Physics_Spring.h"
#include "Fluid_Physics_Spring_Array.h"

namespace Framework
{

#define FLUID_PHYSICS_ENGINE_VISCOSITY_SIGMA  0.001f           
#define FLUID_PHYSICS_ENGINE_VISCOSITY_BETA   0.001f 
#define FLUID_PHYSICS_ENGINE_DT_CAP	          0.01f


  class Fluid_Physics_Engine
  {
  public:

    Fluid_Physics_Particle_Array ParticleTable;

    Fluid_Physics_Engine () :
      FluidPositionLoader (Fluid_Physics_Particle_Position_Loader ()),
      PhysicsCollision2DEngine (Temporary_Collision ()),
      PhysicsFluidEngine (SPH_Fluid_Elastic ()),
      PhysicsIntegrationEngine (0),
      SpringTable (Fluid_Physics_Spring_Array ()),
      FluidMaxWidth (0.0f),
      FluidMaxHeight (0.0f),
      DeltaValue (0.0f)
    {}


    Fluid_Physics_Engine (const Fluid_Physics_Engine& source) :
      FluidPositionLoader (source.FluidPositionLoader),
      PhysicsCollision2DEngine (source.PhysicsCollision2DEngine),
      PhysicsFluidEngine (source.PhysicsFluidEngine),
      PhysicsIntegrationEngine (source.PhysicsIntegrationEngine),
      SpringTable (source.SpringTable),
      FluidMaxWidth (source.FluidMaxWidth),
      FluidMaxHeight (source.FluidMaxHeight),
      DeltaValue (source.DeltaValue)
    {}


    virtual ~Fluid_Physics_Engine ()
    {
      delete PhysicsIntegrationEngine;
      PhysicsIntegrationEngine = nullptr;
    }


    Fluid_Physics_Engine& operator= (const Fluid_Physics_Engine& source)
    {
      FluidPositionLoader = source.FluidPositionLoader;
      PhysicsCollision2DEngine = source.PhysicsCollision2DEngine;
      PhysicsFluidEngine = source.PhysicsFluidEngine;
      PhysicsIntegrationEngine = source.PhysicsIntegrationEngine;
      SpringTable = source.SpringTable;
      FluidMaxWidth = source.FluidMaxWidth;
      FluidMaxHeight = source.FluidMaxHeight;
      DeltaValue = source.DeltaValue;
    }

    void SetMouse (const BOOL attract, const BOOL repel)
    {
      PhysicsMouseInteraction.SetMouse (attract, repel);
    }

    void SetAttractor (const MATH_VECTOR_2D &attractor)
    {
      PhysicsMouseInteraction.SetAttractor (attractor);
    }

    void Initialize (const REAL32 fluid_max_width, const REAL32 fluid_max_height);

    void Update (
      const REAL32 delta_time,
      const BOOL do_viscosity,
      const BOOL is_viscoelastic,
      const BOOL is_plastic
      );

    void ResetViscoelasticity ();
    void InitializePlasticity ();

  private:
    Fluid_Physics_Particle_Position_Loader FluidPositionLoader;
    Temporary_Collision PhysicsCollision2DEngine;
    SPH_Fluid_Elastic PhysicsFluidEngine;
    Fluid_Physics_Integration* PhysicsIntegrationEngine;
    Fluid_Physics_Spring_Array SpringTable;
    Test_Mouse PhysicsMouseInteraction;
    REAL32 FluidMaxWidth;
    REAL32 FluidMaxHeight;
    REAL32 DeltaValue;

    void InitializeParticles ();
    void CalculateDensity (const REAL32 delta_time);
    void CalculatePressureForce (const REAL32 delta_time);

    void UpdateParticlesVelocityAndPosition (const REAL32 delta_time);

    void DetectCollision (const REAL32 width, REAL32 height);

    void CalculateViscosity (const REAL32 delta_time);
    void CalculateSpringsForViscoelasticity (const REAL32 delta_time);
    void CalculatePlasticity (const REAL32 delta_time);

  };
}

#endif
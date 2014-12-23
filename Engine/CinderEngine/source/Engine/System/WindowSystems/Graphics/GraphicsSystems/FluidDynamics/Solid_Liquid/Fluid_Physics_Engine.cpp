/******************************************************************************/
/*!
\file   Fluid_Physics_Engine.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Cinder_Engine_Data_Types.h"
#include "Fluid_Physics_Engine.h"
#include "Fluid_Physics_Constants.h"
#include "Fluid_Physics_Level_Constants.h"
#include <iostream>

namespace Framework
{
#define LOCAL_Margin_with_bottom_of_the_window 1.0f

  void Fluid_Physics_Engine::Initialize (
    const REAL32 fluid_max_width,
    const REAL32 fluid_max_height
    )
  {
    PhysicsIntegrationEngine = new Fluid_Physics_Predict ();
    FluidMaxHeight = fluid_max_height;
    FluidMaxWidth = fluid_max_width;
    InitializeParticles ();
  }


  void Fluid_Physics_Engine::InitializeParticles ()
  {
    MATH_VECTOR_2D
      position;
    Fluid_Physics_Particle
      particle;
    INDEX
      particle_index;

    FluidPositionLoader.LoadPositions (
      "pentagon.bmp"
      );

    particle_index = 0;

    CINDER_LOOP_THROUGH_TABLE (particle_index, FluidPositionLoader.GetPointTable ())
    {
      particle.SetPosition (FluidPositionLoader.GetPointTable () [particle_index]);
      particle.SetOldPosition (particle.GetPosition ());
      ParticleTable.push_back (particle);
    }

    particle_index = 0;

    CINDER_LOOP_THROUGH_TABLE (particle_index, ParticleTable)
    {
      ParticleTable [particle_index].SetViscosityBeta (FLUID_PHYSICS_ENGINE_VISCOSITY_BETA);
      ParticleTable [particle_index].SetViscositySigma (FLUID_PHYSICS_ENGINE_VISCOSITY_SIGMA);
    }
    PhysicsFluidEngine.InitialiseSpringTable (ParticleTable.size ());
  }


  void Fluid_Physics_Engine::Update (
    const REAL32 delta_time,
    const BOOL do_viscosity,
    const BOOL is_viscoelastic,
    const BOOL is_plastic
    )
  {
    COUNTER
      counter,
      counter2;

    DeltaValue += delta_time;
    counter = 0;

    if (delta_time / FLUID_PHYSICS_ENGINE_DT_CAP > 5.0f)
    {
      counter = INT32 (delta_time / FLUID_PHYSICS_ENGINE_DT_CAP - 5.0f);
    }

    if (DeltaValue > FLUID_PHYSICS_ENGINE_DT_CAP)
    {
      CINDER_LOOP_THROUGH_INDEX (counter2, 0, counter)
      {
        if (is_viscoelastic)
        {
          CalculateSpringsForViscoelasticity (FLUID_PHYSICS_ENGINE_DT_CAP);
        }
        if (is_plastic)
        {
          CalculatePlasticity (FLUID_PHYSICS_ENGINE_DT_CAP);
        }
        CalculateDensity (FLUID_PHYSICS_ENGINE_DT_CAP);
        CalculatePressureForce (FLUID_PHYSICS_ENGINE_DT_CAP);
        UpdateParticlesVelocityAndPosition (FLUID_PHYSICS_ENGINE_DT_CAP);
      }
      DeltaValue = 0.0f;
    }
  }


  void Fluid_Physics_Engine::ResetViscoelasticity ()
  {
    PhysicsFluidEngine.ResetSprings ();
  }

  void Fluid_Physics_Engine::InitializePlasticity ()
  {
    PhysicsFluidEngine.InitializePlasticity (
      ParticleTable,
      FLUID_PHYSICS_SMOOTHING_RAD,
      0.09f
      );
  }


  void Fluid_Physics_Engine::CalculateDensity (const REAL32 delta_time)
  {

    PhysicsFluidEngine.CalculateDensity (
      ParticleTable,
      FLUID_PHYSICS_SMOOTHING_RAD
      );
  }


  void Fluid_Physics_Engine::CalculatePressureForce (const REAL32 delta_time)
  {
    PhysicsFluidEngine.CalculatePressure (
      ParticleTable,
      FLUID_PHYSICS_STIFFNESS_K,
      FLUID_PHYSICS_NEAR_STIFFNESS_K,
      FLUID_PHYSICS_REST_DENSITY,
      delta_time
      );
  }


  void Fluid_Physics_Engine::UpdateParticlesVelocityAndPosition (const REAL32 delta_time)
  {
    INDEX particle_index;
    REAL32 attraction_radius;

    particle_index = 0;

    CINDER_LOOP_THROUGH_TABLE (particle_index, ParticleTable)
    {
      PhysicsIntegrationEngine->UpdateVelocity (
        ParticleTable [particle_index],
        delta_time * SPH_ELASTIC_DT_SCALING
        );
    }

    CalculateViscosity (delta_time);

    particle_index = 0;

    CINDER_LOOP_THROUGH_TABLE (particle_index, ParticleTable)
    {
      PhysicsIntegrationEngine->UpdatePosition (
        ParticleTable [particle_index],
        delta_time * SPH_ELASTIC_DT_SCALING
        );

      ParticleTable [particle_index].SetForce (FLUID_PHYSICS_GRAVITY);
    }
    DetectCollision (FluidMaxWidth, FluidMaxHeight);

    particle_index = 0;
    attraction_radius = FluidMaxWidth / 4.0f;

    CINDER_LOOP_THROUGH_TABLE (particle_index, ParticleTable)
    {
      PhysicsMouseInteraction.ApplyMouseBehaviour (ParticleTable [particle_index], attraction_radius);
    }
  }


  void Fluid_Physics_Engine::DetectCollision (const REAL32 widht, const REAL32 height)
  {
    INDEX
      particle_index;
    MATH_POINT_2D
      position;

    particle_index = 0;
    position.Assign (0.0f, 0.0f);

    CINDER_LOOP_THROUGH_TABLE (particle_index, ParticleTable)
    {
      PhysicsCollision2DEngine.CheckForWall (
        ParticleTable [particle_index],
        FLUID_PHYSICS_SPRING_COEFF,
        position,
        -widht,
        +widht,
        -height + LOCAL_Margin_with_bottom_of_the_window,
        +height
        );
    }
  }


  void Fluid_Physics_Engine::CalculateViscosity (const REAL32 delta_time)
  {
    PhysicsFluidEngine.CalculateViscosity (
      ParticleTable,
      FLUID_PHYSICS_SMOOTHING_RAD,
      delta_time
      );
  }


  void Fluid_Physics_Engine::CalculateSpringsForViscoelasticity (const REAL32 delta_time)
  {
    PhysicsFluidEngine.CalculateViscoElasticity (
      ParticleTable,
      FLUID_PHYSICS_SMOOTHING_RAD,
      FLUID_PHYSICS_L_SPRING_COEFF,
      FLUID_PHYSICS_YIELD_RATIO,
      FLUID_PHYSICS_PLASTICITY,
      delta_time
      );
  }


  void Fluid_Physics_Engine::CalculatePlasticity (const REAL32 delta_time)
  {
    PhysicsFluidEngine.CalculatePlasticity (
      ParticleTable,
      delta_time
      );
  }
}

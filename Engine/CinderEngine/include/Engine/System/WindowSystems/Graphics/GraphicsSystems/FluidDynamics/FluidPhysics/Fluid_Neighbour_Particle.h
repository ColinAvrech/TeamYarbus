/******************************************************************************/
/*!
\file   Fluid_Neighbour_Particle.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_N_PARTICLE
#define _FLUID_PHYSICS_N_PARTICLE

#include "Math_Vector2D.h"
#include "Cinder_Engine_Data_Types.h"
#include "Debug_Defines.h"

namespace Framework
{
  class Fluid_Neighbour_Particle
  {
  public:

    Fluid_Neighbour_Particle () :
      ParticleIndex (0),
      SmoothingKernel (0.0f),
      PoweredTwoSmoothingKernel (0.0f),
      Distance (0.0f)
    {}

    Fluid_Neighbour_Particle (const Fluid_Neighbour_Particle& source) :
      ParticleIndex (source.ParticleIndex),
      SmoothingKernel (source.SmoothingKernel),
      PoweredTwoSmoothingKernel (source.PoweredTwoSmoothingKernel),
      Distance (source.Distance)
    {}

    // ~~

    virtual ~Fluid_Neighbour_Particle (){}

    Fluid_Neighbour_Particle& operator= (const Fluid_Neighbour_Particle& source)
    {
      DEBUG_check_this_expression (this != &source);

      ParticleIndex = source.ParticleIndex;
      SmoothingKernel = source.SmoothingKernel;
      PoweredTwoSmoothingKernel = source.PoweredTwoSmoothingKernel;
      Distance = source.Distance;

      return *this;
    }

    // Gettors
    INDEX GetParticleIndex () const
    {
      return ParticleIndex;
    }

    REAL32 GetDistance () const
    {
      return Distance;
    }

    REAL32 GetPoweredTwoSmoothingKernel () const
    {
      return PoweredTwoSmoothingKernel;
    }

    // Settors
    void SetParticleIndex (const INDEX particle_index)
    {
      ParticleIndex = particle_index;
    }

    REAL32 GetSmoothingKernel () const
    {
      return SmoothingKernel;
    }

    void SetSmoothingKernel (const REAL32 smooething_kernel_value)
    {
      SmoothingKernel = smooething_kernel_value;
    }

    void SetPoweredTwoSmoothingKernel (const REAL32 powered_two_smooething_kernel_value)
    {
      PoweredTwoSmoothingKernel = powered_two_smooething_kernel_value;
    }

    void SetDistance (const REAL32 distance_value)
    {
      Distance = distance_value;
    }

  private:
    REAL32
      SmoothingKernel,
      PoweredTwoSmoothingKernel,
      Distance;
    INDEX
      ParticleIndex;
  };
}

#endif
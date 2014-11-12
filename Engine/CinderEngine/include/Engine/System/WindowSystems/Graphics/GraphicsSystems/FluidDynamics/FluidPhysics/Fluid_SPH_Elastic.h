/******************************************************************************/
/*!
\file   Fluid_SPH_Elastic.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_SPH_ELASTIC
#define _FLUID_PHYSICS_SPH_ELASTIC


#include "Debug_Defines.h"
#include "Cinder_Engine_Data_Types.h"
#include "Cinder_Engine_Index_Array.h"
#include "Fluid_Physics_Particle.h"
#include "Fluid_Physics_Particle_Array.h"
#include "Fluid_Physics_Spring.h"
#include "Fluid_Physics_Spring_Array.h"

#define SPH_ELASTIC_DT_SCALING 100

namespace Framework
{
  class SPH_Fluid_Elastic
  {
  public:

    SPH_Fluid_Elastic () : SpringTable (){}
    SPH_Fluid_Elastic (const SPH_Fluid_Elastic& source) : SpringTable (source.SpringTable){}
    virtual ~SPH_Fluid_Elastic (){}

    SPH_Fluid_Elastic& operator= (const SPH_Fluid_Elastic& other)
    {
      DEBUG_check_this_expression (this != &other);
      SpringTable = other.SpringTable;

      return *this;
    }


    // Gettors
    const Fluid_Physics_Spring_Array GetSpringsTable () const
    {
      return SpringTable;
    }

    // Settors
    void SetSpringsTable (const Fluid_Physics_Spring_Array& spring_table)
    {
      SpringTable = spring_table;
    }

    void InitialiseSpringTable (const INT32 number_of_particles);
    void CalculateDensity (Fluid_Physics_Particle_Array & particlesTable, const REAL32 smoothing_radius);


    void CalculatePressure (
      Fluid_Physics_Particle_Array & particlesTable,
      const REAL32 stifness_parameter,
      const REAL32 near_stifness_parameter,
      const REAL32 rest_density,
      const REAL32 delta_time
      );

    void CalculateViscosity (
      Fluid_Physics_Particle_Array & particlesTable,
      const REAL32 smoothing_radius,
      const REAL32 delta_time
      );

    void CalculateViscoElasticity (
      Fluid_Physics_Particle_Array & particlesTable,
      const REAL32 smoothing_radius,
      const REAL32 coefficient_spring,
      const REAL32 yeild_ratio,
      const REAL32 plasticity_constant,
      const REAL32 delta_time
      );

    void InitializePlasticity (
      Fluid_Physics_Particle_Array & particlesTable,
      const REAL32 smoothing_radius,
      const REAL32 coefficient_spring
      );

    void CalculatePlasticity (
      Fluid_Physics_Particle_Array & particlesTable,
      const REAL32 delta_time
      );

    void ResetSprings ();

  private:
    // Spring Array
    Fluid_Physics_Spring_Array
      SpringTable;

  };
}

#endif
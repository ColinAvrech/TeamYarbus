/******************************************************************************/
/*!
\file   Fluid_Physics_Particle.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_PARTICLE
#define _FLUID_PHYSICS_PARTICLE


#include "Debug_Defines.h"
#include "Cinder_Engine_Data_Types.h"
#include "Math_Vector2D.h"
#include "Fluid_Physics_Object.h"
#include "Fluid_Neighbour_Particle.h"
#include "Fluid_Neighbour_Particle_Array.h"

namespace Framework
{
  class Fluid_Physics_Particle : public Fluid_Physics_Object
  {
  public:

    Fluid_Physics_Particle () :
      Fluid_Physics_Object (),
      Density (0.0f),
      NearDensity (0.0f),
      Mass (0.0f),
      ViscositySigma (0.0f),
      ViscosityBeta (0.0f),
      NeighboursTable ()
    {}

    Fluid_Physics_Particle (const Fluid_Physics_Particle& source) :
      Fluid_Physics_Object (source),
      Density (source.Density),
      NearDensity (source.NearDensity),
      Mass (source.Mass),
      ViscositySigma (source.ViscositySigma),
      ViscosityBeta (source.ViscosityBeta),
      NeighboursTable (source.NeighboursTable)
    {}

    virtual ~Fluid_Physics_Particle (){}

    Fluid_Physics_Particle & operator=(const Fluid_Physics_Particle & source)
    {
      DEBUG_check_this_expression (this != &source);

      Fluid_Physics_Object::operator= (source);
      Density = source.Density;
      NearDensity = source.NearDensity;
      Mass = source.Mass;
      ViscositySigma = source.ViscositySigma;
      ViscosityBeta = source.ViscosityBeta;
      NeighboursTable = source.NeighboursTable;

      return *this;
    }

    // Gettors
    REAL32 GetDensity () const
    {
      return Density;
    }

    REAL32 GetNearDensity () const
    {
      return NearDensity;
    }

    REAL32 GetViscositySigma () const
    {
      return ViscositySigma;
    }

    REAL32 GetViscosityBeta () const
    {
      return ViscosityBeta;
    }

    REAL32 GetMass () const
    {
      return Mass;
    }

    Fluid_Neighbour_Particle_Array& GetNeighbours ()
    {
      return NeighboursTable;
    }

    // Settors
    void SetDensity (const REAL32 density_value)
    {
      Density = density_value;
    }

    void SetNearDensity (const REAL32 near_density_value)
    {
      NearDensity = near_density_value;
    }

    void SetViscositySigma (const REAL32 viscosity_sigma_value)
    {
      ViscositySigma = viscosity_sigma_value;
    }

    void SetViscosityBeta (const REAL32 viscosity_beta_value)
    {
      ViscosityBeta = viscosity_beta_value;
    }

    void SetMass (const REAL32 mass_value)
    {
      Mass = mass_value;
    }

    Fluid_Neighbour_Particle_Array& SetNeighbours (const Fluid_Neighbour_Particle_Array& neighbours_table)
    {
      NeighboursTable = neighbours_table;
    }


  private:
    REAL32
      Density,
      NearDensity,
      ViscositySigma,
      ViscosityBeta,
      Mass;
    Fluid_Neighbour_Particle_Array
      NeighboursTable;
  };
}

#endif
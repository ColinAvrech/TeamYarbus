/******************************************************************************/
/*!
\file   Fluid_Physics_Integration.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_INTEGRATION
#define _FLUID_PHYSICS_INTEGRATION

#include "Debug_Defines.h"
#include "Cinder_Engine_Data_Types.h"
#include "Fluid_Physics_Object.h"

namespace Framework
{
  class Fluid_Physics_Integration
  {
  public:

    Fluid_Physics_Integration ()
    {}

    Fluid_Physics_Integration (const Fluid_Physics_Integration& source)
    {}

    virtual ~Fluid_Physics_Integration ()
    {}

    Fluid_Physics_Integration& operator= (const Fluid_Physics_Integration& source)
    {
      return *this;
    }

    virtual void UpdateVelocity (Fluid_Physics_Object& game_entity, const REAL32 delta_time) = 0;

    virtual void UpdatePosition (Fluid_Physics_Object& game_entity, const REAL32 delta_time) = 0;

  private:

  };
}

#endif
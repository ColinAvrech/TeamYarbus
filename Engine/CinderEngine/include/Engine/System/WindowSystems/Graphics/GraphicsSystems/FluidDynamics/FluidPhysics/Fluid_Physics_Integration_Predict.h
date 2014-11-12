/******************************************************************************/
/*!
\file   Fluid_Physics_Integration_Predict.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_INTEGRATION_PREDICTION
#define _FLUID_PHYSICS_INTEGRATION_PREDICTION

#include "Cinder_Engine_Data_Types.h"
#include "Fluid_Physics_Integration.h"
#include "Fluid_Physics_Object.h"


namespace Framework
{
  class Fluid_Physics_Predict : public Fluid_Physics_Integration
  {
  public:

    Fluid_Physics_Predict () : Fluid_Physics_Integration ()
    {}

    Fluid_Physics_Predict (const Fluid_Physics_Predict& other) : Fluid_Physics_Integration (other)
    {}

    virtual ~Fluid_Physics_Predict ()
    {}

    Fluid_Physics_Predict& operator= (const Fluid_Physics_Predict& source)
    {
      DEBUG_check_this_expression (this != &source);

      return *this;
    }

    virtual void UpdateVelocity (Fluid_Physics_Object & game_entit, const REAL32 delta_time);
    virtual void UpdatePosition (Fluid_Physics_Object & game_entity, const REAL32 delta_time);

  };
}

#endif
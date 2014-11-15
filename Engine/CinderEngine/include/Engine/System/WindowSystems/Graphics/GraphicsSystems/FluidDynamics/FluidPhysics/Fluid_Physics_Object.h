/******************************************************************************/
/*!
\file   Fluid_Physics_Object.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_OBJECT
#define _FLUID_PHYSICS_OBJECT

#include "Cinder_Engine_Data_Types.h"
#include "Math_Vector2D.h"
#include "Debug_Defines.h"

namespace Framework
{
  class Fluid_Physics_Object
  {
  public:

    Fluid_Physics_Object () : Position (0.0f), OldPosition (0.0f), Velocity (0.0f), Force (0.0f){}

    Fluid_Physics_Object (const Fluid_Physics_Object & source) :
      Position (source.Position),
      OldPosition (source.OldPosition),
      Velocity (source.Velocity),
      Force (source.Force)
    {}

    virtual ~Fluid_Physics_Object (){}

    const Fluid_Physics_Object & operator= (const Fluid_Physics_Object & other)
    {
      DEBUG_check_this_expression (this != &other);

      Position = other.Position;
      OldPosition = other.OldPosition;
      Velocity = other.Velocity;
      Force = other.Force;

      return *this;
    }

    // Gettors
    const MATH_VECTOR_2D& GetPosition () const
    {
      return Position;
    }

    const MATH_VECTOR_2D & GetVelocity () const
    {
      return Velocity;
    }

    const MATH_VECTOR_2D & GetForce () const
    {
      return Force;
    }

    // Settors
    void SetPosition (const MATH_VECTOR_2D& position)
    {
      Position = position;
    }

    void SetOldPosition (const MATH_VECTOR_2D& old_position)
    {
      OldPosition = old_position;
    }

    void SetVelocity (const MATH_VECTOR_2D& velocity)
    {
      Velocity = velocity;
    }

    void SetForce (const MATH_VECTOR_2D& force)
    {
      Force = force;
    }

    void AccumalateForce (const MATH_VECTOR_2D& force)
    {
      Force += force;
    }

    const MATH_VECTOR_2D & GetOldPosition () const
    {
      return OldPosition;
    }

  protected:
    MATH_VECTOR_2D Velocity;
    MATH_VECTOR_2D Position;
    MATH_VECTOR_2D Force;
    MATH_VECTOR_2D OldPosition;

  };
}

#endif
/******************************************************************************/
/*!
\file   Test_Mouse.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#ifndef _TEST_MOUSE_H
#define _TEST_MOUSE_H

#include "Cinder_Engine_Data_Types.h"
#include "Math_Vector2D.h"
#include "Fluid_Physics_Particle_Array.h"
#include "Fluid_Physics_Object.h"

namespace Framework
{
  class Test_Mouse
  {
  public:

    Test_Mouse () : MouseAttractorVector (MATH_VECTOR_2D (999.0f, 999.0f))
    {}

    Test_Mouse (Test_Mouse& source) : MouseAttractorVector (source.MouseAttractorVector)
    {}

    virtual ~Test_Mouse ()
    {}

    void SetMouse (const BOOL attract, const BOOL repel)
    {
      IsAttracting = attract;
      IsRepelling = repel;
    }

    void SetAttractor (const MATH_VECTOR_2D& attractor)
    {
      MouseAttractorVector = attractor;
    }

    void ApplyMouseBehaviour (Fluid_Physics_Object& game_entity, const REAL32 max_dis_attractor);

  private:
    BOOL IsAttracting;
    BOOL IsRepelling;
    MATH_VECTOR_2D MouseAttractorVector;

  };
}

#endif
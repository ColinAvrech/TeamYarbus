/******************************************************************************/
/*!
\file   Fluid_Physics_Spring.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Fluid_Physics_Spring.h"

namespace Framework
{
  void Fluid_Physics_Spring::CalculateForce (
    MATH_VECTOR_2D & force,
    const Fluid_Physics_Spring & spring,
    const Fluid_Physics_Object & point1,
    const Fluid_Physics_Object & point2
    )
  {
    MATH_VECTOR_2D
      vector_between_point1_and_point2;
    REAL32
      current_length;

    vector_between_point1_and_point2.SetDifference (
      point1.GetPosition (),
      point2.GetPosition ()
      );

    current_length = vector_between_point1_and_point2.GetLength ();
    vector_between_point1_and_point2.normalise ();
    force = vector_between_point1_and_point2
      * (spring.GetRestLength () - current_length)
      * spring.GetCoefficient ();
  }
}
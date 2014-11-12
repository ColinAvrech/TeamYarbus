/******************************************************************************/
/*!
\file   Fluid_Physics_Constants.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_CONSTANTS
#define _FLUID_PHYSICS_CONSTANTS

#include "Math_Vector2D.h"

namespace Framework
{
#define FLUID_PHYSICS_GRAVITY \
  MATH_VECTOR_2D (0, -0.1f)

#define FLUID_PHYSICS_SPRING_COEFF 0.5f
}

#endif
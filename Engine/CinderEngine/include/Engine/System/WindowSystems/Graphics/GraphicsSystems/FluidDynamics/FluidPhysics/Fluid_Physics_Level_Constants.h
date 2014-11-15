/******************************************************************************/
/*!
\file   Fluid_Physics_Level_Constants.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_LEVEL_CONSTANTS
#define _FLUID_PHYSICS_LEVEL_CONSTANTS

namespace Framework
{
#define FLUID_PHYSICS_PART_SIZE  4.0f

#define FLUID_PHYSICS_SMOOTHING_RAD \
  FLUID_PHYSICS_PART_SIZE

#define FLUID_PHYSICS_STIFFNESS_K    0.002f

#define FLUID_PHYSICS_NEAR_STIFFNESS_K  \
  FLUID_PHYSICS_STIFFNESS_K * 10.0f

#define FLUID_PHYSICS_REST_DENSITY 10.0f
#define FLUID_PHYSICS_L_SPRING_COEFF 0.2f
#define FLUID_PHYSICS_YIELD_RATIO 0.1f
#define FLUID_PHYSICS_PLASTICITY 0.5f
}

#endif
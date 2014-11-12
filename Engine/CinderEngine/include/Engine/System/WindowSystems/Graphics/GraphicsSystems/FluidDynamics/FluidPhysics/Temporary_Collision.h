/******************************************************************************/
/*!
\file   Temporary_Collision.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _TEMPORARY_COLLISION
#define _TEMPORARY_COLLISION


#include "Fluid_Physics_Object.h"
#include "Cinder_Engine_Data_Types.h"
#include "Math_Point2D.h"


namespace Framework
{
  class Temporary_Collision
  {
  public:

    void CheckForWall (
      Fluid_Physics_Object &entity,
      const REAL32 wall_spring_coefficient,
      const MATH_POINT_2D & position,
      const REAL32 min_x_value, const REAL32 max_x_value,
      const REAL32 min_y_value, const REAL32 max_y_value
      );

    bool LineCollision (
      const MATH_POINT_2D& A1, const MATH_POINT_2D& A2,
      const MATH_POINT_2D& B1, const MATH_POINT_2D& B2,
      double* out = 0
      );
  };
}


#endif
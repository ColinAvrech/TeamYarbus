/******************************************************************************/
/*!
\file   Fluid_Particle_Position_Loader.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_PARTICLE_POS_LOADER
#define _FLUID_PHYSICS_PARTICLE_POS_LOADER

#include "Cinder_Engine_Data_Types.h"
#include "Debug_Defines.h"
#include "Cinder_Engine_Image_Loader.h"
#include "Math_Point2D_Array.h"
#include "Math_Vector2D.h"

namespace Framework
{
  class Fluid_Physics_Particle_Position_Loader
  {
  public:

    Fluid_Physics_Particle_Position_Loader () : PointTable ()
    {}

    Fluid_Physics_Particle_Position_Loader (const Fluid_Physics_Particle_Position_Loader& source) : PointTable (source.PointTable)
    {}

    virtual ~Fluid_Physics_Particle_Position_Loader ()
    {}

    Fluid_Physics_Particle_Position_Loader& operator= (const Fluid_Physics_Particle_Position_Loader& other)
    {
      DEBUG_check_this_expression (this != &other);
      PointTable = other.PointTable;

      return *this;
    }

    // Gettors
    const MATH_POINT_2D_ARRAY& GetPointTable () const
    {
      return PointTable;
    }

    // Settors
    void SetPointTable (const MATH_POINT_2D_ARRAY& point_table)
    {
      PointTable = point_table;
    }

    void LoadPositions (const CHAR* bitmap);

  private:
    MATH_POINT_2D_ARRAY
      PointTable;

  };
}

#endif
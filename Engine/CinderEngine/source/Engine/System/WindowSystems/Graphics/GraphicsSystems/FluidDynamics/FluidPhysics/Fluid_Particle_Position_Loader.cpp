/******************************************************************************/
/*!
\file   Fluid_Particle_Position_Loader.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "Fluid_Particle_Position_Loader.h"
#include "Cinder_Engine_Data_Types.h"
#include "Math_Point2D.h"
#include "Cinder_Engine_Image_Loader.h"

namespace Framework
{
  void Fluid_Physics_Particle_Position_Loader::LoadPositions (const CHAR *bitmap)
  {
    UINT32
      width,
      height,
      colour;
    Cinder_Image_Loader
      image;
    UINT32
      index_x,
      index_y;

    Cinder_Image_Loader::LoadBMP (image, bitmap);
    width = image.GetWidth ();
    height = image.GetHeight ();

    CINDER_LOOP_THROUGH_INDEX (index_y, 0, height - 1)
    {
      CINDER_LOOP_THROUGH_INDEX (index_x, 0, width - 1)
      {
        colour = image.GetPixels () [3 * (index_y * width + index_x)];

        if (colour != -1)
        {
          PointTable.push_back (
            MATH_POINT_2D (
            REAL32 (INT32 (index_x - width / 2)),
            REAL32 (INT32 (index_y - height / 2))
            )
            );
        }
      }
    }
  }
}
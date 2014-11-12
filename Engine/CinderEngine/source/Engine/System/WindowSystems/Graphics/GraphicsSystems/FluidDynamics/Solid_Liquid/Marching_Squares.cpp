/******************************************************************************/
/*!
\file   Marching_Squares.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "Marching_Squares.h"
#include "Cinder_Engine_Data_Types.h"
#include "Math_Vector2D.h"

namespace Framework
{
  // Each square is created from four points

#define LOCAL_square_point_1 temp_index_1][temp_index_2 
#define LOCAL_square_point_2 temp_index_1][temp_index_2+1 
#define LOCAL_square_point_3 temp_index_1+1][temp_index_2+1 
#define LOCAL_square_point_4 temp_index_1+1][ temp_index_2 

  // If a point is not inside the IsoSurfaces this function
  // find a point between pointA( not inside the IsoSurface ) 
  // and PointB( inside the IsoSurface ) through a linear
  // interpolation.

  void Fluid_Marching_Squares::InterpolateBetweenPoints (
    MATH_POINT_2D & point,
    const MATH_POINT_2D &a,
    const MATH_POINT_2D &b,
    const REAL32 value1,
    const REAL32 value2
    ) const
  {
    if ((value2 - value1) != 0.0f)
    {
      point = a + (b - a) * (MARCHING_SQUARE_THRESHOLD - value1) / (value2 - value1);
    }
    else
    {
      point = a;
    }
  }


  void Fluid_Marching_Squares::Initialize (
    const REAL32 width,
    const REAL32 height
    )
  {
    INDEX
      temp_index_1,
      temp_index_2;
    MATH_POINT_2D
      point;
    REAL32
      initial_position_x,
      initial_position_y;

    Width = width;
    Height = height;
    initial_position_x = -width;
    initial_position_y = height;
    PointTable1.Allocate (COUNTER (width * 2 + 1));

    CINDER_LOOP_THROUGH_INDEX (temp_index_2, 0, INDEX (height * 2))
    {
      CINDER_LOOP_THROUGH_INDEX (temp_index_1, 0, INDEX (width * 2))
      {
        point.Assign (initial_position_x, initial_position_y);
        PointTable1 [temp_index_1].push_back (point);
        initial_position_x += 1.0f;
        if (initial_position_x > width)
        {
          initial_position_x = -width;
          initial_position_y -= 1.0f;
        }
      }
    }
    IsInsideSurface = new INT32* [INDEX (width) * 2];
    SurfaceValueTable = new REAL32* [INDEX (width) * 2 + 1];

    CINDER_LOOP_THROUGH_INDEX (temp_index_1, 0, INDEX (width) * 2 - 1)
    {
      IsInsideSurface [temp_index_1] = new INT32 [INDEX (width) * 2];
    }

    CINDER_LOOP_THROUGH_INDEX (temp_index_1, 0, INDEX (width) * 2)
    {
      SurfaceValueTable [temp_index_1] = new REAL32 [INDEX (width) * 2 + 1];
    }
    Reset ();
  }


  void Fluid_Marching_Squares::CalculatePoint (
    const MATH_VECTOR_2D & point,
    const INDEX square_starting_index_x_value,
    const INDEX square_starting_index_y_value
    )
  {
    REAL32 value;
    REAL32 h;

    h = (point
      - PointTable1 [square_starting_index_x_value][square_starting_index_y_value]
      ).GetSquareLength ();

    if (h != 0.0f)
    {
      value = 1.0f / h;
      if (value < 0.16f)
      {
        value = 0;
      }
      else
      {
        SurfaceValueTable [square_starting_index_x_value][square_starting_index_y_value]
          += value;
      }
    }
  }

  // ~~

  void Fluid_Marching_Squares::CalculatePoints (
    const MATH_VECTOR_2D & point,
    const INDEX x_value,
    const INDEX y_value,
    const INT32 range
    )
  {
    COUNTER
      counter_value_1,
      counter_value_2;

    CINDER_LOOP_THROUGH_INDEX (counter_value_1, -range, 0)
    {
      CINDER_LOOP_THROUGH_INDEX (counter_value_2, 0, range)
      {
        if ((x_value + counter_value_1) < (Width * 2 + 1) && (x_value + counter_value_1) >= 0)
        {
          if ((y_value + counter_value_2) < (Height * 2 + 1) && (y_value + counter_value_2) >= 0)
          {
            CalculatePoint (point, x_value + counter_value_1, y_value + counter_value_2);
          }
        }
      }
    }
    CINDER_LOOP_THROUGH_INDEX (counter_value_1, -range, 0)
    {
      CINDER_LOOP_THROUGH_INDEX (counter_value_2, -range, 0)
      {
        if ((x_value + counter_value_1) < (Width * 2 + 1) && (x_value + counter_value_1) >= 0)
        {
          if ((y_value - 1 + counter_value_2) < (Height * 2 + 1) && (y_value - 1 + counter_value_2) >= 0)
          {
            CalculatePoint (point, x_value + counter_value_1, y_value - 1 + counter_value_2);
          }
        }
      }
    }
    CINDER_LOOP_THROUGH_INDEX (counter_value_1, 0, range)
    {
      CINDER_LOOP_THROUGH_INDEX (counter_value_2, -range, 0)
      {
        if ((x_value + 1 + counter_value_1) < (Width * 2 + 1) && (x_value + 1 + counter_value_1) >= 0)
        {
          if ((y_value - 1 + counter_value_2) < (Height * 2 + 1) && (y_value - 1 + counter_value_2) >= 0)
          {
            CalculatePoint (point, x_value + 1 + counter_value_1, y_value - 1 + counter_value_2);
          }
        }
      }
    }
    CINDER_LOOP_THROUGH_INDEX (counter_value_1, 0, range)
    {
      CINDER_LOOP_THROUGH_INDEX (counter_value_2, 0, range)
      {
        if ((x_value + 1 + counter_value_1) < (Width * 2 + 1) && (x_value + 1 + counter_value_1) >= 0)
        {
          if ((y_value + counter_value_2) < (Height * 2 + 1) && (y_value + counter_value_2) >= 0)
          {
            CalculatePoint (point, x_value + 1 + counter_value_1, y_value + counter_value_2);
          }

        }
      }
    }
  }

  // ~~

  void Fluid_Marching_Squares::Reset ()
  {
    INDEX
      temp_index_1,
      temp_index_2;

    CINDER_LOOP_THROUGH_INDEX (temp_index_1, 0, INDEX (Width) * 2)
    {
      CINDER_LOOP_THROUGH_INDEX (temp_index_2, 0, INDEX (Height) * 2)
      {
        SurfaceValueTable [temp_index_1][temp_index_2] = 0.0f;
      }
    }
    CINDER_LOOP_THROUGH_INDEX (temp_index_1, 0, INDEX (Width) * 2 - 1)
    {
      CINDER_LOOP_THROUGH_INDEX (temp_index_2, 0, INDEX (Height) * 2 - 1)
      {
        IsInsideSurface [temp_index_1][temp_index_2] = 0;
      }
    }

    PointTable2.clear ();
    PolygonVertexCountTable.clear ();
  }


  // Main Function
  // Use grid in fluid engine to solve for each point - IsInsideSurface
  void Fluid_Marching_Squares::GeneratePoints ()
  {
    INDEX
      temp_index_1,
      temp_index_2,
      index_x,
      index_y;
    MATH_VECTOR_2D
      result;

    CINDER_LOOP_THROUGH_INDEX (temp_index_1, 0, Width * 2 - 1)
    {
      index_x = temp_index_1;

      CINDER_LOOP_THROUGH_INDEX (temp_index_2, 0, Height * 2 - 1)
      {
        index_y = temp_index_2;

        if (
          SurfaceValueTable [LOCAL_square_point_1]
          > MARCHING_SQUARE_THRESHOLD
          )
        {
          // Operator OR with 0001 is been used to show 
          // that the first point of the square is inside 
          // the IsoSurface.

          IsInsideSurface [index_x][index_y] |= 1;
        }
        if (
          SurfaceValueTable [LOCAL_square_point_2]
        > MARCHING_SQUARE_THRESHOLD
        )
        {
          // Operator OR with 0010 is been used to show 
          // that the second point of the square is inside 
          // the IsoSurface.

          IsInsideSurface [index_x][index_y] |= 2;
        }
        if (
          SurfaceValueTable [LOCAL_square_point_3]
          > MARCHING_SQUARE_THRESHOLD
          )
        {
          // Operator OR with 0100 is been used to show 
          // that the third point of the square is inside 
          // the IsoSurface.

          IsInsideSurface [index_x][index_y] |= 4;
        }
        if (
          SurfaceValueTable [LOCAL_square_point_4]
        > MARCHING_SQUARE_THRESHOLD
        )
        {
          // Operator OR with 0100 is been used to show 
          // that the fourth point of the square is inside 
          // the IsoSurface.

          IsInsideSurface [index_x][index_y] |= 8;
        }
      }
    }

    CINDER_LOOP_THROUGH_INDEX (temp_index_1, 0, Width * 2 - 1)
    {
      index_x = temp_index_1;

      CINDER_LOOP_THROUGH_INDEX (temp_index_2, 0, Height * 2 - 1)
      {
        index_y = temp_index_2;

        switch (IsInsideSurface [index_x][index_y])
        {
        case 15:
          PolygonVertexCountTable.push_back (4);
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_2]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_4]
            );
          break;

        case 14:
          PolygonVertexCountTable.push_back (5);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_1],
            PointTable1 [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_4]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_1],
            PointTable1 [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_2]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_2]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_4]
            );
          break;

        case 13:
          PolygonVertexCountTable.push_back (5);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_2],
            PointTable1 [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_2],
            PointTable1 [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_4]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_1]
            );
          break;

        case 12:
          PolygonVertexCountTable.push_back (4);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_1],
            PointTable1 [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_4]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_2],
            PointTable1 [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_4]
            );
          break;

        case 11:
          PolygonVertexCountTable.push_back (5);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_3],
            PointTable1 [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_2]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_3],
            PointTable1 [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_4]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_4]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_2]
            );
          break;

        case 10:
          PolygonVertexCountTable.push_back (6);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_1],
            PointTable1 [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_4]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_1],
            PointTable1 [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_2]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_2]
            );

          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_3],
            PointTable1 [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_2]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_3],
            PointTable1 [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_4]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_4]
            );
          break;

        case 9:
          PolygonVertexCountTable.push_back (4);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_2],
            PointTable1 [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_3],
            PointTable1 [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_4]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_4]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_1]
            );
          break;

        case 8:
          PolygonVertexCountTable.push_back (3);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_1],
            PointTable1 [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_4]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_3],
            PointTable1 [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_4]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_4]
            );
          break;

        case 7:
          PolygonVertexCountTable.push_back (5);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_4],
            PointTable1 [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_4],
            PointTable1 [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_2]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_3]
            );
          break;

        case 6:
          PolygonVertexCountTable.push_back (4);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_1],
            PointTable1 [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_2]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_4],
            PointTable1 [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_2]
            );
          break;

        case 5:
          PolygonVertexCountTable.push_back (6);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_4],
            PointTable1 [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_4],
            PointTable1 [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_1]
            );

          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_2],
            PointTable1 [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_2],
            PointTable1 [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_3]
            );
          break;

        case 4:
          PolygonVertexCountTable.push_back (3);
          InterpolateBetweenPoints (result,
            PointTable1 [LOCAL_square_point_4],
            PointTable1 [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_2],
            PointTable1 [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_3]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_3]
            );
          break;

        case 3:
          PolygonVertexCountTable.push_back (4);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_3],
            PointTable1 [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_2]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_4],
            PointTable1 [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_2]
            );
          break;

        case 2:
          PolygonVertexCountTable.push_back (3);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_3],
            PointTable1 [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_3],
            SurfaceValueTable [LOCAL_square_point_2]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_1],
            PointTable1 [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_2]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_2]
            );
          break;

        case 1:
          PolygonVertexCountTable.push_back (3);
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_2],
            PointTable1 [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_2],
            SurfaceValueTable [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            result
            );
          InterpolateBetweenPoints (
            result,
            PointTable1 [LOCAL_square_point_4],
            PointTable1 [LOCAL_square_point_1],
            SurfaceValueTable [LOCAL_square_point_4],
            SurfaceValueTable [LOCAL_square_point_1]
            );
          PointTable2.push_back (
            result
            );
          PointTable2.push_back (
            PointTable1 [LOCAL_square_point_1]
            );
          break;
        }
      }
    }
  }
}
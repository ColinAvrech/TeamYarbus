/******************************************************************************/
/*!
\file   Marching_Squares.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _MARCHING_SQUARES_H
#define _MARCHING_SQUARES_H

#include "Cinder_Engine_Data_Types.h"
#include "Math_Vector2D.h"
#include "Math_Point2D.h"
#include "Math_Point2D_Array.h"
#include "Math_Point2D_Array2D.h"
#include "Cinder_Engine_Index_Array.h"


namespace Framework
{
#define MARCHING_SQUARE_THRESHOLD 1.55f


  class Fluid_Marching_Squares
  {
  public:

    Fluid_Marching_Squares () :
      IsInsideSurface (0),
      SurfaceValueTable (0),
      Width (0.0f),
      Height (0.0f),
      PointTable2 (),
      PointTable1 (),
      PolygonVertexCountTable ()
    {}


    virtual ~Fluid_Marching_Squares ()
    {
      INDEX
        temp_index;

      CINDER_LOOP_THROUGH_INDEX (temp_index, 0, INDEX (Width) * 2)
      {
        delete [] SurfaceValueTable [temp_index];
      }
      CINDER_LOOP_THROUGH_INDEX (temp_index, 0, INDEX (Width) * 2 - 1)
      {
        delete [] IsInsideSurface [temp_index];
      }
      delete SurfaceValueTable;
      delete IsInsideSurface;
    }


    Fluid_Marching_Squares (const Fluid_Marching_Squares& source) :
      Width (source.Width),
      Height (source.Height),
      PointTable2 (source.PointTable2),
      PointTable1 (source.PointTable1),
      PolygonVertexCountTable (source.PolygonVertexCountTable)
    {
      INDEX
        temp_index_1,
        temp_index_2;

      CINDER_LOOP_THROUGH_INDEX (temp_index_1, 0, Width * 2)
      {
        CINDER_LOOP_THROUGH_INDEX (temp_index_2, 0, Height * 2)
        {
          SurfaceValueTable [temp_index_1][temp_index_2]
            = source.SurfaceValueTable [temp_index_1][temp_index_2];
        }
      }
      CINDER_LOOP_THROUGH_INDEX (temp_index_1, 0, Width * 2 - 1)
      {
        CINDER_LOOP_THROUGH_INDEX (temp_index_2, 0, Height * 2 - 1)
        {
          IsInsideSurface [temp_index_1][temp_index_2]
            = source.IsInsideSurface [temp_index_1][temp_index_2];
        }
      }
    }


    Fluid_Marching_Squares & operator= (const Fluid_Marching_Squares& source)
    {
      IsInsideSurface = source.IsInsideSurface;
      SurfaceValueTable = source.SurfaceValueTable;
      Width = source.Width;
      Height = source.Height;
      PointTable2 = source.PointTable2;
      PointTable1 = source.PointTable1;
      PolygonVertexCountTable = source.PolygonVertexCountTable;
    }


    const MATH_POINT_2D_ARRAY & GetPointTable () const
    {
      return PointTable2;
    }

    void SetPointTable (const MATH_POINT_2D_ARRAY & point_table)
    {
      PointTable2 = point_table;
    }

    const Cinder_Index_Array & GetPolygonVertexCountTable () const
    {
      return PolygonVertexCountTable;
    }


    void SetPolygonVertexCountTable (const Cinder_Index_Array & polygon_vertex_count_table)
    {
      PolygonVertexCountTable = polygon_vertex_count_table;
    }

    void InterpolateBetweenPoints (
      MATH_POINT_2D & point,
      const MATH_POINT_2D & a,
      const MATH_POINT_2D & b,
      const REAL32 value1,
      const REAL32 value2
      ) const;

    void Reset ();

    void Initialize (const REAL32 width, const REAL32 height);

    void CalculatePoint (
      const MATH_VECTOR_2D& point,
      const INDEX square_starting_index_x_value,
      const INDEX square_starting_index_y_value
      );


    void CalculatePoints (
      const MATH_VECTOR_2D& point,
      const INDEX x_value,
      const INDEX y_value,
      const INT32 range
      );

    void GeneratePoints ();

  private:
    INT32** IsInsideSurface;
    REAL32** SurfaceValueTable;
    REAL32 Width;
    REAL32 Height;
    MATH_POINT_2D_ARRAY PointTable2;
    MATH_POINT_2D_ARRAY_ARRAY PointTable1;
    Cinder_Index_Array PolygonVertexCountTable;
  };
}

#endif
/******************************************************************************/
/*!
\file   Fluid_Physics_Spring.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_SPRING_H
#define _FLUID_PHYSICS_SPRING_H

#include "Fluid_Physics_Object.h"
#include "Cinder_Engine_Data_Types.h"
#include "Debug_Defines.h"


namespace Framework
{
  class Fluid_Physics_Spring
  {
  public:

    Fluid_Physics_Spring () : Point1Index (0), Point2Index (0), RestLength (0.0f), Coefficient (0.0f)
    {}

    Fluid_Physics_Spring (const Fluid_Physics_Spring& source) :
      Point1Index (source.Point1Index),
      Point2Index (source.Point2Index),
      RestLength (source.RestLength),
      Coefficient (source.Coefficient)
    {}

    virtual ~Fluid_Physics_Spring ()
    {}

    Fluid_Physics_Spring& operator= (const Fluid_Physics_Spring& source)
    {
      DEBUG_check_this_expression (this != &source);
      Point1Index = source.Point1Index;
      Point2Index = source.Point2Index;
      RestLength = source.RestLength;
      Coefficient = source.Coefficient;

      return *this;
    }

    // Gettors
    INT32 GetPoint1Index () const
    {
      return Point1Index;
    }

    INT32 GetPoint2Index () const
    {
      return Point2Index;
    }

    REAL32 GetRestLength () const
    {
      return RestLength;
    }

    REAL32 GetCoefficient () const
    {
      return Coefficient;
    }

    REAL32 GetCoefficientOfDamping () const
    {
      return CoefficientOfDamping;
    }

    // Settors
    void SetPoint1Index (INT32 point1_index)
    {
      Point1Index = point1_index;
    }

    void SetPoint2Index (INT32 point2_index)
    {
      Point2Index = point2_index;
    }

    void SetCoefficient (const REAL32 coefficient)
    {
      Coefficient = coefficient;
    }

    void SetCoefficientOfDamping (const REAL32 coefficientOfDamping)
    {
      CoefficientOfDamping = CoefficientOfDamping;
    }


    void SetRestLength (const REAL32 rest_length)
    {
      RestLength = rest_length;
    }

    static void CalculateForce (
      MATH_VECTOR_2D & force,
      const Fluid_Physics_Spring & spring,
      const Fluid_Physics_Object & point1,
      const Fluid_Physics_Object & point2
      );


  private:
    INT32 Point1Index;
    INT32 Point2Index;
    REAL32 RestLength;
    REAL32 Coefficient;
    REAL32 CoefficientOfDamping;

  };
}

#endif
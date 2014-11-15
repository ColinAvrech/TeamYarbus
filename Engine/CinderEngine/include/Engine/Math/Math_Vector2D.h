/******************************************************************************/
/*!
\file   Math_Vector2D.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _CINDER_ENGINE_MATH_VECTOR_2D
#define _CINDER_ENGINE_MATH_VECTOR_2D

#include "Debug_Defines.h"
#include "Cinder_Engine_Data_Types.h"
#include "Math_Square_Root.h"

namespace Framework
{
  class MATH_VECTOR_2D
  {
  public:

    MATH_VECTOR_2D () : X (0), Y (0)
    {}

    MATH_VECTOR_2D (const REAL32 x_value) : X (x_value), Y (x_value)
    {}

    MATH_VECTOR_2D (const REAL32 x_value, const REAL32 y_value) : X (x_value), Y (y_value)
    {}

    MATH_VECTOR_2D (const MATH_VECTOR_2D& source) : X (source.X), Y (source.Y)
    {}


    virtual ~MATH_VECTOR_2D ()
    {}

    MATH_VECTOR_2D& operator= (const MATH_VECTOR_2D& rhs)
    {
      DEBUG_check_this_expression (this != &rhs);

      X = rhs.X;
      Y = rhs.Y;

      return *this;
    }

    MATH_VECTOR_2D operator+ (const MATH_VECTOR_2D& rhs) const
    {
      return MATH_VECTOR_2D (X + rhs.X, Y + rhs.Y);
    }

    MATH_VECTOR_2D operator- (const MATH_VECTOR_2D & rhs) const
    {
      return MATH_VECTOR_2D (X - rhs.X, Y - rhs.Y);
    }

    MATH_VECTOR_2D operator* (const MATH_VECTOR_2D& rhs) const
    {
      return MATH_VECTOR_2D (X * rhs.X, Y * rhs.Y);
    }

    MATH_VECTOR_2D operator/ (const MATH_VECTOR_2D& rhs) const
    {
      return MATH_VECTOR_2D (X / rhs.X, Y / rhs.Y);
    }

    void operator += (const MATH_VECTOR_2D& rhs)
    {
      X += rhs.X;
      Y += rhs.Y;
    }

    void operator -= (const MATH_VECTOR_2D& rhs)
    {
      X -= rhs.X;
      Y -= rhs.Y;
    }

    void operator*= (const MATH_VECTOR_2D& rhs)
    {
      X *= rhs.X;
      Y *= rhs.Y;
    }

    VOID operator/= (const MATH_VECTOR_2D& rhs)
    {
      X /= rhs.X;
      Y /= rhs.Y;
    }

    BOOL operator== (
      const MATH_VECTOR_2D & other
      )
    {
      return ((X == other.X) && (Y == other.Y));
    }

    // .. INQUIRIES

    REAL32 GetLength (
      VOID
      ) const
    {
      return Cinder_Square_Root::GetSquareRoot (X * X + Y * Y);
    }

    // ~~

    REAL32 GetSquareLength (
      VOID
      ) const
    {
      return (X * X + Y * Y);
    }

    // ~~

    REAL32 GetDotProduct (
      const MATH_VECTOR_2D & other
      ) const
    {
      return X * other.X + Y * other.Y;
    }

    // .. OPERATIONS

    VOID Assign (
      const REAL32 x_value,
      const REAL32 y_value
      )
    {
      X = x_value;
      Y = y_value;
    }

    // ~~

    void Reset (
      VOID
      )
    {
      X = 0.0f;
      Y = 0.0f;
    }

    // ~~

    BOOL normalise (
      VOID
      )
    {
      if (GetLength () != 0.0f)
      {
        X = X / GetLength ();
        Y = Y / GetLength ();

        return true;
      }
      else
      {
        X = 0.0f;
        Y = 0.0f;

        return false;
      }
    }

    // ~~

    VOID SetDifference (
      const MATH_VECTOR_2D & first,
      const MATH_VECTOR_2D & second
      )
    {
      X = second.X - first.X;
      Y = second.Y - first.Y;
    }

    // .. ATTRIBUTES

    REAL32
      X,
      Y;
  };
}

#endif

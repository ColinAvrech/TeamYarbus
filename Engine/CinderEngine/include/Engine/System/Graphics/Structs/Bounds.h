/******************************************************************************/
/*!
\file   Bounds.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _BOUNDS_H
#define _BOUNDS_H

/*------------------------------------------------------------------------------
// Includes
------------------------------------------------------------------------------*/
#pragma region Includes
#include "Vec2.h"
#pragma endregion

namespace Framework
{

/*------------------------------------------------------------------------------
// Class
------------------------------------------------------------------------------*/
#pragma region Class
  struct Bounds
  {

/*------------------------------------------------------------------------------
// Constructors
------------------------------------------------------------------------------*/
#pragma region Constructors
    Bounds (Vec2& center, Vec2& size);
#pragma endregion


/*------------------------------------------------------------------------------
// Public Variables - Methods
------------------------------------------------------------------------------*/
#pragma region Public

    // VARIABLES
    Vec2 center;
    Vec2 max;
    Vec2 min;

    // METHODS
    bool Contains (Vec2& point);
    void Encapsulate (Bounds& bounds);
    void Encapsulate (Vec2& point);
    void Expand (float amount);
    void Expand (Vec2& amount);
    bool Intersects (Bounds& bounds);
    void SetMinMax (Vec2& min, Vec2 max);
    float SqrDistance (Vec2& point);
    Vec2 size ();
    Vec2 extents ();
#pragma endregion

/*------------------------------------------------------------------------------
// Static / Other
------------------------------------------------------------------------------*/
#pragma region Static / Other
    friend bool operator != (Bounds& lhs, Bounds& rhs);
    friend bool operator == (Bounds& lhs, Bounds& rhs);
#pragma endregion
  };

  //----------------------------------------------------------------------------
#pragma endregion
}

//------------------------------------------------------------------------------

#endif				// _BOUNDS_H
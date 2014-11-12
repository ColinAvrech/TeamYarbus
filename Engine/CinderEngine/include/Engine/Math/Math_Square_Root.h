/******************************************************************************/
/*!
\file   Math_Square_Root.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _CINDER_ENGINE_MATH_SQUARE_ROOT_H
#define _CINDER_ENGINE_MATH_SQUARE_ROOT_H

#include "Cinder_Engine_Data_Types.h"

namespace Framework
{
  class Cinder_Square_Root
  {
  public:

    Cinder_Square_Root ()
    {}

    Cinder_Square_Root (Cinder_Square_Root& other)
    {}

    virtual ~Cinder_Square_Root ()
    {}

    Cinder_Square_Root& operator=(const Cinder_Square_Root& other)
    {
      return *this;
    }

    // Algorithm taken from USC
    static REAL32 GetSquareRoot (const REAL32 value)
    {
      union
      {
        INT32 i;
        REAL32 value;
      } u;
      u.value = value;
      u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
      return u.value;
    }

  };
}

#endif
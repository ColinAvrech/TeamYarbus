/******************************************************************************/
/*!
\file   Cinder_Engine_Color.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _GRAPHICS_COLOR_H
#define _GRAPHICS_COLOR_H


#include "Cinder_Engine_Data_Types.h"
#include "Debug_Defines.h"

namespace Framework
{
  class Graphics_Color
  {
  public:

    Graphics_Color ()
    {
      Assign (0.0, 0.0, 0.0, 0.0);
    }

    Graphics_Color (const FLOAT r, const FLOAT g, const FLOAT b, const FLOAT a)
    {
      Assign (r, g, b, a);
    }

    Graphics_Color (const FLOAT* rgba)
    {
      Assign (rgba [0], rgba [1], rgba [2], rgba [3]);
    }

    virtual ~Graphics_Color ()
    {}

    Graphics_Color (const Graphics_Color& source)
    {
      RGBA [0] = source.RGBA [0];
      RGBA [1] = source.RGBA [1];
      RGBA [2] = source.RGBA [2];
      RGBA [3] = source.RGBA [3];
    }


    Graphics_Color & operator= (const Graphics_Color & source)
    {
      DEBUG_check_this_expression (this != &source);

      RGBA [0] = source.RGBA [0];
      RGBA [1] = source.RGBA [1];
      RGBA [2] = source.RGBA [2];
      RGBA [3] = source.RGBA [3];

      return *this;
    }

    const FLOAT& operator[] (INDEX index) const
    {
      return RGBA [index];
    }


    FLOAT& operator [] (INDEX index)
    {
      return RGBA [index];
    }


    // Gettors
    const FLOAT* GetRGBA () const
    {
      return RGBA;
    }

    void Assign (const FLOAT r, const FLOAT g, const FLOAT b, const FLOAT a)
    {
      RGBA [0] = r;
      RGBA [1] = g;
      RGBA [2] = b;
      RGBA [3] = a;
    }


    static Graphics_Color Black ()
    {
      return Graphics_Color (0.0f, 0.0f, 0.0f, 1.0f);
    }

    static Graphics_Color Grey ()
    {
      return Graphics_Color (0.5f, 0.5f, 0.5f, 1.0f);
    }

    static Graphics_Color White ()
    {
      return Graphics_Color (1.0f, 1.0f, 1.0f, 1.0f);
    }

    static Graphics_Color Red ()
    {
      return Graphics_Color (1.0f, 0.0f, 0.0f, 1.0f);
    }

    static Graphics_Color Green ()
    {
      return Graphics_Color (0.0f, 1.0f, 0.0f, 1.0f);
    }

    static Graphics_Color Blue ()
    {
      return Graphics_Color (0.0f, 0.0f, 1.0f, 1.0f);
    }

    static Graphics_Color Yellow ()
    {
      return Graphics_Color (1.0f, 1.0f, 0.0f, 1.0f);
    }

    static Graphics_Color Brown ()
    {
      return Graphics_Color (0.2f, 0.1f, 0.1f, 1.0f);
    }

    static Graphics_Color Null ()
    {
      return Graphics_Color (0.0f, 0.0f, 0.0f, 0.0f);
    }

  private:
    FLOAT RGBA [4];

  };
}

#endif
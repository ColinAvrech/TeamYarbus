/******************************************************************************/
/*!
\file   Template.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The Transform Component holds Position, Rotation, and Scale for
all objects.
*/
/******************************************************************************/


#pragma once

#include "Component.h"
#include "Affine.h"
#include "GOC.h"
namespace Framework
{
  class Transform : public GameComponent
  {
  public:
    Transform(GOC* obj)
      :Position(obj->GOCPosition), Rotation(0){}
    Transform ();
    double Rotation;
    Vec2& Position;


    // Transform Methods
    void Translate (Vec2&);
    void Translate (float x, float y);
    void Rotate (float z);
    void ScaleXY (Vec2&);
    void ScaleXY (float rx, float ry);

    //! Add serialization overload

  private:
    Affine _transform;
  };
}
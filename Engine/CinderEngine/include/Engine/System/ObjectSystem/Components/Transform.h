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
#include "Vec2.h"
#include "Affine.h"

namespace Framework
{
  class Transform : public GameComponent
  {
  public:
    Transform(GameObject* obj)
      : GameComponent(obj){}



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
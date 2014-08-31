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
#include "GOC.h"
namespace Framework
{
  class Transform : public GameComponent
  {
    Transform(GOC* obj)
      :Position(obj->GOCPosition), Rotation(0){}
    double Rotation;
    Vec2& Position;
    //! Add serialization overload
  };
}
/******************************************************************************/
/*!
\file   InputEvent.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#pragma once

#include "Common.h"
#include "Vec2.h"
#include "BaseEvent.h"

namespace Framework
{

  class MouseEvent : public BaseEvent
  {
  public:

    bool ButtonDown;
    glm::vec2 MousePosition;
  };
}
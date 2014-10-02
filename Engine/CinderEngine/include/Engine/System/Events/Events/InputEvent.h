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
#include "BaseEvent.h"

namespace Framework
{

  class MouseEvent : public BaseEvent
  {
  public:
    typedef void(*MouseCall)(GameObject*, MouseEvent*);

    MouseEvent(const std::string)
    {}

    ~MouseEvent()
    {}

    void DispatchEvent()
    {
      for each(auto it in Listeners)
      {
        ((MouseCall)(it.second))(it.first, this);
      }
    }
    
    Vec2 MouseScreenPosition;
    Vec2 MouseScenePosition;
  };
}
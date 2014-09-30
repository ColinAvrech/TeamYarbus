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

  class GameEvent : public BaseEvent
  {
  public:
    typedef void(*GameCall)(GameObject*, GameEvent*);
    GameEvent(const std::string)
    {}

    ~GameEvent()
    {}

    void DispatchEvent()
    {
      for each(auto it in Listeners)
      {
        ((GameCall)(it.second))(it.first, this);
      }
    }

  };
}
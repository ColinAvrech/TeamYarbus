/******************************************************************************/
/*!
\file   InputEvent.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Sends an event Whenever there is a key state change
*/
/******************************************************************************/

#pragma once

#include "Common.h"
#include "BaseEvent.h"

namespace Framework
{

  class KeyEvent : public BaseEvent
  {
  public:
    typedef void(*KeyCall)(GameObject*, KeyEvent*);

    KeyEvent(const std::string)
    {}

    ~KeyEvent()
    {}

    void DispatchEvent()
    {
      for each(auto it in Listeners)
      {
        ((KeyCall)(it.second))(it.first, this);
      }
    }

    double TimePassed;
    bool KeyDown;
    unsigned KeyValue;
  };
}
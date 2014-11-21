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


// Old Use BaseEvent to make customEvents
namespace Framework
{

  // Basic GameEvent for callbacks
  class GameEvent : public BaseEvent
  {
  public:

  };

  // Basic GameEvent for callbacks
  class UIEvent : public BaseEvent
  {
  public:
    std::string Message;
  };
}
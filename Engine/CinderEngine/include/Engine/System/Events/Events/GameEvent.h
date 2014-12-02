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
	  ZilchDeclareBaseType(GameEvent, Zilch::TypeCopyMode::ReferenceType);

  };

  // Basic GameEvent for callbacks
  class UIEvent : public BaseEvent
  {
  public:
    ZilchDeclareBaseType(UIEvent, Zilch::TypeCopyMode::ReferenceType);
    Zilch::String Message;
  };

  class PauseEvent : public BaseEvent
  {
  public:
    ZilchDeclareBaseType (PauseEvent, Zilch::TypeCopyMode::ReferenceType);
    bool Paused;
  };
}
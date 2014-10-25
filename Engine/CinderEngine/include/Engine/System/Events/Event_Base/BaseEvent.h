/******************************************************************************/
/*!
\file   BaseEvent.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The BaseEvent is inherited from into all events and is used as
and interface for every type of event.
*/
/******************************************************************************/


#pragma once

#include "Common.h"
#include "GameObject.h"

namespace Framework
{

  class BaseEvent
  {
  public:
    /*!The EventSystem will need to see all elements of the BaseEvent*/
    friend class EventSystem;

    //static unsigned int totalevents;

    BaseEvent::BaseEvent()
    {
    }

    BaseEvent::~BaseEvent()
    {
    }
  };


}
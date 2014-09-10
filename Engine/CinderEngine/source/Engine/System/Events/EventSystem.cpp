/******************************************************************************/
/*!
\file   EventSystem.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "EventSystem.h"

namespace Framework
{
  //! Global pointer to  the Event System
  EventSystem* EVENTSYSTEM = NULL;

  EventSystem::EventSystem()
  {
    ErrorIf(EVENTSYSTEM != NULL, "EVENTSYSTEM ALREADY CREATED");
    EVENTSYSTEM = this;
  }

  EventSystem::~EventSystem()
  {
  }

  /**********************************************************
  //
  //    CONNECTING TO EVENTS
  //
  ***********************************************************/
  /*!Connect to an input event*/

  /**********************************************************
  //
  //    EVENTS EXIST
  //
  ***********************************************************/

  /*Returns NULL if no event found*/

  /**********************************************************
  //
  //    CREATING EVENTS
  //
  ***********************************************************/

  void EventSystem::Update(const double dt)
  {

  }

}
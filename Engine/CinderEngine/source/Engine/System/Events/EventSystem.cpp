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
#include "BaseEvent.h"

#include "Events.h"
#include "MouseEvent.h"
#include "UpdateEvent.h"
#include "KeyEvent.h"
#include "GameEvent.h"
#include "CollisionEvent.h"

#include "ZilchCompiledLib.h"
#include "Zilch.hpp"

#include "Delegates.h"

//ZilchDefineType(EventSystem, CinderZilch)
//{
//  ZilchBindMethod(zConnect);
//}

namespace Framework
{

  //! Global pointer to  the Event System
  EventSystem* EVENTSYSTEM = NULL;

  EventSystem::EventSystem()
  {
    ErrorIf(EVENTSYSTEM != NULL, "EVENTSYSTEM ALREADY CREATED");
    EVENTSYSTEM = this;
  }

  /// CLEAN CLEAN CLEAN NEEDS TO BE ADDEDD !#!@#!@#!@#!@# TODO
  EventSystem::~EventSystem()
  {
    // Clean UP STUFF
  }

  bool EventSystem::Initialize()
  {
    std::cout << CinderConsole::yellow << "Initializing EventSystem" << std::endl;
    return true;
  }


  void zConnect(const std::string eventname)
  {

  }

  void EventSystem::TriggerEvent(const std::string eventname, BaseEvent& e)
  {
    auto eDeployer = RegisteredEvents.find(eventname);

    if (eDeployer == RegisteredEvents.end())
    {
      return;
    }
    else
    {
      eDeployer->second->TriggerEvent(e);
    }
  }

  /* Returns the total Number of Events */
  unsigned EventSystem::NumberOfEvents()
  {
    return RegisteredEvents.size();
  }


  /******************************************/
  //  Private
  /******************************************/

  /* The Update Function which is called in the main loop*/
  void EventSystem::Update(const double dt)
  {
    UpdateEvent updateEvent;
    updateEvent.Dt = static_cast<float>(dt);
    updateEvent.TimePassed += static_cast<float>(dt);
    
    TriggerEvent(Events::UPDATEEVENT, updateEvent);

  }

}
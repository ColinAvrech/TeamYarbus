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

  /// CLEAN CLEAN CLEAN NEEDS TO BE ADDEDD !#!@#!@#!@#!@# TODO
  EventSystem::~EventSystem()
  {
    // Clean UP STUFF
  }

  bool EventSystem::Initialize()
  {
    std::cout << Console::yellow << "Initializing EventSystem" << std::endl;
    if (RegisteredEvents.find(Events::LOGICUPDATE) == RegisteredEvents.end())
    {
      RegisteredEvents[Events::LOGICUPDATE] = new UpdateEvent(Events::LOGICUPDATE);
    }
    return true;
  }

  void EventSystem::Connect(GameObject* obj, const std::string eventname, BaseEvent::BaseCall func)
  {
    EventMap::iterator eventToConnectTo = RegisteredEvents.find(eventname);
    
    // if event does not exist, create a Listen for it
    if (eventToConnectTo == RegisteredEvents.end())
    {
      //Logic Update should always be made by the Event Manager
      if (eventname.substr(0, eventname.find_first_of("_")) == Events::Types::KEY)
      {
        RegisteredEvents[eventname] = new KeyEvent(eventname);
        RegisteredEvents[eventname]->Listeners.push_back(BaseEvent::BaseEventListener(obj, func));
      }
      else if (eventname.substr(0, eventname.find_first_of("_")) == Events::Types::MOUSE)
      {
        RegisteredEvents[eventname] = new MouseEvent(eventname);
        RegisteredEvents[eventname]->Listeners.push_back(BaseEvent::BaseEventListener(obj, func));
      }
      else if (eventname.substr(0, eventname.find_first_of("_")) == Events::Types::GAME)
      {
        RegisteredEvents[eventname] = new GameEvent(eventname);
        RegisteredEvents[eventname]->Listeners.push_back(BaseEvent::BaseEventListener(obj, func));
      }
      else if (eventname.substr(0, eventname.find_first_of("_")) == Events::Types::COLLISION)
      {
        RegisteredEvents[eventname] = new GameEvent(eventname);
        RegisteredEvents[eventname]->Listeners.push_back(BaseEvent::BaseEventListener(obj, func));
      }
    }
    else //Sign up to be called by the base event 
    {
      RegisteredEvents[eventname]->Listeners.push_back(BaseEvent::BaseEventListener(obj, func));
    }
  }

  void EventSystem::TriggerEvent(const std::string eventname)
  {
    EventMap::iterator eventToTrigger = RegisteredEvents.find(eventname);
    RETURNIF(eventToTrigger == RegisteredEvents.end());
    
    if (eventname.substr(0, eventname.find_first_of("_")) == Events::Types::COLLISION)
    {
      static_cast<CollisionEvent*>(eventToTrigger->second)->DispatchEvent();
    }
    else if (eventname.substr(0, eventname.find_first_of("_")) == Events::LOGICUPDATE)
    {
      static_cast<UpdateEvent*>(eventToTrigger->second)->Dt = _dt;
      static_cast<UpdateEvent*>(eventToTrigger->second)->TimePassed = _TotalTimePassed;
      static_cast<UpdateEvent*>(eventToTrigger->second)->DispatchEvent();
    }
    else if (eventname.substr(0, eventname.find_first_of("_")) == Events::Types::KEY)
    {
      // Event system will disbatch an event the frame after this event stops being sent to tell the button has been lifted
      // TODO ^
      static_cast<KeyEvent*>(eventToTrigger->second)->DispatchEvent();
    }
    else if (eventname.substr(0, eventname.find_first_of("_")) == Events::Types::MOUSE)
    {
      static_cast<MouseEvent*>(eventToTrigger->second)->DispatchEvent();
    }
    else if (eventname.substr(0, eventname.find_first_of("_")) == Events::Types::GAME)
    {
      static_cast<GameEvent*>(eventToTrigger->second)->DispatchEvent();
    }

  }

  unsigned EventSystem::NumberOfEvents()
  {
    return RegisteredEvents.size();
  }


  /******************************************/
  //  Private
  /******************************************/

  void EventSystem::Update(const double dt)
  {
    _dt = dt;
    _TotalTimePassed += dt;

    TriggerEvent(Events::LOGICUPDATE);
  }

}
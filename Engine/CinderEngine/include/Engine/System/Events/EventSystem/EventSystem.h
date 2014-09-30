
/******************************************************************************/
/*!
\file   EventSystem.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#pragma once

#include "Common.h"
#include "BaseSystem.h"
#include "BaseEvent.h"

#include "Events.h"

#include "MouseEvent.h"
#include "UpdateEvent.h"
#include "KeyEvent.h"
#include "GameEvent.h"


namespace Framework
{

  class EventSystem : public BaseSystem
  {
  public:
    EventSystem();
    ~EventSystem();

    virtual void Update(const double dt);

    virtual const std::string GetName() { return "EventSystem";}

    virtual bool Initialize();

    unsigned NumberOfEvents();

    void Connect(GameObject* obj, const std::string eventname, BaseEvent::BaseCall func);
    void TriggerEvent(const std::string eventname);

  private:

    double _TotalTimePassed = 0;
    double _dt = 0;

    typedef std::unordered_map<std::string, BaseEvent*> EventMap;
    EventMap RegisteredEvents;


  };

  extern EventSystem* EVENTSYSTEM;
}

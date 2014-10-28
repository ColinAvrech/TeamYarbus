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
#include "Zilch.hpp"


namespace Framework
{
  class EventDeployer;
  class BaseEvent;

  class EventSystem : public BaseSystem
  {
  public:
    //ZilchDeclareBaseType(EventSystem, TypeCopyMode::ReferenceType);

    EventSystem ();
    ~EventSystem ();

    virtual void Update (const double dt);

    virtual const std::string GetName () { return "EventSystem"; }

    virtual bool Initialize ();

    unsigned NumberOfEvents ();


    //Global functions use this connect function to conntect to events.
    template<typename EventType>
    void gConnect (const std::string eventname, void (*func)(EventType*));

    // C++ Member functions use this connect function to connect to events.
    template<typename EventType, typename ClassType>
    void mConnect (const std::string eventname, ClassType *this_ptr, void(ClassType::*func)(EventType*));

    void zConnect(const std::string eventname);

    void TriggerEvent (const std::string eventname, BaseEvent& e);

  private:

    double _TotalTimePassed = 0;
    double _dt = 0;

    typedef std::unordered_map<std::string, EventDeployer*> EventMap;
    EventMap RegisteredEvents;
  };

  extern EventSystem* EVENTSYSTEM;

}

#include "EventSystem.tpp"
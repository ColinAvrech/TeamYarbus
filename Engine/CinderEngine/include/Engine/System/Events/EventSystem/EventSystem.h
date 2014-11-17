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
#include "ZilchCompiledLib.h"


namespace Framework
{
  class EventDeployer;
  class BaseEvent;
  class UpdateEvent;

  class EventSystem : public BaseSystem
  {
  public:
    ZilchDeclareBaseType(EventSystem, Zilch::TypeCopyMode::ReferenceType);

    EventSystem ();
    ~EventSystem ();

    virtual void Update (const double& dt);

    virtual const string GetName () { return "EventSystem"; }

    virtual bool Initialize ();

    unsigned NumberOfEvents ();


    //Global functions use this connect function to conntect to events.
    template<typename EventType>
    void gConnect (const string eventname, void (*func)(EventType*));

    // C++ Member functions use this connect function to connect to events.
    template<typename EventType, typename ClassType>
    void mConnect (const string eventname, ClassType *this_ptr, void(ClassType::*func)(EventType*));

    void EventSystem::zConnect(Call& call, ExceptionReport& report);

    void TriggerEvent (const string eventname, BaseEvent& e);
    
    double _TotalTimePassed = 0;
    double _dt = 0;

    typedef std::unordered_map<string, EventDeployer*> EventMap;
    EventMap RegisteredEvents;
  };

  extern EventSystem* EVENTSYSTEM;

}

#include "EventSystem.tpp"
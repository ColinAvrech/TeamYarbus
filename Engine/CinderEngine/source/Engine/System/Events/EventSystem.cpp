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

#include "InputManager.h"
#include "Delegates.h"

namespace Framework
{

  //! Global pointer to  the Event System
  EventSystem* EVENTSYSTEM = NULL;

  void ZilchConnect(Call& call, ExceptionReport& report);

  ZilchDefineType(EventSystem, CinderZilch)
  {
    ParameterArray parameters;
    DelegateParameter& p0 = parameters.push_back();
    p0.Name = "eventName";
    p0.ParameterType = ZilchTypeId(String);
    DelegateParameter& p1 = parameters.push_back();
    p1.Name = "function";
    p1.ParameterType = Core::GetInstance().AnyDelegateType;
    builder.AddBoundFunction(type, "Connect", ZilchConnect, parameters, ZilchTypeId(void), FunctionOptions::Static);
  }

  EventSystem::EventSystem()
  {
    ErrorIf(EVENTSYSTEM != NULL, "EVENTSYSTEM ALREADY CREATED");
    EVENTSYSTEM = this;
  }

  EventSystem::~EventSystem(){}

  bool EventSystem::Initialize()
  {
    std::cout << CinderConsole::yellow << "Initializing EventSystem" << std::endl;
    return true;
  }

  // Function called from Zilch Script to register to events.
  void ZilchConnect(Call& call, ExceptionReport& report)
  {
    string eventname = call.Get<String>(0).c_str();
    Zilch::Delegate zDelegate = call.GetDelegate(1);

    auto event = EVENTSYSTEM->RegisteredEvents.find(eventname);

    //If deployer does not exist
    if (event == EVENTSYSTEM->RegisteredEvents.end())
    {
      //Add Deployer
      auto eDeployer = new EventDeployer();
      EVENTSYSTEM->RegisteredEvents[eventname] = eDeployer;

      //Add Delegate
      if (eventname[0] == UpdateEventPrefix)
      {
        //Add UpdateDelegate
        ZilchDelegate<UpdateEvent>* zd = new ZilchDelegate<UpdateEvent>();
        zd->Function = zDelegate;
        eDeployer->AddDelegate(zd);
      }
      else if (eventname[0] == KeyEventPrefix)
      {
        //Add UpdateDelegate
        ZilchDelegate<KeyEvent>* zd = new ZilchDelegate<KeyEvent>();
        zd->Function = zDelegate;
        eDeployer->AddDelegate(zd);
      }
      else if (eventname[0] == CollisionEventPrefix)
      {
        //Add UpdateDelegate
        ZilchDelegate<CollisionEvent>* zd = new ZilchDelegate<CollisionEvent>();
        zd->Function = zDelegate;
        eDeployer->AddDelegate(zd);
      }
    }
    else
    {
      // Get Deployer
      auto eDeployer = event->second;

      //Add Delegate
      if (eventname[0] == UpdateEventPrefix)
      {
        //Add UpdateDelegate
        ZilchDelegate<UpdateEvent>* zd = new ZilchDelegate<UpdateEvent>();
        zd->Function = zDelegate;
        eDeployer->AddDelegate(zd);
      }
      else if (eventname[0] == KeyEventPrefix)
      {
        //Add UpdateDelegate
        ZilchDelegate<KeyEvent>* zd = new ZilchDelegate<KeyEvent>();
        zd->Function = zDelegate;
        eDeployer->AddDelegate(zd);
      }
      else if (eventname[0] == CollisionEventPrefix)
      {
        //Add UpdateDelegate
        ZilchDelegate<CollisionEvent>* zd = new ZilchDelegate<CollisionEvent>();
        zd->Function = zDelegate;
        eDeployer->AddDelegate(zd);
      }
    }
  }

  void EventSystem::TriggerEvent(const string eventname, BaseEvent& e)
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

  // Disconnet all events
  void EventSystem::DeleteAllEvents()
  {
    for each(auto regEvent in  RegisteredEvents)
    {
      regEvent.second->DisconnectAll();
    }
    RegisteredEvents.clear();
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
  void EventSystem::Update(const double &dt)
  {
    UpdateEvent updateEvent;
    updateEvent.Dt = static_cast<float>(dt);
    _TotalTimePassed += static_cast<float>(dt);
    updateEvent.TimePassed = static_cast<float>(_TotalTimePassed);
    
    TriggerEvent(Events::UPDATEEVENT, updateEvent);
    TriggerEvent("UpdateEvent", updateEvent);

	InputManager::Update();
  }

}
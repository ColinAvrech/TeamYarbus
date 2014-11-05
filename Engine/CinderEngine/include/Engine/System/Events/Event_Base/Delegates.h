/******************************************************************************/
/*!
\file   Delegates.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The BaseEvent is inherited from into all events and is used as
and interface for every type of event.
*/
/******************************************************************************/


#pragma once


#include "Common.h"
#include "BaseEvent.h"
#include "ZilchCompiledLib.h"

namespace Framework
{
  //forward Declaration
  class BaseEvent;
  class GameObject;

  struct Delegate
  {
    virtual void Invoke(BaseEvent* e) = 0;
    virtual ~Delegate() {};
  };

  // The base event from which all over events inherit
  class EventDeployer
  {
  public:

    void AddDelegate(Delegate* delegate_)
    {
      Delegates.push_back(delegate_);
    }

    void TriggerEvent(BaseEvent& e);

    void DisconnectAll();

  private:
    typedef std::vector<Delegate*> EventDelegates;
    EventDelegates Delegates;
  };

  /********************************************************************/
  // Event Delagates
  /********************************************************************/


  // Global Delegate
  template<typename EventType>
  struct GlobalDelegate : public Delegate
  {
    typedef void(*Signature)(EventType* e);
    Signature Function;

    virtual void Invoke(BaseEvent* e) override
    {
      EventType* castedEvent = static_cast<EventType*>(e);
      if (castedEvent == nullptr)
      {
        ErrorIf(true, "ERROR Tried to invoke the wrong event type");
      }
      Function(castedEvent);
    }
  };

  //Member function delegate
  template<typename EventType, typename ClassType>
  struct MemberDelegate : public Delegate
    {
      typedef void(ClassType::*Signature)(EventType* e);

      Signature Function;
      ClassType* This_ptr;

      virtual void Invoke(BaseEvent* e) override
      {
        EventType* castedEvent = static_cast<EventType*>(e);
        if (castedEvent == nullptr)
        {
          ErrorIf(true, "ERROR Tried to invoke the wrong event type");
        }
        (This_ptr->*Function)(castedEvent);
      }
    };

  template<typename EventType>
  struct ZilchDelegate : public Delegate
  {
    Zilch::Delegate Function;
    
    virtual void Invoke(BaseEvent* e) override
    {
      EventType* castedEvent = static_cast<EventType*>(e);
      if (castedEvent == nullptr)
      {
        ErrorIf(true, "ERROR Tried to invoke the wrong event type");
      }

      ExceptionReport report;
      Zilch::Call call(Function, ZILCH->GetDependencies());
      call.Set<EventType*>(0, castedEvent);
      call.Invoke(report);
    }

  };

}
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

namespace Framework
{

  class BaseEvent
  {
  public:
    /*!The EventSystem will need to see all elements of the BaseEvent*/
    friend class EventSystem;

    //static unsigned int totalevents;

    BaseEvent::BaseEvent(Events::Types eventType)
      : type(eventType)
    {
      //++totalevents;
    }

    BaseEvent::~BaseEvent()
    {
      //--totalevents;
    }
    
    double Dt() { return _dt; }
    const Events::Types type; 

    /*static unsigned int GetTotalEvents()
    {
      return totalevents;
    }*/

  protected:

    void AddDelagate();

    struct EventDelegate
    {
      GOC *obj = NULL;

      /*!All possible types of Events and their related data*/
      union Action
      {
        void *Input(GOC &obj, const class InputEvent &Event);
        void *Engine(GOC &obj, const class EngineEvent &Event);
        void *Grapics(GOC &obj, const class GraphicsEvent &Event);
        void *Windows(GOC &obj, const class WindowsEvent &Event);
        void *Audio(GOC &obj, const class AudioEvent &Event);
        void *Physics(GOC &obj, const class PhysicsEvent &Event);
        void *Script(GOC &obj, const class ScriptEvent &Event);
      };
    };

    std::vector<EventDelegate> Delagates;

  private:

    /*!dt of the frame the event wa*/
    double _dt;
  };

}

/*!

/auther Micah
/file EventSystem.h

/brief Contains all of the framework for creating and maintaining a window.

*/
#pragma once

#include "BaseSystem.h"
#include "Events.h"
#include "GOC.h"
#include <vector>


namespace Framework
{

  class EventSystem : public BaseSystem
  {
  public:
    EventSystem();
    ~EventSystem();

    virtual void Update(const double dt);

    virtual const std::string GetName() { return "EventSystem";}

    /*!All of the Connection functions to listen to events*/
    bool Connect(GOC &obj, Events::Types type, Events::InputKeys key);

  private:

    /*!Check if event exists*/
    InputEvent* EventExists(Events::InputKeys key);
    bool EventExists(Events::Types type);

    /*!Create Event*/
    InputEvent* CreateEvent(Events::InputKeys key);

    /*!List of pointers to Events which will be cast to their specific Events based on their name*/
    std::vector<BaseEvent*> EventList;
    
  };

  extern EventSystem* EVENTSYSTEM;
}

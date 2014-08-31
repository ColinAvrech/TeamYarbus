/*!

/auther Micah
/file EventSystem.cpp

/brief Contains all of the framework for creating and maintaining a window.

*/

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
  bool EventSystem::Connect(GOC &obj, Events::Types type, Events::InputKeys key)
  {
    //!Check to see if event exists
    InputEvent* inputEvent = EventExists(key);

    if (inputEvent == NULL)
    {

    }
    else
    {

    }
    
    return true;
  }

  /**********************************************************
  //
  //    EVENTS EXIST
  //
  ***********************************************************/

  /*Returns NULL if no event found*/
  InputEvent* EventSystem::EventExists(Events::InputKeys key)
  {
    for (unsigned i = 0; i < EventList.size(); ++i)
    {
      if (EventList[i]->type == Events::Types::Input)
      {
        InputEvent* input = (InputEvent*)EventList[i];
        if (input->inputKey == key)
          return input;
      }
    }
    return NULL;
  }

  /**********************************************************
  //
  //    CREATING EVENTS
  //
  ***********************************************************/

  InputEvent* EventSystem::CreateEvent(Events::InputKeys key)
  {
    InputEvent* input = new InputEvent(key);
    Assert(input != NULL, "COULDN'T CREATE INPUTEVENT: ", key);
    EventList.push_back((BaseEvent*)input);
    return input;
  }

  void EventSystem::Update(const double dt)
  {

  }

}
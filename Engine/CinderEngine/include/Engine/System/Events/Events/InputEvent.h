/******************************************************************************/
/*!
\file   InputEvent.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#pragma once


#include <vector>
#include "CinderDebug.h"

namespace Framework
{

  class InputEvent : public BaseEvent
  {
  public:

    InputEvent::InputEvent(Events::InputKeys inputKey)
      : BaseEvent(Events::Types::Input), inputKey(inputKey)
    {
      //! Check for duplicate Key events
#ifdef _DEBUG
      for (auto it = _registeredKeys.begin(); it != _registeredKeys.end(); ++it)
      {
        Assert(*it != inputKey, "ERROR: InputEvent Key DUPLICATED!");
      }

      //!Push key onto the list
      _registeredKeys.push_back(inputKey);
#endif
    }

    ~InputEvent(){}

    /*!Key for this Event*/
    const Events::InputKeys inputKey;

    //TODO VEC2 for mouse position
    

  private:
#ifdef _DEBUG
    /*!To check so that we don't create multiple events of the same type and key*/
    std::vector<Events::InputKeys> _registeredKeys;
#endif
  };
}
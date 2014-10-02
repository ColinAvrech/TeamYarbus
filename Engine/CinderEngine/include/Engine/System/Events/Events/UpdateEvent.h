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

#include "Common.h"
#include "BaseEvent.h"

namespace Framework
{

  class UpdateEvent : public BaseEvent
  {

  public:

    typedef void(*UpdateCall)(GameObject*, UpdateEvent*);

    UpdateEvent(const std::string eventname)
    {}

    ~UpdateEvent()
    {}
    
    void DispatchEvent()
    {
      for each(auto it in Listeners)
      {
        ((UpdateCall)(it.second))(it.first, this);
      }
    }

    double TimePassed;
    
  };

}
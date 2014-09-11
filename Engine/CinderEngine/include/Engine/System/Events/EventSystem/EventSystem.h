
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

#include "BaseSystem.h"

namespace Framework
{

  class EventSystem : public BaseSystem
  {
  public:
    EventSystem();
    ~EventSystem();

    virtual void Update(const double dt);

    virtual const std::string GetName() { return "EventSystem";}


  private:
        
  };

  extern EventSystem* EVENTSYSTEM;
}

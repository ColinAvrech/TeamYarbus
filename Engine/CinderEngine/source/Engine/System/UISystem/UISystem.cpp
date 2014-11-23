/******************************************************************************/
/*!
\file   UISystem.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "UISystem.h"
#include "Eventsystem.h"
#include "GameEvent.h"
#include "Core.h"

namespace Framework
{
  //! Global pointer to  the windows system.
  UISystem* UISYSTEM = NULL;
  UIListener UISystem::listener;

  // Constructor
  UISystem::UISystem ()
  {
    UISYSTEM = this;
  }
  
  // Destructor
  UISystem::~UISystem ()
  {
  }

  
  // Called once when System is Initialized
  bool UISystem::Initialize ()
  {
    std::cout << GetName () + " initialized\n";
    listener.Initialize ();
    return true;
  }
  
  // Called Every Frame
  void UISystem::Update (const double& dt)
  {}

  UIListener::UIListener ()
  {
  }

  UIListener::~UIListener ()
  {
  }

  void UIListener::Initialize ()
  {
    EVENTSYSTEM->mConnect<UIEvent, UIListener> (Events::UI, this, &UIListener::MessageHandler);
  }

  void UIListener::MessageHandler (UIEvent* ui)
  {
    if (ui->Message == "Exit")
    {
      //CORE->QuitGame ();
    }
    else if (ui->Message == "Restart")
    {
      //OBJECTSYSTEM->RestartLevel ();
    }
  }

}
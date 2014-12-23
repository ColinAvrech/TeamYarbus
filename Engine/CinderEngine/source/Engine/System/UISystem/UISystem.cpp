/******************************************************************************/
/*!
\file   UISystem.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>
#include "UISystem.h"
#include "Eventsystem.h"
#include "GameEvent.h"
#include "CinderEngine_UI.h"
#include "Core.h"
#include "Events.h"

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
  void UISystem::Update (const float& dt)
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
      Sprite* dialog = reinterpret_cast<Sprite*> (OBJECTSYSTEM->FindObjectByName("Dialog")->GetComponent("Sprite"));
      UIBox* yes = reinterpret_cast<UIBox*> (OBJECTSYSTEM->FindObjectByName("Yes")->GetComponent("UIBox"));
      UIBox* no = reinterpret_cast<UIBox*> (OBJECTSYSTEM->FindObjectByName("No")->GetComponent("UIBox"));
      dialog->enabled = true;
      yes->enabled = true;
      no->enabled = true;
    }
    else if (ui->Message == "Yes")
    {
      CORE->QuitGame();
    }
    else if (ui->Message == "No")
    {
      Sprite* dialog = reinterpret_cast<Sprite*> (OBJECTSYSTEM->FindObjectByName("Dialog")->GetComponent("Sprite"));
      UIBox* yes = reinterpret_cast<UIBox*> (OBJECTSYSTEM->FindObjectByName("Yes")->GetComponent("UIBox"));
      UIBox* no = reinterpret_cast<UIBox*> (OBJECTSYSTEM->FindObjectByName("No")->GetComponent("UIBox"));
      dialog->enabled = false;
      yes->enabled = false;
      no->enabled = false;
    }
    else if (ui->Message == "Restart")
    {
      OBJECTSYSTEM->RestartLevel();
    }
    else if (ui->Message == "Resume")
    {
      CORE->TogglePaused();
    }
    else if (ui->Message == "How To Play")
    {
      //Set how to play pane visibility to true
      //Set other panes visibility to false
    }
    else if (ui->Message == "Mute")
    {
      //TODO: Change this to set volume to 0
      if(AUDIOSYSTEM->GetMuted())
      {
        AUDIOSYSTEM->SetPaused(false);
      }
      else
      {
        AUDIOSYSTEM->SetPaused();
      }
    }
  }

}
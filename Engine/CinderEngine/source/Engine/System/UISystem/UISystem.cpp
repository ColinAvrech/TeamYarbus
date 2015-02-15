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
#include "UILayer.h"

namespace Framework
{
  //! Global pointer to  the windows system.
  UISystem* UISYSTEM = NULL;

  // Constructor
  UISystem::UISystem ()
  {
    UISYSTEM = this;
  }
  
  // Destructor
  UISystem::~UISystem ()
  {
    this->Clear();
  }

  
  // Called once when System is Initialized
  bool UISystem::Initialize ()
  {
    std::cout << GetName () + " initialized\n";
    return true;
  }
  
  // Called Every Frame
  void UISystem::Update (const float& dt)
  {}

  bool UISystem::UpdatesOnPaused()
  {
    return true;
  }

  void UISystem::AddLayer(const char *layername)
  {
#ifdef _DEBUG
    //look for other layers with same name
    for (auto it = UILayers.begin(); it != UILayers.end(); ++it)
    {
      if (std::string((*it)->GetName()).compare(layername) == 0)
      {
        std::cout << CinderConsole::red << "Warning!" << CinderConsole::white <<
          "Multiple layers have the same name." << std::endl;
      }
    }
#endif
    UILayers.push_back(new UILayer(layername));
  }

  void UISystem::Clear()
  {
    for (auto* it : UILayers)
    {
      delete it;
    }
  }
}
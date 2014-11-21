/******************************************************************************/
/*!
\file   MenuController.h
\author Micah
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains all of the framework for creating and maintaining a window.
*/
/******************************************************************************/


#pragma once

#include "Common.h"
#include "Component.h"

namespace Framework
{
  class GameObject;
  class UpdateEvent;
  class PingEvent;


  struct Button
  {
    unsigned ActiveState = 0;
    GameObject* Obj = nullptr;
  };

  typedef struct Button Button;

  class MenuController : public Component
  {
  public:

    MenuController() {}
    virtual ~MenuController();

    virtual void Initialize();
    virtual void Serialize(Serializer::DataNode* data);

    // Add Buttons who ping
    void AddButtonsWhoPing(PingEvent * e);

    // Function to check mouse position and collision with buttons
    void MenuControllerUpdate(UpdateEvent* e);


    std::vector<Button> Buttons;
    std::vector<Button> ActiveMenuButtons;
    GameObject * SelectedButton;
  };


  class ButtonController : public Component
  {
  public:

    ButtonController() {}
    virtual ~ButtonController();

    // Sign-up for Level started to sign up to the MenuController
    virtual void Initialize();
    virtual void Serialize(Serializer::DataNode* data);

    unsigned ActiveState = 0;
  };


} // namespace Framework
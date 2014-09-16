/******************************************************************************/
/*!
\file   WindowSystem.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This system creates a window for the graphics system. It also holds the
function to handle windows Messages.
*/
/******************************************************************************/

#include "windowsystem.h"
#include "Core.h"

namespace Framework
{
  //! Global pointer to  the windows system.
  WindowSystem* WINDOWSYSTEM = NULL;
  
  //! This is for the message handling precedure in the game
  void WindowSystem::GLFWMessageHandler(GLFWwindow* window, int key, int scanCode, int state, int mod)
  {


  }


  WindowSystem::WindowSystem(const char* WindowTitle, int ClientWidth, int ClientHeight)
  {

  }


  WindowSystem::~WindowSystem()
  {

  }
  
  void WindowSystem::Update(const double dt)
  {
    WindowsUpdate(dt);
    GraphicsUpdate(dt);
  }

  void WindowSystem::WindowsUpdate(const double dt)
  {
    
  }

  void WindowSystem::GraphicsUpdate(const double dt)
  {

  }

}


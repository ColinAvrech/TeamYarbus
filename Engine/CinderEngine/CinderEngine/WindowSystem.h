/*!

/auther Micah
/file windowSystem.h

/brief Contains all of the framework for creating and maintaining a window.

*/
#pragma once

#include "BaseSystem.h"
#include "CinderWindows.h"

namespace Framework
{


  class WindowSystem : public BaseSystem
  {
  public:
    WindowSystem(HINSTANCE hInst, const char* WindowTitle, int ClientWidth, int ClientHeight);
    ~WindowSystem();

    
    void ActivateWindow();
    virtual void Update(const double dt);
    virtual const std::string GetName() { return "WindowSystem"; }

    HWND hWnd;
    HINSTANCE hInstance;
  };


  extern WindowSystem* WINDOWSYSTEM;
}
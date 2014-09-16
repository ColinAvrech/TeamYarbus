/*!

/auther Micah
/file windowSystem.h

/brief Contains all of the framework for creating and maintaining a window.

*/
#pragma once

#include "BaseSystem.h"
#include "GLFW.h"

namespace Framework
{

  class WindowSystem : public BaseSystem
  {
  public:
    WindowSystem(const char* WindowTitle, int ClientWidth, int ClientHeight);
    ~WindowSystem();

    virtual void Update(const double dt);
    virtual const std::string GetName() { return "WindowSystem"; }

    void WindowsUpdate(const double dt);
    void GraphicsUpdate(const double dt);


    void GLFWMessageHandler(GLFWwindow* window, int key, int scanCode, int state, int mod);

    unsigned WindowHeight;
    unsigned WindowWidth;
  };

  extern WindowSystem* WINDOWSYSTEM;
}
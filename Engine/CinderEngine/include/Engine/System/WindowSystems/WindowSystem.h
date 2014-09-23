/*!

/auther Micah
/file windowSystem.h

/brief Contains all of the framework for creating and maintaining a window.

*/
#pragma once

#include "BaseSystem.h"
#include "GraphicsCommon.h"


namespace Framework
{

  class WindowSystem : public BaseSystem
  {
  public:
    WindowSystem(const char* WindowTitle, int ClientWidth, int ClientHeight);
    ~WindowSystem();

    virtual bool Initialize ();
    virtual void Update(const double dt);
    virtual const std::string GetName() { return "WindowSystem"; }

    void WindowsUpdate(const double dt);
    void GraphicsUpdate(const double dt);

    void (*GLFWMessageHandler)(GLFWwindow* window, int key, int scanCode, int state, int mod);
    void (*GLFWMouseButtonFunction)(GLFWwindow *, int button, int action, int mod);
    void (*GLFWMouseCursorMoved) (GLFWwindow* window, double xPos, double yPos);


  private:
    GLFWwindow* window;

    unsigned WindowHeight;
    unsigned WindowWidth;

    void Setup_Buffer_Objects ();
  };

  extern WindowSystem* WINDOWSYSTEM;
}
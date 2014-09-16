/*!

/auther Micah
/file windowSystem.h

/brief Contains all of the framework for creating and maintaining a window.

*/
#pragma once

#include "BaseSystem.h"
#include "glew.h"
#include "glfw3.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "opengl32.lib")

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

  private:
    GLFWwindow* window;

    unsigned WindowHeight;
    unsigned WindowWidth;
  };

  extern WindowSystem* WINDOWSYSTEM;
}
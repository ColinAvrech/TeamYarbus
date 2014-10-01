/*!

/auther Micah
/file windowSystem.h

/brief Contains all of the framework for creating and maintaining a window.

*/
#pragma once

#include "BaseSystem.h"
#include "GraphicsCommon.h"
#include "glfw3.h"


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

    void  GLFWMessageHandler(GLFWwindow* window, int key, int scanCode, int state, int mod);
    void GLFWMouseButtonFunction(GLFWwindow *, int button, int action, int mod);
    void GLFWMouseCursorMoved (GLFWwindow* window, double xPos, double yPos);

    void Resize (GLFWwindow* window, int w, int h);

    unsigned Get_Width ();
    unsigned Get_Height ();
    void Set_W_H (unsigned, unsigned);

  private:
    GLFWwindow* window;

    unsigned WindowHeight;
    unsigned WindowWidth;
  };

  extern WindowSystem* WINDOWSYSTEM;
}
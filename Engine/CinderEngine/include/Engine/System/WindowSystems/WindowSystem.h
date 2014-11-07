/*!

/auther Micah
/file windowSystem.h

/brief Contains all of the framework for creating and maintaining a window.

*/
#pragma once

#include "BaseSystem.h"
#include "GraphicsCommon.h"
#include "glfw3.h"
#include "KeyEvent.h"
#include "IGraphicsObject.h"


namespace Framework
{

  namespace WindowNameSpace
  {
    void SetupKeyEvent(KeyEvent* triggered_key_event, int key, int scanCode, int state, int mod);
    void TriggerKeyEvent(const std::string eventname, int key, int scanCode, int state, int mod);
  }

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

    // GLFW callbacks
    void GLFWMessageHandler(GLFWwindow* window, int key, int scanCode, int state, int mod);
    void GLFWMouseButtonFunction(GLFWwindow *, int button, int action, int mod);
    void GLFWMouseCursorMoved (GLFWwindow* window, double xPos, double yPos);
    void GLFWWindowClosed(GLFWwindow* window);

    void Resize (GLFWwindow* window, int w, int h);
    void FrameBufferResize (GLFWwindow* _window, int w, int h);

    unsigned Get_Width ();
    unsigned Get_Height ();
    inline GLFWwindow* Get_Window ()
    {
      return window;
    }
    void Set_W_H (unsigned, unsigned);

    static std::list <Transform*> transformList;
    static std::list <IGraphicsObject*> graphicsObjects;

  private:
    GLFWwindow* window;


    unsigned WindowHeight;
    unsigned WindowWidth;
  };

  extern WindowSystem* WINDOWSYSTEM;
}
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

  namespace WindowNameSpace
  {
    void GLFWMessageHandler(GLFWwindow* window, int key, int scanCode, int state, int mod)
    {

    }

    void Create_Context(GLFWwindow** GLFWwindowptr)
    {
      glfwInit();

      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

      glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

      *GLFWwindowptr = glfwCreateWindow(400, 400, "CinderEngine", nullptr, nullptr); // Windowed

      if (!GLFWwindowptr)
      {
        glfwTerminate();
        exit(EXIT_FAILURE);
      }

      glfwMakeContextCurrent(*GLFWwindowptr);
      glfwSetKeyCallback(*GLFWwindowptr, GLFWMessageHandler);
    }

    void Init_Glew()
    {
      glewInit();
      std::cout << "OpenGl Version: " << Console::green << glGetString(GL_VERSION) << Console::gray << std::endl;
    }

  }

  WindowSystem::WindowSystem(const char* WindowTitle, int ClientWidth, int ClientHeight)
  {
    WindowNameSpace::Create_Context(&window);
    WindowNameSpace::Init_Glew();
  }

  WindowSystem::~WindowSystem()
  {
    glfwTerminate();
  }
  
  void WindowSystem::Update(const double dt)
  {
    WindowsUpdate(dt);
    GraphicsUpdate(dt);
  }

  void WindowSystem::WindowsUpdate(const double dt)
  {
    glfwSwapBuffers(window);
  }

  void WindowSystem::GraphicsUpdate(const double dt)
  {

  }


  ////////////////////////////////////////////////////////////
  



}
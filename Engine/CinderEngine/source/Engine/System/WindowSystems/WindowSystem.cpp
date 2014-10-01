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
#include "GraphicsCommon.h"
#include "Resources.h"
#include "ResourceManager.h"
#include "ShapeGenerator.h"
#include "Core.h"
#include "AudioSystem.h"
#include "SceneManager.h"
#include "ComponentInclude.h"

namespace Framework
{
  //! Global pointer to  the windows system.
  WindowSystem* WINDOWSYSTEM = NULL;

  namespace WindowNameSpace
  {

    void Resize (GLFWwindow* window, int w, int h)
    {
      WINDOWSYSTEM->Set_W_H (w, h);
      SCENEMANAGER->Change_Size (w, h);
    }

    void  GLFWMessageHandler (GLFWwindow* window, int key, int scanCode, int state, int mod)
    {
      SCENEMANAGER->Key_Pressed (key, scanCode, state, mod);
    }

    void GLFWMouseButtonFunction (GLFWwindow *, int button, int action, int mod)
    {
      SCENEMANAGER->Mouse_Input (button, action, mod);
    }
    void GLFWMouseCursorMoved (GLFWwindow* window, double xPos, double yPos)
    {
      SCENEMANAGER->Mouse_Position (xPos, yPos);
    }

    void Create_Context(GLFWwindow** GLFWwindowptr)
    {
      // Init GLFW Before Using Any Functionality
      glfwInit ();
      // Properties
      glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
      glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      glfwWindowHint (GLFW_RESIZABLE, GL_TRUE);

      WINDOWSYSTEM->Set_W_H (1920 , 1080);
      // Window Creation
      *GLFWwindowptr = glfwCreateWindow (WINDOWSYSTEM->Get_Width (), WINDOWSYSTEM->Get_Height (), "OpenGL", nullptr, nullptr); // Windowed

      //*GLFWwindowptr = glfwCreateWindow (800, 600, "OpenGL", glfwGetPrimaryMonitor (), nullptr);
      glfwMakeContextCurrent (*GLFWwindowptr);

      // Set Callback Functions
      glfwSetKeyCallback(*GLFWwindowptr, GLFWMessageHandler);
      glfwSetMouseButtonCallback (*GLFWwindowptr, GLFWMouseButtonFunction);
      glfwSetCursorPosCallback (*GLFWwindowptr, GLFWMouseCursorMoved);
      glfwSetWindowSizeCallback (*GLFWwindowptr, Resize);
    }

    void Init_Glew()
    {
      glewExperimental = GL_TRUE;
      glewInit();
      std::cout << "OpenGl Version: " << Console::green << glGetString(GL_VERSION) << Console::gray << std::endl;
      glEnable (GL_DEPTH_TEST);
      glEnable (GL_BLEND);
      glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
  }

  WindowSystem::WindowSystem(const char* WindowTitle, int ClientWidth, int ClientHeight)
  {
    WINDOWSYSTEM = this;
    WindowNameSpace::Create_Context(&window);
    WindowNameSpace::Init_Glew();
  }


  bool WindowSystem::Initialize ()
  {
    std::cout << GetName () << " initialized\n";
    return true;
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
    glfwPollEvents ();
  }

  void WindowSystem::GraphicsUpdate (const double dt)
  {
  }


  unsigned WindowSystem::Get_Width ()
  {
    return WindowWidth;
  }

  unsigned WindowSystem::Get_Height ()
  {
    return WindowHeight;
  }

  void WindowSystem::Set_W_H (unsigned w, unsigned h)
  {
    WindowWidth = w;
    WindowHeight = h;
  }

}
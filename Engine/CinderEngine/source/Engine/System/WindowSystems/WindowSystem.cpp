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
#include "Sprite.h"
#include "Core.h"

Sprite* sprite;
ResourceManager resourceManager;

namespace Framework
{
  //! Global pointer to  the windows system.
  WindowSystem* WINDOWSYSTEM = NULL;

  namespace WindowNameSpace
  {
    void GLFWMessageHandler (GLFWwindow* window, int key, int scanCode, int state, int mod)
    {

    }

    void Create_Context(GLFWwindow** GLFWwindowptr)
    {
      glfwInit ();

      glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
      glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

      glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);

      *GLFWwindowptr = glfwCreateWindow (400, 400, "OpenGL", nullptr, nullptr); // Windowed
      //GLFWwindow* window = glfwCreateWindow (800, 600, "OpenGL", glfwGetPrimaryMonitor (), nullptr);
      glfwMakeContextCurrent (*GLFWwindowptr);
      glfwSetKeyCallback(*GLFWwindowptr, GLFWMessageHandler);
    }

    void Init_Glew()
    {
      glewExperimental = GL_TRUE;
      glewInit();
      std::cout << "OpenGl Version: " << Console::green << glGetString(GL_VERSION) << Console::gray << std::endl;
    }

  }

  WindowSystem::WindowSystem(const char* WindowTitle, int ClientWidth, int ClientHeight)
  {
    WindowNameSpace::Create_Context(&window);
    WindowNameSpace::Init_Glew();
  }


  bool WindowSystem::Initialize ()
  {
    // LOAD RESOURCES (TEXTURES, SHADERS)
    resourceManager.Load_Resources ();

    sprite = new Sprite (resourceManager.Get_Shader ("VertexShader.glsl"), resourceManager.Get_Texture ("ScarlettJohansson.jpg"));
    sprite->Specify_Attributes ();

    while (!glfwWindowShouldClose(window))
    {
      glfwPollEvents ();
      sprite->Draw ();
      glfwSwapBuffers (window);
    }

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
    sprite->Draw ();
  }

  void WindowSystem::GraphicsUpdate(const double dt)
  {

  }


  ////////////////////////////////////////////////////////////
  



}
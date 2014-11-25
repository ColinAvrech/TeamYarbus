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

#include "Windowsystem.h"
#include "GraphicsCommon.h"
#include "Resources.h"
#include "ResourceManager.h"
#include "Core.h"
#include "AudioSystem.h"
#include "SceneManager.h"
#include "ComponentInclude.h"
#include "EventSystem.h"
#include "Thermodynamics.h"
#include "Fluid_Engine.h"
#include "Smoke_Grid.h"
#include "IGraphicsObject.h"
#include "CinderEngine_UI.h"
#include "Pipeline.h"
#include "WindowFocusEvent.h"

namespace Framework
{
  //! Global pointer to  the windows system.
  WindowSystem* WINDOWSYSTEM = NULL;

  ZilchDefineType(WindowSystem, CinderZilch)
  {
	  type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	  ZilchBindMethodAs(ZGet_Mouse_Position, "GetMousePosition");
	  ZilchBindMethodAs(ZGet_Normalized_Mouse_Position, "GetNormalMousePosition");
	  ZilchBindFieldGet(IsMouseDown);

  }

  namespace WindowNameSpace
  {
    void GLFWResize (GLFWwindow* window, const int w, const int h)
    {
      WINDOWSYSTEM->Set_W_H (w, (int)(w / (1.6f / 0.9f)));
      glfwSetWindowSize (window, WINDOWSYSTEM->Get_Width (), WINDOWSYSTEM->Get_Height ());
    }

    void GLFWFrameBufferResize (GLFWwindow* _window, const int w, const int h)
    {
    }

    void GLFWWindowFocus(GLFWwindow* window, const int focus)
    {
      WindowFocusEvent e;
      e.InFocus = focus;
      EVENTSYSTEM->TriggerEvent(Events::WINDOWFOCUSEVENT, e);
    }


    /*Triggers a Key event if there are any listeners*/
    void TriggerKeyEvent (const string eventname, const int& key, const int& scanCode, const int& state, const int& mod)
    {
      KeyEvent triggered_key_event;
      SetupKeyEvent (&triggered_key_event, key, scanCode, state, mod);
      EVENTSYSTEM->TriggerEvent (eventname, triggered_key_event);
    }

    /*Sets up the Key event before dispatching it*/
    void SetupKeyEvent (KeyEvent* triggered_key_event, const int& key, const int& scanCode, const int& state, const int& mod)
    {
      //Key value as dictated by GLFW
      triggered_key_event->KeyValue = key;

      // Set the Key's State
      if (state == GLFW_RELEASE)
      {
        triggered_key_event->KeyDown = false;
        triggered_key_event->KeyRepeat = false;
      }
      else if (state == GLFW_PRESS)
      {
        triggered_key_event->KeyDown = true;
        triggered_key_event->KeyRepeat = false;
      }
      else if (state == GLFW_REPEAT)
      {
        triggered_key_event->KeyDown = true;
        triggered_key_event->KeyRepeat = true;
      }

      // Active Key Modifiers
      if (mod & GLFW_MOD_SHIFT)
      {
        triggered_key_event->SHIFTPressed = true;
      }
      else{
        triggered_key_event->SHIFTPressed = false;
      }

      if (mod & GLFW_MOD_CONTROL)
      {
        triggered_key_event->CTRLPressed = true;
      }
      else{
        triggered_key_event->CTRLPressed = false;
      }

      if (mod & GLFW_MOD_ALT)
      {
        triggered_key_event->ALTPressed = true;
      }
      else{
        triggered_key_event->ALTPressed = false;
      }
    }

    void GLFWMessageHandler (GLFWwindow* window, const int key, const int scanCode, const int state, const int mod)
    {
      //A Key has been pressed
      TriggerKeyEvent (Events::KEY_ANY, key, scanCode, state, mod);

      switch (key)
      {
        //Numbers
      case GLFW_KEY_0:
        TriggerKeyEvent (Events::KEY_0, key, scanCode, state, mod);
        break;
      case GLFW_KEY_1:
        TriggerKeyEvent (Events::KEY_1, key, scanCode, state, mod);
        break;
      case GLFW_KEY_2:
        TriggerKeyEvent (Events::KEY_2, key, scanCode, state, mod);
        break;
      case GLFW_KEY_3:
        TriggerKeyEvent (Events::KEY_3, key, scanCode, state, mod);
        break;
      case GLFW_KEY_4:
        TriggerKeyEvent (Events::KEY_4, key, scanCode, state, mod);
        break;
      case GLFW_KEY_5:
        TriggerKeyEvent (Events::KEY_5, key, scanCode, state, mod);
        break;
      case GLFW_KEY_6:
        TriggerKeyEvent (Events::KEY_6, key, scanCode, state, mod);
        break;
      case GLFW_KEY_7:
        TriggerKeyEvent (Events::KEY_7, key, scanCode, state, mod);
        break;
      case GLFW_KEY_8:
        TriggerKeyEvent (Events::KEY_8, key, scanCode, state, mod);
        break;
      case GLFW_KEY_9:
        TriggerKeyEvent (Events::KEY_9, key, scanCode, state, mod);
        break;
        //Letters
      case GLFW_KEY_A:
        TriggerKeyEvent (Events::KEY_A, key, scanCode, state, mod);
        break;
      case GLFW_KEY_B:
        TriggerKeyEvent (Events::KEY_B, key, scanCode, state, mod);
        break;
      case GLFW_KEY_C:
        TriggerKeyEvent (Events::KEY_C, key, scanCode, state, mod);
        break;
      case GLFW_KEY_D:
        TriggerKeyEvent (Events::KEY_D, key, scanCode, state, mod);
        break;
      case GLFW_KEY_E:
        TriggerKeyEvent (Events::KEY_E, key, scanCode, state, mod);
        break;
      case GLFW_KEY_F:
        TriggerKeyEvent (Events::KEY_F, key, scanCode, state, mod);
        break;
      case GLFW_KEY_G:
        TriggerKeyEvent (Events::KEY_G, key, scanCode, state, mod);
        break;
      case GLFW_KEY_H:
        TriggerKeyEvent (Events::KEY_H, key, scanCode, state, mod);
        break;
      case GLFW_KEY_I:
        TriggerKeyEvent (Events::KEY_I, key, scanCode, state, mod);
        break;
      case GLFW_KEY_J:
        TriggerKeyEvent (Events::KEY_J, key, scanCode, state, mod);
        break;
      case GLFW_KEY_K:
        TriggerKeyEvent (Events::KEY_K, key, scanCode, state, mod);
        break;
      case GLFW_KEY_L:
        TriggerKeyEvent (Events::KEY_L, key, scanCode, state, mod);
        break;
      case GLFW_KEY_M:
        TriggerKeyEvent (Events::KEY_M, key, scanCode, state, mod);
        break;
      case GLFW_KEY_N:
        TriggerKeyEvent (Events::KEY_N, key, scanCode, state, mod);
        break;
      case GLFW_KEY_O:
        TriggerKeyEvent (Events::KEY_O, key, scanCode, state, mod);
        break;
      case GLFW_KEY_P:
        TriggerKeyEvent (Events::KEY_P, key, scanCode, state, mod);
        break;
      case GLFW_KEY_Q:
        TriggerKeyEvent (Events::KEY_Q, key, scanCode, state, mod);
        break;
      case GLFW_KEY_R:
        TriggerKeyEvent (Events::KEY_R, key, scanCode, state, mod);
        break;
      case GLFW_KEY_S:
        TriggerKeyEvent (Events::KEY_S, key, scanCode, state, mod);
        break;
      case GLFW_KEY_T:
        TriggerKeyEvent (Events::KEY_T, key, scanCode, state, mod);
        break;
      case GLFW_KEY_U:
        TriggerKeyEvent (Events::KEY_U, key, scanCode, state, mod);
        break;
      case GLFW_KEY_V:
        TriggerKeyEvent (Events::KEY_V, key, scanCode, state, mod);
        break;
      case GLFW_KEY_W:
        TriggerKeyEvent (Events::KEY_W, key, scanCode, state, mod);
        break;
      case GLFW_KEY_X:
        TriggerKeyEvent (Events::KEY_X, key, scanCode, state, mod);
        break;
      case GLFW_KEY_Y:
        TriggerKeyEvent (Events::KEY_Y, key, scanCode, state, mod);
        break;
      case GLFW_KEY_Z:
        TriggerKeyEvent (Events::KEY_Z, key, scanCode, state, mod);
        break;

        // Arrows
      case GLFW_KEY_UP:
        TriggerKeyEvent (Events::KEY_UP, key, scanCode, state, mod);
        break;
      case GLFW_KEY_DOWN:
        TriggerKeyEvent (Events::KEY_DOWN, key, scanCode, state, mod);
        break;
      case GLFW_KEY_LEFT:
        TriggerKeyEvent (Events::KEY_LEFT, key, scanCode, state, mod);
        break;
      case GLFW_KEY_RIGHT:
        TriggerKeyEvent (Events::KEY_RIGHT, key, scanCode, state, mod);
        break;

        // Misc
      case GLFW_KEY_SPACE:
        TriggerKeyEvent (Events::KEY_SPACE, key, scanCode, state, mod);
        break;
      case GLFW_KEY_BACKSPACE:
        TriggerKeyEvent (Events::KEY_BACKSPACE, key, scanCode, state, mod);
        break;
      case GLFW_KEY_ESCAPE:
        TriggerKeyEvent (Events::KEY_ESCAPE, key, scanCode, state, mod);
        break;
      case GLFW_KEY_TAB:
        TriggerKeyEvent (Events::KEY_TAB, key, scanCode, state, mod);
        break;
      case GLFW_KEY_GRAVE_ACCENT:
        TriggerKeyEvent (Events::KEY_GRAVEACCENT, key, scanCode, state, mod);
        break;
      case GLFW_KEY_ENTER:
        TriggerKeyEvent (Events::KEY_ENTER, key, scanCode, state, mod);
        break;

      default:
        break;
      }
    }


    void GLFWMouseButtonFunction (GLFWwindow *, const int button, const int action, const int mod)
    {
		  WINDOWSYSTEM->IsMouseDown = action;
    }
    void GLFWMouseCursorMoved (GLFWwindow* window, const double xPos, const double yPos)
    {
      WINDOWSYSTEM->cursorPosition.x = xPos;
      WINDOWSYSTEM->cursorPosition.y = yPos;
    }

    void GLFWWindowClosed (GLFWwindow* window)
    {
      CORE->QuitGame ();
    }

    void Create_Context(GLFWwindow** GLFWwindowptr, const int& ClientWidth, const int& ClientHeight, const bool& fullscreen)
    {
      // Init GLFW Before Using Any Functionality
      if (!glfwInit ())
      {
        throw("Could Not Create GLFW Context");
        CORE->QuitGame ();
      }


      // Window Creation
      int count = 0;
      GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
      const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

      //TODO: use this for resolution selector
      //const GLFWvidmode* modes = glfwGetVideoModes(primaryMonitor, &count);
      if (fullscreen)
      {
        WINDOWSYSTEM->Set_W_H (mode->width, mode->height);
        *GLFWwindowptr = glfwCreateWindow
          (
            WINDOWSYSTEM->Get_Width (),
            WINDOWSYSTEM->Get_Height (),
            "OpenGL",
            primaryMonitor,
            nullptr
          ); // Windowed
      }
      else
      {
        WINDOWSYSTEM->Set_W_H (ClientWidth, (int)(ClientWidth / (16.f / 9)));
        *GLFWwindowptr = glfwCreateWindow
          (
            WINDOWSYSTEM->Get_Width(),
            WINDOWSYSTEM->Get_Height(),
            "OpenGL",
            nullptr,
            nullptr
          ); // Windowed
      }

      //*GLFWwindowptr = glfwCreateWindow (800, 600, "OpenGL", glfwGetPrimaryMonitor (), nullptr);
      glfwMakeContextCurrent (*GLFWwindowptr);

      // Set Callback Functions
      glfwSetKeyCallback (*GLFWwindowptr, GLFWMessageHandler);
      glfwSetMouseButtonCallback (*GLFWwindowptr, GLFWMouseButtonFunction);
      glfwSetCursorPosCallback (*GLFWwindowptr, GLFWMouseCursorMoved);
      glfwSetWindowSizeCallback (*GLFWwindowptr, GLFWResize);
      glfwSetFramebufferSizeCallback (*GLFWwindowptr, GLFWFrameBufferResize);
      glfwSetWindowCloseCallback (*GLFWwindowptr, GLFWWindowClosed);
    }

    void Init_Glew ()
    {
      glewExperimental = GL_TRUE;
      glewInit ();
      std::cout << "OpenGl Version: " << CinderConsole::green << glGetString (GL_VERSION) << CinderConsole::gray << std::endl;
    }
  } //namespace WindowNameSpace


  WindowSystem::WindowSystem(const char* WindowTitle, const int& ClientWidth, const int& ClientHeight, const bool& fullscreen)
  {
    WINDOWSYSTEM = this;
    WindowNameSpace::Create_Context (&window, ClientWidth, ClientHeight, fullscreen);
    WindowNameSpace::Init_Glew ();
  }


  static void OnKeyPressed (KeyEvent* key)
  {
    if (key->KeyDown)
    {
      switch (key->KeyValue)
      {
      case GLFW_KEY_ESCAPE:
        CORE->QuitGame ();
        break;
      default:
        break;
      }
    }
  }

  bool WindowSystem::Initialize ()
  {
    std::cout << GetName () << " initialized\n";

    OPENGL = new Pipeline ();
    EVENTSYSTEM->gConnect (Events::KEY_ANY, &OnKeyPressed);
    return true;
  }


  WindowSystem::~WindowSystem ()
  {
    delete OPENGL;
    glfwTerminate ();
  }

  void WindowSystem::Update (const double& dt)
  {
    WindowsUpdate (dt);
    GraphicsUpdate (dt);

    //std::cout << "{ " << Camera::GetWorldMousePosition ().x << ", " << Camera::GetWorldMousePosition ().y << " }\n";
  }


  void WindowSystem::WindowsUpdate (const double& dt)
  {
    glfwPollEvents ();
  }

  void WindowSystem::GraphicsUpdate (const double& dt)
  {
    OPENGL->Update ();

    glfwSwapBuffers (window);
  }

  int WindowSystem::Get_Width ()
  {
    return WindowWidth;
  }

  int WindowSystem::Get_Height ()
  {
    return WindowHeight;
  }

  void WindowSystem::Set_W_H (const int& w, const int& h)
  {
    glViewport (0, 0, w, h);
    WindowWidth = w;
    WindowHeight = h;
  }

  glm::vec2 WindowSystem::Get_Mouse_Position ()
  {
    return glm::vec2 (cursorPosition);
  }

  Zilch::Real2 WindowSystem::ZGet_Mouse_Position()
  {
	  return Zilch::Real2(Zilch::Real(cursorPosition.x), Zilch::Real(cursorPosition.y));
  }

  glm::vec2 WindowSystem::Get_Normalized_Mouse_Position ()
  {
    glm::vec2 normPos;
    normPos.x = (float) (cursorPosition.x / (WindowWidth) -0.5f) * 2.0f;
    normPos.y = (float) ((WindowHeight - cursorPosition.y) / WindowHeight - 0.5f) * 2.0f;

    return normPos;
  }

  Zilch::Real2 WindowSystem::ZGet_Normalized_Mouse_Position()
  {
	  glm::vec2 normPos;
	  normPos.x = (float)(cursorPosition.x / (WindowWidth)-0.5f) * 2.0f;
	  normPos.y = (float)((WindowHeight - cursorPosition.y) / WindowHeight - 0.5f) * 2.0f;

	  return Zilch::Real2(Zilch::Real(normPos.x), Zilch::Real(normPos.y));
  }

}
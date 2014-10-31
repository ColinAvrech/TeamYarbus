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
#include "EventSystem.h"
#include "DebugRenderer.h"
#include "CLParticleRenderer.h"
#include "HeatMap.h"
#include "FrameBufferObject.h"
#include "Thermodynamics.h"
#include "glm/gtc/random.hpp"

namespace Framework
{
  static glm::vec3 lightPos;
  static bool active = false;
  //! Global pointer to  the windows system.
  WindowSystem* WINDOWSYSTEM = NULL;

  std::list <Sprite*> WindowSystem::spriteList;

  static VAO* vao, *vao1;
  static VBO* vbo, *vbo1;
  static EBO* ebo;
  static FBO* fbo;
  GLuint renderTexture;
  static DebugRenderer dr;
  static CLParticleRenderer clRenderer;
  static float shininess = 200;
  static HeatMap heatMap (101, 101);
  static Sound *fire;

  namespace WindowNameSpace
  {
    void Resize (GLFWwindow* window, int w, int h)
    {
      WINDOWSYSTEM->Set_W_H (w, h);
      glBindTexture (GL_TEXTURE_2D, renderTexture);
      glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, WINDOWSYSTEM->Get_Width (), WINDOWSYSTEM->Get_Height (), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
      SCENEMANAGER->Change_Size (w, h);
    }

    void FrameBufferResize (GLFWwindow* _window, int w, int h)
    {
      glViewport (0, 0, w, h);
    }

    /*Triggers a Key event if there are any listeners*/
    void TriggerKeyEvent (const std::string eventname, int key, int scanCode, int state, int mod)
    {
      KeyEvent triggered_key_event;
      SetupKeyEvent (&triggered_key_event, key, scanCode, state, mod);
      EVENTSYSTEM->TriggerEvent (eventname, triggered_key_event);
    }

    /*Sets up the Key event before dispatching it*/
    void SetupKeyEvent (KeyEvent* triggered_key_event, int key, int scanCode, int state, int mod)
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

    void  GLFWMessageHandler (GLFWwindow* window, int key, int scanCode, int state, int mod)
    {
      //A Key has been pressed
      TriggerKeyEvent (Events::KEY_ANY, key, scanCode, state, mod);

      switch (key)
      {
        //Numbers
      case GLFW_KEY_0:
        TriggerKeyEvent (Events::KEY_0, key, scanCode, state, mod);
      case GLFW_KEY_1:
        TriggerKeyEvent (Events::KEY_1, key, scanCode, state, mod);
      case GLFW_KEY_2:
        TriggerKeyEvent (Events::KEY_2, key, scanCode, state, mod);
      case GLFW_KEY_3:
        TriggerKeyEvent (Events::KEY_3, key, scanCode, state, mod);
      case GLFW_KEY_4:
        TriggerKeyEvent (Events::KEY_4, key, scanCode, state, mod);
      case GLFW_KEY_5:
        TriggerKeyEvent (Events::KEY_5, key, scanCode, state, mod);
      case GLFW_KEY_6:
        TriggerKeyEvent (Events::KEY_6, key, scanCode, state, mod);
      case GLFW_KEY_7:
        TriggerKeyEvent (Events::KEY_7, key, scanCode, state, mod);
      case GLFW_KEY_8:
        TriggerKeyEvent (Events::KEY_8, key, scanCode, state, mod);
      case GLFW_KEY_9:
        TriggerKeyEvent (Events::KEY_9, key, scanCode, state, mod);

        //Letters
      case GLFW_KEY_A:
        TriggerKeyEvent (Events::KEY_A, key, scanCode, state, mod);
      case GLFW_KEY_B:
        TriggerKeyEvent (Events::KEY_B, key, scanCode, state, mod);
      case GLFW_KEY_C:
        TriggerKeyEvent (Events::KEY_C, key, scanCode, state, mod);
      case GLFW_KEY_D:
        TriggerKeyEvent (Events::KEY_D, key, scanCode, state, mod);
      case GLFW_KEY_E:
        TriggerKeyEvent (Events::KEY_E, key, scanCode, state, mod);
      case GLFW_KEY_F:
        TriggerKeyEvent (Events::KEY_F, key, scanCode, state, mod);
      case GLFW_KEY_G:
        TriggerKeyEvent (Events::KEY_G, key, scanCode, state, mod);
      case GLFW_KEY_H:
        TriggerKeyEvent (Events::KEY_H, key, scanCode, state, mod);
      case GLFW_KEY_I:
        TriggerKeyEvent (Events::KEY_I, key, scanCode, state, mod);
      case GLFW_KEY_J:
        TriggerKeyEvent (Events::KEY_J, key, scanCode, state, mod);
      case GLFW_KEY_K:
        TriggerKeyEvent (Events::KEY_K, key, scanCode, state, mod);
      case GLFW_KEY_L:
        TriggerKeyEvent (Events::KEY_L, key, scanCode, state, mod);
      case GLFW_KEY_M:
        TriggerKeyEvent (Events::KEY_M, key, scanCode, state, mod);
      case GLFW_KEY_N:
        TriggerKeyEvent (Events::KEY_N, key, scanCode, state, mod);
      case GLFW_KEY_O:
        TriggerKeyEvent (Events::KEY_O, key, scanCode, state, mod);
      case GLFW_KEY_P:
        TriggerKeyEvent (Events::KEY_P, key, scanCode, state, mod);
      case GLFW_KEY_Q:
        TriggerKeyEvent (Events::KEY_Q, key, scanCode, state, mod);
      case GLFW_KEY_R:
        TriggerKeyEvent (Events::KEY_R, key, scanCode, state, mod);
      case GLFW_KEY_S:
        TriggerKeyEvent (Events::KEY_S, key, scanCode, state, mod);
      case GLFW_KEY_T:
        TriggerKeyEvent (Events::KEY_T, key, scanCode, state, mod);
      case GLFW_KEY_U:
        TriggerKeyEvent (Events::KEY_U, key, scanCode, state, mod);
      case GLFW_KEY_V:
        TriggerKeyEvent (Events::KEY_V, key, scanCode, state, mod);
      case GLFW_KEY_W:
        TriggerKeyEvent (Events::KEY_W, key, scanCode, state, mod);
      case GLFW_KEY_X:
        TriggerKeyEvent (Events::KEY_X, key, scanCode, state, mod);
      case GLFW_KEY_Y:
        TriggerKeyEvent (Events::KEY_Y, key, scanCode, state, mod);
      case GLFW_KEY_Z:
        TriggerKeyEvent (Events::KEY_Z, key, scanCode, state, mod);

        // Arrows
      case GLFW_KEY_UP:
        TriggerKeyEvent (Events::KEY_UP, key, scanCode, state, mod);
      case GLFW_KEY_DOWN:
        TriggerKeyEvent (Events::KEY_DOWN, key, scanCode, state, mod);
      case GLFW_KEY_LEFT:
        TriggerKeyEvent (Events::KEY_LEFT, key, scanCode, state, mod);
      case GLFW_KEY_RIGHT:
        TriggerKeyEvent (Events::KEY_RIGHT, key, scanCode, state, mod);
        break;

        // Misc
      case GLFW_KEY_SPACE:
        if (state == GLFW_PRESS)
          active = !active;
        TriggerKeyEvent (Events::KEY_SPACE, key, scanCode, state, mod);
        break;
      case GLFW_KEY_BACKSPACE:
        TriggerKeyEvent (Events::KEY_BACKSPACE, key, scanCode, state, mod);
      case GLFW_KEY_ESCAPE:
        TriggerKeyEvent (Events::KEY_ESCAPE, key, scanCode, state, mod);
      case GLFW_KEY_TAB:
        TriggerKeyEvent (Events::KEY_TAB, key, scanCode, state, mod);
      case GLFW_KEY_GRAVE_ACCENT:
        TriggerKeyEvent (Events::KEY_GRAVEACCENT, key, scanCode, state, mod);
      case GLFW_KEY_ENTER:
        TriggerKeyEvent (Events::KEY_ENTER, key, scanCode, state, mod);

      default:
        break;
      }
    }


    void GLFWMouseButtonFunction (GLFWwindow *, int button, int action, int mod)
    {
    }
    void GLFWMouseCursorMoved (GLFWwindow* window, double xPos, double yPos)
    {
    }

    void GLFWWindowClosed (GLFWwindow* window)
    {
      CORE->QuitGame ();
    }

    void Create_Context (GLFWwindow** GLFWwindowptr)
    {
      // Init GLFW Before Using Any Functionality
      glfwInit ();
      // Properties
      //Request an OpenGL 4.3 core context
      glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      glfwWindowHint (GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
      glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      WINDOWSYSTEM->Set_W_H (1280, 720);
      // Window Creation
      *GLFWwindowptr = glfwCreateWindow (WINDOWSYSTEM->Get_Width (), WINDOWSYSTEM->Get_Height (), "OpenGL", nullptr, nullptr); // Windowed

      //*GLFWwindowptr = glfwCreateWindow (800, 600, "OpenGL", glfwGetPrimaryMonitor (), nullptr);
      glfwMakeContextCurrent (*GLFWwindowptr);

      // Set Callback Functions
      glfwSetKeyCallback (*GLFWwindowptr, GLFWMessageHandler);
      glfwSetMouseButtonCallback (*GLFWwindowptr, GLFWMouseButtonFunction);
      glfwSetCursorPosCallback (*GLFWwindowptr, GLFWMouseCursorMoved);
      glfwSetWindowSizeCallback (*GLFWwindowptr, Resize);
      glfwSetFramebufferSizeCallback (*GLFWwindowptr, FrameBufferResize);
      glfwSetWindowCloseCallback (*GLFWwindowptr, GLFWWindowClosed);
    }

    void Init_Glew ()
    {
      glewExperimental = GL_TRUE;
      glewInit ();
      std::cout << "OpenGl Version: " << Console::green << glGetString (GL_VERSION) << Console::gray << std::endl;
      //glEnable (GL_DEPTH_TEST);
      //glEnable (GL_BLEND);
      //glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
  } //namespace WindowNameSpace

  static Shader* shader;
  static float lighting = 150.0f;

  WindowSystem::WindowSystem (const char* WindowTitle, int ClientWidth, int ClientHeight)
  {
    WINDOWSYSTEM = this;
    WindowNameSpace::Create_Context (&window);
    WindowNameSpace::Init_Glew ();
  }

  static Shader* sceneShader;
  glm::vec3 lights [5];
  bool WindowSystem::Initialize ()
  {
    std::cout << GetName () << " initialized\n";

    fire = Resources::RS->Get_Sound("FireA.ogg");
    fire->Play();
    fire->LowPassFilter();
    fire->HighPassFilter();
    fire->SetHPF(200, 1);

    GLfloat vertices [] =
    {
      -1.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, -1.0f, 1.0f, 0.0f,

      1.0f, -1.0f, 1.0f, 0.0f,
      -1.0f, -1.0f, 0.0f, 0.0f,
      -1.0f, 1.0f, 0.0f, 1.0f
    };

    vao1 = new VAO ();
    vbo1 = new VBO (sizeof(vertices), vertices);
    fbo = new FBO ();
    glGenTextures (1, &renderTexture);
    glBindTexture (GL_TEXTURE_2D, renderTexture);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
    //void specifyScreenVertexAttributes (GLuint shaderProgram)
    //{
    //  GLint posAttrib = glGetAttribLocation (shaderProgram, "position");
    //  glEnableVertexAttribArray (posAttrib);
    //  glVertexAttribPointer (posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    //
    //  GLint texAttrib = glGetAttribLocation (shaderProgram, "texcoord");
    //  glEnableVertexAttribArray (texAttrib);
    //  glVertexAttribPointer (texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
    //}
    sceneShader = Resources::RS->Get_Shader ("Passthrough");
    sceneShader->Use ();
    GLint posAttrib = sceneShader->attribLocation ("position");
    sceneShader->enableVertexAttribArray (posAttrib);
    sceneShader->vertexAttribPtr (posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof GLfloat, 0);
    GLint texAttrib = sceneShader->attribLocation ("texcoord");
    sceneShader->enableVertexAttribArray (texAttrib);
    sceneShader->vertexAttribPtr (texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof GLfloat, 2 * sizeof GLfloat);

    clRenderer.GenerateTextures ();
    clRenderer.GenerateBuffers ();
    clRenderer.GenerateShaders ();

    heatMap.Initialize ();
    //dr.Initialize ();
    ShapeData data = ShapeGenerator::Generate_Quad ();
    vao = new VAO ();
    vbo = new VBO (data.vbo_size (), data.vertices);
    ebo = new EBO (data.ebo_size (), data.indices);
    data.Clean ();

    shader = Resources::RS->Get_Shader ("Lighting");

    lights [0] = { -0.7f, 0.7f, 0 };
    lights [1] = { 0.7f, 0.7f, 0 };
    lights [2] = { -0.7f, -0.7f, 0 };
    lights [3] = { 0.7f, -0.7f, 0 };
    lights [4] = { 0, 0, 0 };

    return true;
  }

  WindowSystem::~WindowSystem ()
  {
    delete vao, vbo, ebo;
    glfwTerminate ();
  }

  void WindowSystem::Update (const double dt)
  {
    WindowsUpdate (dt);
    GraphicsUpdate (dt);
  }
  
  float lpf = 6000.0f;
  static float micdata() { return AUDIOSYSTEM->input.peaklevel[0]; }

  void WindowSystem::WindowsUpdate (const double dt)
  {
    if (AUDIOSYSTEM->input.peaklevel[0] > 0.05f)
    {
      if (lpf < 22000)
      {
        lpf += micdata() * 200.0f;
      }
    }
    else
    {
      if (lpf > 6000)
      {
        lpf -= 150.0f;
      }
    }
    fire->SetLPF(lpf, 1);
    glfwSwapBuffers (window);
    glfwPollEvents ();
  }  

  void WindowSystem::GraphicsUpdate (const double dt)
  {
    fbo->unBind ();
    //glViewport (0, 0, WindowWidth, WindowHeight);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glfwSwapBuffers (window);
    clRenderer.Render ();

    if (active)
    {
      heatMap.Update (dt);
      heatMap.Draw ();
    }

    vao->bindVAO ();
    //shader->Use();
    Physics::THERMODYNAMICS->SetCellTemperature (0.0f, 0.4f,
      Physics::THERMODYNAMICS->GetCellTemperature (0.0f, 0.4f) + 100000.f * micdata (),
      0.016);
    for (auto i : spriteList)
    {
      i->gameObject->Transform->UpdateMatrices ();
      if (micdata () > 0.01f)
      {
        if (lighting > 5)
        {
          lighting -= micdata () * 0.5f;
        }
      }
      else
      {
        if (lighting < 250)
        {
          lighting += 0.16f;
        }
      }
      //shader->uni1f("shininess", lighting);
      i->Draw ();
    }
    vao->unbindVAO ();

    //fbo->unBind ();
    ////glViewport (0, 0, WindowWidth, WindowHeight);
    //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //vao1->bindVAO ();
    //sceneShader->Use ();
    //glBindTexture (GL_TEXTURE_2D, renderTexture);
    //sceneShader->enableVertexAttribArray (sceneShader->attribLocation ("position"));
    //sceneShader->enableVertexAttribArray (sceneShader->attribLocation ("texcoord"));
    ////for (int i = 0; i < 5; ++i)
    ////{
    ////  lights [i] = glm::linearRand (glm::vec3 (-2, -2, 0), glm::vec3 (2, 2, 0));
    ////}
    //sceneShader->uni3f ("lightPos", clRenderer.destPosX * 2, clRenderer.destPosY * 2, 0.0);
    //sceneShader->uni3fv ("lights", glm::value_ptr (lights [0]), 5);
    //sceneShader->uni1i ("image", 0);

    //glDrawArrays (GL_TRIANGLES, 0, 6);
    //clRenderer.Render ();
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // To use Debug Draw:
    // Create a DebugRenderer
    // Initialize
    // Use Draw Function
    // Can draw points
    // Can draw lines
    // Can draw polygon with n number of shapes. max is 140
    //////////////////////////////////////////////////////////////////////////
    //CircleCollider c (NULL);
    //dr.Draw (&c);
    //dr.Draw ((LineCollider*)nullptr);
    //dr.Draw (nullptr, 3);
    //dr.Draw (nullptr, 5);
    //dr.Draw ((PointCollider*)nullptr);

    //////////////////////////////////////////////////////////////////////////
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
    glViewport (0, 0, w, h);
    WindowWidth = w;
    WindowHeight = h;
  }

}
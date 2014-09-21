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
#include "ComponentInclude.h"

namespace Framework
{
  Camera camera (NULL, true);
  Transform light (NULL);
  Sprite sprite (NULL);
  Sprite sprite1 (NULL);
  float camScrollSpeed = 0.05f;
  float shininess = 100.0f;
  bool isPressed = false;
  VAO* vao;
  VBO* vbo;
  EBO* ebo;
  glm::vec2 texoffset (0.0f, 0.0f);
  const int samples = 4;
  int frames;
  struct SoundName
  {
    Sound *test1;
    Sound *test2;
  }SoundName;

  //! Global pointer to  the windows system.
  WindowSystem* WINDOWSYSTEM = NULL;

  namespace WindowNameSpace
  {
    void GLFWMessageHandler (GLFWwindow* window, int key, int scanCode, int state, int mod)
    {
      //if (state == GLFW_REPEAT)
      switch (key)
      {
      case GLFW_KEY_W:
        light.Translate (0, 0.01f, 0);
        //SoundName.test2->SetPause(true);
        break;
      case GLFW_KEY_S:
        light.Translate (0, -0.01f, 0);
        //SoundName.test2->SetPause(false);
        break;
      case GLFW_KEY_D:
        light.Translate (0.01f, 0, 0);
        break;
      case GLFW_KEY_A:
        light.Translate (-0.01f, 0, 0);
        break;
      case GLFW_KEY_Q:
        shininess += 0.5f;
        break;
      case GLFW_KEY_E:
        shininess -= 0.5f;
        break;
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose (window, GL_TRUE);
        break;
      case GLFW_KEY_LEFT:
        //////////////////////////////////////////////////////////////////////////
        // Only For Testing
        sprite.modelMatrix = glm::translate (sprite.modelMatrix, glm::vec3 (-0.1f, 0.0f, 0.0f));
        //////////////////////////////////////////////////////////////////////////
        //camera.UpdatePosition (glm::vec3 (-camScrollSpeed, 0.0f, 0.0f));
        break;
      case GLFW_KEY_RIGHT:
        //////////////////////////////////////////////////////////////////////////
        // Only Fore Testing
        sprite.modelMatrix = glm::translate (sprite.modelMatrix, glm::vec3 (0.1f, 0.0f, 0.0f));
        //////////////////////////////////////////////////////////////////////////
        //camera.UpdatePosition (glm::vec3 (camScrollSpeed, 0.0f, 0.0f));
        break;
      case GLFW_KEY_UP:
        camera.UpdatePosition (glm::vec3 (0.0f, camScrollSpeed, 0.0f));
        break;
      case GLFW_KEY_DOWN:
        camera.UpdatePosition (glm::vec3 (0.0f, -camScrollSpeed, 0.0f));
        break;
      case GLFW_KEY_Z:
        camera.Zoom (0.01f);
        break;
      case GLFW_KEY_X:
        camera.Zoom (-0.01f);
        break;
      default:
        break;

        
      }
    }

    void GLFWMouseButtonFunction (GLFWwindow * window, int button, int action, int mod)
    {
      if (action == GLFW_PRESS)
      switch (button)
      {
      case GLFW_MOUSE_BUTTON_1:
        isPressed = true;
        std::cout << "LMB Pressed\n";
        break;
      default:
        break;
      }
      else
      {
        isPressed = false;
      }
    }

    void GLFWMouseCursorMoved (GLFWwindow* window, double xPos, double yPos)
    {
      if (isPressed)
      {
        camera.MouseUpdate (glm::vec2 (float (xPos), float (yPos)));
      }
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
      glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);

      // Window Creation
      *GLFWwindowptr = glfwCreateWindow (1920, 1920, "OpenGL", nullptr, nullptr); // Windowed
      //*GLFWwindowptr = glfwCreateWindow (800, 600, "OpenGL", glfwGetPrimaryMonitor (), nullptr);
      glfwMakeContextCurrent (*GLFWwindowptr);

      // Set Callback Functions
      glfwSetKeyCallback(*GLFWwindowptr, GLFWMessageHandler);
      glfwSetMouseButtonCallback (*GLFWwindowptr, GLFWMouseButtonFunction);
      glfwSetCursorPosCallback (*GLFWwindowptr, GLFWMouseCursorMoved);
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
    WindowNameSpace::Create_Context(&window);
    WindowNameSpace::Init_Glew();
  }


  bool WindowSystem::Initialize ()
  {
    light.Translate (0, 0.36f, 0);
    // Generate a Quad for Us
    ShapeData quad = ShapeGenerator::Generate_Quad ();

    vao = new VAO ();
    vbo = new VBO (quad.vbo_size(), quad.vertices);
    ebo = new EBO (quad.ebo_size(), quad.indices);

    // Free Allocated Memory
    quad.Clean ();

    // Create Sprite
    sprite.Create
      (
      ResourceManager::RESOURCE_MANAGER->Get_Shader ("LightingAnimation"),
      ResourceManager::RESOURCE_MANAGER->Get_SpriteSheet ("Logo.png")
      );
    sprite1.Create
      (
      ResourceManager::RESOURCE_MANAGER->Get_Shader ("Default"),
      ResourceManager::RESOURCE_MANAGER->Get_Texture ("TeamName.png")
      );

    sprite.modelMatrix = glm::translate (glm::vec3 (0.0f, 0.0f, -1.0f));
    sprite.modelMatrix = glm::scale (sprite.modelMatrix, glm::vec3 (1.5f, 1.5f, 1.0f));

    sprite1.modelMatrix = glm::translate (glm::vec3 (0, -1, -1));
    sprite1.modelMatrix = glm::scale (sprite1.modelMatrix, glm::vec3 (0.5f, 0.5f / sprite1.Get_Texture()->Get_Aspect_Ratio(), 1.0f));

    //SoundName.test1 = ResourceManager::RESOURCE_MANAGER->Get_Sound("music2.mp3");
    //SoundName.test1->Play();
    //
    //SoundName.test2 = ResourceManager::RESOURCE_MANAGER->Get_Sound("music.mp3");
    //SoundName.test2->Play();

    return true;
  }

  WindowSystem::~WindowSystem()
  {
    delete vao;
    delete vbo;
    delete ebo;
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
    glClearColor (0, 0, 0, 0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    light.UpdateMatrices (0);

    if (shininess >= 4.0f)
      shininess -= 0.5f;


    // Update Uniform Variables
    glUniform3fv (glGetUniformLocation (sprite.Get_Shader ()->Get_ID (), "lightPos"), 1, glm::value_ptr(light.GetPosition()));
    glUniform3f (glGetUniformLocation (sprite.Get_Shader ()->Get_ID (), "mambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f (glGetUniformLocation (sprite.Get_Shader ()->Get_ID (), "mdiffuse"), 0.6f, 0.6f, 0.6f);
    glUniform3f (glGetUniformLocation (sprite.Get_Shader ()->Get_ID (), "mspecular"), 1.0f, 1.0f, 1.0f);

    glUniform3f (glGetUniformLocation (sprite.Get_Shader ()->Get_ID (), "lambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f (glGetUniformLocation (sprite.Get_Shader ()->Get_ID (), "ldiffuse"), 0.6f, 0.6f, 0.6f);
    glUniform3f (glGetUniformLocation (sprite.Get_Shader ()->Get_ID (), "lspecular"), 1.0f, 1.0f, 1.0f);
    glUniform1f (glGetUniformLocation (sprite.Get_Shader ()->Get_ID (), "shininess"), shininess);


    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#pragma region Instance Drawing
    //////////////////////////////////////////////////////////////////////////
    // Instancing
    // Very Basic and Wrong
    // Will Be Replaced By glDrawElementsInstanced after some systems are ready
    // Top Left
    //sprite.modelMatrix = glm::translate (glm::vec3 (-1.5f, 1.5f, -1.0f));
    //sprite.Draw ();
    //// Center Left
    //sprite.modelMatrix = glm::translate (glm::vec3 (-1.5f, 0.0f, -1.0f));
    //sprite.Draw ();
    //// Bottom Left
    //sprite.modelMatrix = glm::translate (glm::vec3 (-1.5f, -1.5f, -1.0f));
    //sprite.Draw ();
    //// Center Top
    //sprite.modelMatrix = glm::translate (glm::vec3 (0.0f, 1.5f, -1.0f));
    //sprite.Draw ();
    //// Center
    //sprite.modelMatrix = glm::translate (glm::vec3 (0.0f, 0.0f, -1.0f));
    //sprite.Draw ();
    //// Center Bottom
    //sprite.modelMatrix = glm::translate (glm::vec3 (0.0f, -1.5f, -1.0f));
    //sprite.Draw ();
    //// Top Right
    //sprite.modelMatrix = glm::translate (glm::vec3 (1.5f, 1.5f, -1.0f));
    //sprite.Draw ();
    //// Center Right
    //sprite.modelMatrix = glm::translate (glm::vec3 (1.5f, 0.0f, -1.0f));
    //sprite.Draw ();
    //// Bottom Right
    //sprite.modelMatrix = glm::translate (glm::vec3 (1.5f, -1.5f, -1.0f));
    //sprite.Draw ();
    //////////////////////////////////////////////////////////////////////////
#pragma endregion

    glUseProgram (sprite1.Get_Shader ()->Get_ID ());
    sprite1.Draw ();

    glUseProgram (sprite.Get_Shader ()->Get_ID ());
    sprite.Draw ();

    glEnable (GL_DEPTH_TEST);
    glDisable (GL_BLEND);
  }

}
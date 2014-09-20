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
#include "VertexData.h"
#include "ShapeGenerator.h"
#include "Core.h"
#include "ComponentInclude.h"


namespace Framework
{
  Camera camera (true);
  float camScrollSpeed = 0.05f;
  Transform light(NULL);
  Sprite sprite;
  Sprite sprite1;
  ResourceManager resourceManager;
  glm::vec3 position;
  float shininess = 15.0f;
  bool isPressed = false;
  VAO* vao;
  VBO* vbo;
  EBO* ebo;

  //! Global pointer to  the windows system.
  WindowSystem* WINDOWSYSTEM = NULL;

  namespace WindowNameSpace
  {
    void GLFWMessageHandler (GLFWwindow* window, int key, int scanCode, int state, int mod)
    {
      switch (key)
      {
      case GLFW_KEY_W:
        light.Translate (0, 0.01f, 0);
        break;
      case GLFW_KEY_S:
        light.Translate (0, -0.01f, 0);
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
        camera.UpdatePosition (glm::vec3 (-camScrollSpeed, 0.0f, 0.0f));
        break;
      case GLFW_KEY_RIGHT:
        camera.UpdatePosition (glm::vec3 (camScrollSpeed, 0.0f, 0.0f));
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
      glEnable (GL_BLEND);
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
    ebo = new EBO (quad.ibo_size(), quad.indices);

    // Free Allocated Memory
    quad.Clean ();

    // Load all Resources (Textures, Shaders, Maps,...)
    resourceManager.Load_Resources ();

    // Create Sprite
    sprite.Create (resourceManager.Get_Shader ("FragmentLighting.frag")->shaderProgram, resourceManager.Get_Texture("ScarlettJohansson.jpg")->textureID);

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
    glClear (GL_COLOR_BUFFER_BIT);

    // Update Uniform Variables
    glUniform3f (glGetUniformLocation (sprite.shaderID, "lightPos"), light.GetPosition().x, light.GetPosition ().y, light.GetPosition ().z);
    glUniform3f (glGetUniformLocation (sprite.shaderID, "mambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f (glGetUniformLocation (sprite.shaderID, "mdiffuse"), 0.6f, 0.6f, 0.6f);
    glUniform3f (glGetUniformLocation (sprite.shaderID, "mspecular"), 1.0f, 1.0f, 1.0f);

    glUniform3f (glGetUniformLocation (sprite.shaderID, "lambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f (glGetUniformLocation (sprite.shaderID, "ldiffuse"), 0.6f, 0.6f, 0.6f);
    glUniform3f (glGetUniformLocation (sprite.shaderID, "lspecular"), 1.0f, 1.0f, 1.0f);
    glUniform1f (glGetUniformLocation (sprite.shaderID, "shininess"), shininess);

    sprite.Draw ();

  }

}
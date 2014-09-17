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


Sprite sprite;
Sprite sprite1;
ResourceManager resourceManager;

namespace Framework
{
  //! Global pointer to  the windows system.
  WindowSystem* WINDOWSYSTEM = NULL;

  namespace WindowNameSpace
  {
    void GLFWMessageHandler (GLFWwindow* window, int key, int scanCode, int state, int mod)
    {
      switch (key)
      {
      case GLFW_KEY_W:
        break;
      default:
        break;
      }
    }

    void Create_Context(GLFWwindow** GLFWwindowptr)
    {
      glfwInit ();

      glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
      glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

      glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);

      *GLFWwindowptr = glfwCreateWindow (1280, 1280, "OpenGL", nullptr, nullptr); // Windowed
      //GLFWwindow* window = glfwCreateWindow (800, 600, "OpenGL", glfwGetPrimaryMonitor (), nullptr);
      glfwMakeContextCurrent (*GLFWwindowptr);
      glfwSetKeyCallback(*GLFWwindowptr, GLFWMessageHandler);
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
    //////////////////////////////////////////////////////////////////////////
    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers (1, &vbo);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    GLfloat vertices [] =
    {
      //  Position   Color             Texcoords
      -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Top-left
      0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Top-right
      0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
      -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
    };
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // Create an element array
    GLuint ebo;
    glGenBuffers (1, &ebo);

    GLuint elements [] = {
      0, 1, 2,
      2, 3, 0
    };
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
     //LOAD RESOURCES (TEXTURES, SHADERS)
    resourceManager.Load_Resources ();

    sprite.Create (resourceManager.Get_Shader ("VertexShader.glsl")->shaderProgram, resourceManager.Get_Texture("ScarlettJohansson.jpg")->textureID);
    sprite1.Create (resourceManager.Get_Shader ("VertexShader1.glsl")->shaderProgram, resourceManager.Get_Texture ("Default")->textureID);

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
    //sprite->Draw ();
  }

  void WindowSystem::GraphicsUpdate (const double dt)
  {
    glClearColor (0, 0, 0, 0);
    glClear (GL_COLOR_BUFFER_BIT);
    //glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    sprite.Draw ();
    sprite1.Draw ();
  }

}

#pragma region BACKUP
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Create and compile the vertex shader
//GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
//glShaderSource (vertexShader, 1, &vertexSource, NULL);
//glCompileShader (vertexShader);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Create and compile the fragment shader
//GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
//glShaderSource (fragmentShader, 1, &fragmentSource, NULL);
//glCompileShader (fragmentShader);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Link the vertex and fragment shader into a shader program
//GLuint shaderProgram = glCreateProgram ();
//glAttachShader (shaderProgram, vertexShader);
//glAttachShader (shaderProgram, fragmentShader);
//glBindFragDataLocation (shaderProgram, 0, "outColor");
//glLinkProgram (shaderProgram);
//glUseProgram (shaderProgram);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//// Load texture
//GLuint tex;
//glGenTextures (1, &tex);

//int width, height;
//unsigned char* image = SOIL_load_image ("Default.jpg", &width, &height, 0, SOIL_LOAD_RGB);
//glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//SOIL_free_image_data (image);

//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//////////////////////////////////////////////////////////////////////////
#pragma endregion
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

// Shader sources
const GLchar* vertexSource =
"#version 150 core\n"
"in vec2 position;"
"in vec4 color;"
"in vec2 texcoord;"
"out vec4 Color;"
"out vec2 Texcoord;"
"void main() {"
"   Color = color;"
"   Texcoord = texcoord;"
"   gl_Position = vec4(position, 0.0, 1.0);"
"}";
const GLchar* fragmentSource =
"#version 150 core\n"
"in vec4 Color;"
"in vec2 Texcoord;"
"out vec4 outColor;"
"uniform sampler2D tex;"
"void main() {"
"   outColor = Color;"
"}";

Sprite sprite;
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
    GLfloat vertices [] = {
      //  Position   Color             Texcoords
      -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,// Top-left
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // Top-right
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,// Bottom-right
      -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f // Bottom-left
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
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vertexShader, 1, &vertexSource, NULL);
    glCompileShader (vertexShader);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader (fragmentShader);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram ();
    glAttachShader (shaderProgram, vertexShader);
    glAttachShader (shaderProgram, fragmentShader);
    glBindFragDataLocation (shaderProgram, 0, "outColor");
    glLinkProgram (shaderProgram);
    glUseProgram (shaderProgram);
    //////////////////////////////////////////////////////////////////////////
     //LOAD RESOURCES (TEXTURES, SHADERS)
    resourceManager.Load_Resources ();
    //////////////////////////////////////////////////////////////////////////
    //// Specify the layout of the vertex data
    //GLint posAttrib = glGetAttribLocation (shaderProgram, "position");
    //glEnableVertexAttribArray (posAttrib);
    //glVertexAttribPointer (posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

    //GLint colAttrib = glGetAttribLocation (shaderProgram, "color");
    //glEnableVertexAttribArray (colAttrib);
    //glVertexAttribPointer (colAttrib, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));

    //GLint texAttrib = glGetAttribLocation (shaderProgram, "texcoord");
    //glEnableVertexAttribArray (texAttrib);
    //glVertexAttribPointer (texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat)));
    //////////////////////////////////////////////////////////////////////////

    sprite.Create (resourceManager.Get_Shader ("VertexShader.glsl")->shaderProgram);

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

  void WindowSystem::GraphicsUpdate(const double dt)
  {
    glClearColor (0, 0, 0, 0);
    glClear (GL_COLOR_BUFFER_BIT);
    //glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    sprite.Draw ();
  }


  ////////////////////////////////////////////////////////////
  



}
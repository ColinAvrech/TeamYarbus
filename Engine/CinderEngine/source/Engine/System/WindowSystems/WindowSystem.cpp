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


namespace Framework
{
  Sprite sprite;
  Sprite sprite1;
  ResourceManager resourceManager;
  glm::vec3 position;
  float shininess = 64.0f;
  VAO* vao;
  VBO* vbo;
  EBO* ebo;
  static GLfloat vertices [] =
  {
    //  Position   Color             Texcoords
    -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Top-left
    0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Top-right
    0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
  };

  static GLuint elements [] = {
    0, 1, 2,
    2, 3, 0
  };

  //! Global pointer to  the windows system.
  WindowSystem* WINDOWSYSTEM = NULL;

  namespace WindowNameSpace
  {
    void GLFWMessageHandler (GLFWwindow* window, int key, int scanCode, int state, int mod)
    {
      switch (key)
      {
      case GLFW_KEY_W:
        position.y += 0.01f;
        break;
      case GLFW_KEY_S:
        position.y -= 0.01f;
        break;
      case GLFW_KEY_D:
        position.x += 0.01f;
        break;
      case GLFW_KEY_A:
        position.x -= 0.01f;
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

      *GLFWwindowptr = glfwCreateWindow (1920, 1920, "OpenGL", nullptr, nullptr); // Windowed
      //*GLFWwindowptr = glfwCreateWindow (800, 600, "OpenGL", glfwGetPrimaryMonitor (), nullptr);
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
    // Create Vertex Array Object
    vao = new VAO ();
    // Create Vertex Buffer To Store Quad Vertices (Maybe replace it completely by creating a Geometry Shader which defines the Quad Shape)
    vbo = new VBO (sizeof (vertices), vertices);
    // Create Element/Index Buffer To Reduce Size Of Vertex Array (4 vertices instead of 6 for one Quad)
    ebo = new EBO (sizeof (elements), elements);

    // Load all Resources (Textures, Shaders, Maps,...)
    resourceManager.Load_Resources ();

    // Sprite can be created using this method of in the Non-Default Constructor
    // Get Shader will return Default shader if wrong name specified
    // Get Texture is optional
    // For Custom Mesh use the next two parameters
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

    //light position (is the same as the player position)
    glUniform3f (glGetUniformLocation (sprite.shaderID, "lightPos"), position.x, position.y, position.z);
    glUniform3f (glGetUniformLocation (sprite.shaderID, "mambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f (glGetUniformLocation (sprite.shaderID, "mdiffuse"), 0.6f, 0.6f, 0.6f);
    glUniform3f (glGetUniformLocation (sprite.shaderID, "mspecular"), 1.0f, 1.0f, 1.0f);

    //setting light property
    glUniform3f (glGetUniformLocation (sprite.shaderID, "lambient"), 0.2f, 0.2f, 0.2f);
    glUniform3f (glGetUniformLocation (sprite.shaderID, "ldiffuse"), 0.6f, 0.6f, 0.6f);
    glUniform3f (glGetUniformLocation (sprite.shaderID, "lspecular"), 1.0f, 1.0f, 1.0f);
    glUniform1f (glGetUniformLocation (sprite.shaderID, "shininess"), shininess);    //shininess

    glNormal3f (0.0f, 0.0f, 1.0f);
    sprite.Draw ();

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
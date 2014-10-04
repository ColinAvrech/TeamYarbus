/******************************************************************************/
/*!
\file   Scene_ComputeTest.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Scene_ComputeTest.h"
#include "ComponentInclude.h"
#include "ResourceManager.h"
#include "CLParticleRenderer.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "BaseEvent.h"
#include "KeyEvent.h"
#include "TimeQuery.h"

namespace Framework
{
  static double AppTime;
  static float Fps;

  class glRenderer
  {
  private:
    GLuint baseshader;
    GLuint computeshader;
    GLuint SSBOPos;
    GLuint SSBOVel;
    GLuint particleTex;
    float frameDelta = 0.0f;
    float speedMultiplier = 0.15f;
    bool borderEnabled = true;
    bool colorFade = false;
    bool pause = false;
    float color [3];
    float colVec [3];
    float colorChangeTimer;
    float colorChangeLength;
    void resetPositionSSBO ();
    void resetVelocitySSBO ();
    GLuint loadShader (const char* vertexShaderFile, const char* fragmentShaderFile);
    GLuint loadComputeShader (const char* computeShaderFile);
    void printProgramLog (GLuint shader);
    void printShaderLog (GLuint program);
  public:
    GLFWwindow* window;
    int particleCount = 100000;
    glRenderer ();
    ~glRenderer ();
    void generateShaders ();
    void generateBuffers ();
    void resetBuffers ();
    void generateTextures ();
    void renderScene ();
    void keyCallback (int key, int scancode, int action, int mods);
  };

  glRenderer renderer;

  static float random (float fMin, float fMax)
  {
    float fRandNum = (float) rand () / RAND_MAX;
    return fMin + (fMax - fMin) * fRandNum;
  }

  glRenderer::glRenderer ()
  {
    srand ((unsigned)time (NULL));
    color [0] = random (64, 255);
    color [1] = random (64, 255);
    color [2] = random (64, 255);
    colorChangeTimer = 1000.0f;
  }


  glRenderer::~glRenderer ()
  {
  }

  void glRenderer::generateShaders ()
  {
    baseshader = Resources::RS->Get_Shader ("Simple")->Get_ID();
    computeshader = Resources::RS->Get_ComputeShader ("Velocities.cl")->Get_ID ();
  }

  void glRenderer::resetPositionSSBO ()
  {

    // Reset to mouse cursor pos
    double cursorX, cursorY;
    int windowWidth, windowHeight;
    glfwPollEvents ();
    glfwGetCursorPos (window, &cursorX, &cursorY);
    glfwGetWindowSize (window, &windowWidth, &windowHeight);

    float destPosX = (float) (cursorX / (windowWidth) -0.5f) * 2.0f;
    float destPosY = (float) ((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;

    struct vertex4f* verticesPos = (struct vertex4f*) glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, particleCount * sizeof(vertex4f), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    for (int i = 0; i < particleCount; i++) {
      float rnd = (float) rand () / (float) (RAND_MAX);
      float rndVal = (float) rand () / (float) (RAND_MAX / (360.0f * 3.14f * 2.0f));
      float rndRad = (float) rand () / (float) (RAND_MAX) * 0.2f; // TODO : Change multiplier to get cool effects (e.g. wider range)
      verticesPos [i].x = destPosX + cos (rndVal) * rndRad;
      verticesPos [i].y = destPosY + sin (rndVal) * rndRad;
      verticesPos [i].z = 0.0f;
      verticesPos [i].w = 1.0f;
    }
    glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
  }

  void glRenderer::resetVelocitySSBO ()
  {
    struct vertex4f* verticesVel = (struct vertex4f*) glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, particleCount * sizeof(vertex4f), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    for (int i = 0; i < particleCount; i++) {
      verticesVel [i].x = 0.0f;
      verticesVel [i].y = 0.0f;
      verticesVel [i].z = 0.0f;
      verticesVel [i].w = 1.0f;
    }
    glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
  }

  void glRenderer::resetBuffers ()
  {
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, SSBOPos);
    resetPositionSSBO ();
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, SSBOVel);
    resetVelocitySSBO ();
  }

  void glRenderer::generateBuffers ()
  {
    // No more default VAO with OpenGL 3.3+ core profile context,
    // so in order to make our SSBOs render create and bind a VAO
    // that's never used again
    GLuint VAO;
    glGenVertexArrays (1, &VAO);
    glBindVertexArray (VAO);

    // Position SSBO
    if (glIsBuffer (SSBOPos)) {
      glDeleteBuffers (1, &SSBOPos);
    };
    glGenBuffers (1, &SSBOPos);
    // Bind to SSBO
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, SSBOPos);
    // Generate empty storage for the SSBO
    glBufferData (GL_SHADER_STORAGE_BUFFER, particleCount * sizeof(vertex4f), NULL, GL_STATIC_DRAW);
    // Fill
    resetPositionSSBO ();
    // Bind buffer to target index 0
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, SSBOPos);

    // Velocity SSBO
    if (glIsBuffer (SSBOVel)) {
      glDeleteBuffers (1, &SSBOVel);
    };
    glGenBuffers (1, &SSBOVel);
    // Bind to SSBO
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, SSBOVel);
    // Generate empty storage for the SSBO
    glBufferData (GL_SHADER_STORAGE_BUFFER, particleCount * sizeof(vertex4f), NULL, GL_STATIC_DRAW);
    // Fill
    resetVelocitySSBO ();
    // Bind buffer to target index 1
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, SSBOVel);

  }

  void glRenderer::generateTextures ()
  {
    particleTex = Resources::RS->Get_Texture ("Fire1.png")->Get_ID ();
  }

  void glRenderer::renderScene ()
  {

    if (colorFade)
    {

      colorChangeTimer -= frameDelta * 25.0f;

      if ((colorChangeTimer <= 0.0f) & (colorChangeLength <= 0.0f)) {
        if (random (0.0f, 100.0f) < 50.0f)
        {
          colVec [0] = (int) random (0.0f, 8.0f) * 32.0f - color [0];
          colVec [1] = (int) random (0.0f, 8.0f) * 32.0f - color [1];
          colVec [2] = (int) random (0.0f, 8.0f) * 32.0f - color [2];

          float vlength = sqrt (colVec [0] * colVec [0] + colVec [1] * colVec [1] + colVec [2] * colVec [2]);
          colorChangeLength = vlength * 2.0f;

          colVec [0] = colVec [0] / vlength;
          colVec [1] = colVec [2] / vlength;
          colVec [2] = colVec [2] / vlength;

        };
        colorChangeTimer = 1000.0f;
      }

      if (colorChangeLength > 0.0f)
      {
        color [0] += colVec [0] * frameDelta;
        color [1] += colVec [1] * frameDelta;
        color [2] += colVec [2] * frameDelta;

        colorChangeLength -= frameDelta;
      }

    }
    else
    {
      color [0] = 255.0f;
      color [1] = 64.0f;
      color [2] = 0.0f;
    }


    double frameTimeStart = glfwGetTime ();

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);

    // Run compute shader

    double cursorX, cursorY;
    int windowWidth, windowHeight;
    glfwGetCursorPos (window, &cursorX, &cursorY);
    glfwGetWindowSize (window, &windowWidth, &windowHeight);

    float destPosX = (float) (cursorX / (windowWidth) -0.5f) * 2.0f;
    float destPosY = (float) ((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;

    glUseProgram (computeshader);
    glUniform1f (glGetUniformLocation (computeshader, "deltaT"), frameDelta * speedMultiplier * (pause ? 0.0f : 1.0f));
    glUniform3f (glGetUniformLocation (computeshader, "destPos"), destPosX, destPosY, 0);
    glUniform2f (glGetUniformLocation (computeshader, "vpDim"), 1, 1);
    glUniform1i (glGetUniformLocation (computeshader, "borderClamp"), (int) borderEnabled);

    int workingGroups = particleCount / 16;

    glDispatchCompute (workingGroups, 1, 1);

    glUseProgram (0);

    // Set memory barrier on per vertex base to make sure we get what was written by the compute shaders
    glMemoryBarrier (GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    // Render scene

    glUseProgram (baseshader);

    glUniform4f (glGetUniformLocation (baseshader, "Color"), color [0] / 255.0f, color [1] / 255.0f, color [2] / 255.0f, 1.0f);

    glGetError ();

    glBindTexture (GL_TEXTURE_2D, particleTex);

    GLuint posAttrib = glGetAttribLocation (baseshader, "position");

    glBindBuffer (GL_ARRAY_BUFFER, SSBOPos);
    glVertexAttribPointer (posAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray (posAttrib);
    glPointSize (16);
    glDrawArrays (GL_POINTS, 0, particleCount);

    glfwSwapBuffers (window);

    frameDelta = (float) (glfwGetTime () - frameTimeStart) * 100.0f;

  }

  void glRenderer::keyCallback (int key, int scancode, int action, int mods)
  {
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
      resetBuffers ();
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
      speedMultiplier += 0.025f;
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
      speedMultiplier -= 0.025f;
    if (key == GLFW_KEY_B && action == GLFW_PRESS)
      borderEnabled = !borderEnabled;
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
      colorFade = !colorFade;
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
      pause = !pause;
    if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
    {
      particleCount += 1024;
      printf ("particle count : %d\nRegenerating SSBOs...\n", particleCount);
      generateBuffers ();
    }
    if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS && particleCount > 1024)
    {
      particleCount -= 1024;
      printf ("particle count : %d\nRegenerating SSBOs...\n", particleCount);
      generateBuffers ();
    }
  }


  CLParticleRenderer clRenderer;

  // Constructor
  Scene_ComputeTest::Scene_ComputeTest ()
  {
  }

  // Destructor
  Scene_ComputeTest::~Scene_ComputeTest ()
  {
  }


  void Scene_ComputeTest::Key_Pressed (int key, int scanCode, int state, int mods)
  {
  }


  void Scene_ComputeTest::Mouse_Button (int button, int action, int mods)
  {
  }


  void Scene_ComputeTest::Mouse_Moved (double xPos, double yPos)
  {
  }


  void OnKeyPressed (GameObject* go, KeyEvent* key)
  {
    if (key->KeyValue == GLFW_KEY_R && key->KeyDown)
      clRenderer.ResetBuffers ();
    if (key->KeyValue == GLFW_KEY_D && key->KeyDown)
      clRenderer.speedMultiplier += 0.025f;
    if (key->KeyValue == GLFW_KEY_A && key->KeyDown)
      clRenderer.speedMultiplier -= 0.025f;
    if (key->KeyValue == GLFW_KEY_B && key->KeyDown)
      clRenderer.borderEnabled = !clRenderer.borderEnabled;
    if (key->KeyValue == GLFW_KEY_C && key->KeyDown)
      clRenderer.colorFade = !clRenderer.colorFade;
    if (key->KeyValue == GLFW_KEY_P && key->KeyDown)
      clRenderer.pause = !clRenderer.pause;
    if (key->KeyValue == GLFW_KEY_X && key->KeyDown)
    {
      clRenderer.particleCount += 1024;
      printf ("particle count : %d\nRegenerating SSBOs...\n", clRenderer.particleCount);
      clRenderer.GenerateBuffers ();
    }
    if (key->KeyValue == GLFW_KEY_Z && key->KeyDown && clRenderer.particleCount > 1024)
    {
      clRenderer.particleCount -= 100000;
      printf ("particle count : %d\nRegenerating SSBOs...\n", clRenderer.particleCount);
      clRenderer.GenerateBuffers ();
    }

    //renderer.keyCallback (key->KeyValue, 0, key->KeyDown ? 1 : 0, 0);
  }


  void Scene_ComputeTest::Load_Scene (const char* filename)
  {

    EVENTSYSTEM->Connect (NULL, Events::KEY_ANY, BaseEvent::BaseCall (OnKeyPressed));

    renderer = glRenderer ();
    clRenderer.GenerateTextures ();
    clRenderer.GenerateBuffers ();
    clRenderer.GenerateShaders ();

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }


  void Scene_ComputeTest::Update (const double dt)
  {
    std::string WindowTitle = "OpenCL Particles ";
    double deltaTime;
    Utils::updateTimer (&deltaTime, &AppTime);
    Utils::calculateFps (&Fps);
    WindowTitle += std::to_string(clRenderer.particleCount) + " FPS: " + std::to_string (Fps);

    glfwSetWindowTitle (WINDOWSYSTEM->Get_Window (), WindowTitle.c_str ());
  }


  void Scene_ComputeTest::Draw ()
  {
    clRenderer.Render ();
  }
  
  
  void Scene_ComputeTest::Change_Size (int x, int y)
  {
  }

}
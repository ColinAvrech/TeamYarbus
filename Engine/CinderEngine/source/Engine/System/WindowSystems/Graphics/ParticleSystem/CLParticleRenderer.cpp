/******************************************************************************/
/*!
\file   CLParticleRenderer.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "CLParticleRenderer.h"
#include "WindowSystem.h"

namespace Framework
{

  static float random (float fMin, float fMax)
  {
    float fRandNum = (float) rand () / RAND_MAX;
    return fMin + (fMax - fMin) * fRandNum;
  }


  CLParticleRenderer::CLParticleRenderer ()
  {
    particleCount = MILLION;
    particleSize = 1.0f;
    srand ((unsigned) time (NULL));
    color [0] = random (64, 255);
    color [1] = random (64, 255);
    color [2] = random (64, 255);
    colorChangeTimer = 1000.0f;
  }

  CLParticleRenderer::~CLParticleRenderer ()
  {

  }

  void CLParticleRenderer::GenerateShaders ()
  {
    shader = Resources::RS->Get_Shader ("Simple");
    computeshader = Resources::RS->Get_ComputeShader ("Velocities.cl.glsl");
  }


  void CLParticleRenderer::GenerateBuffers ()
  {
    // OpenGL 3.3+
    // Create a VAO and never use it again!!!
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
    ResetPosition ();
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
    ResetVelocity ();
    // Bind buffer to target index 1
    glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, SSBOVel);
  }

  void CLParticleRenderer::ResetBuffers ()
  {
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, SSBOPos);
    ResetPosition ();
    glBindBuffer (GL_SHADER_STORAGE_BUFFER, SSBOVel);
    ResetVelocity ();
  }

  void CLParticleRenderer::GenerateTextures ()
  {
    texture = Resources::RS->Get_Texture ("Particle.bmp");
  }

  void CLParticleRenderer::ResetPosition ()
  {
    // Reset to mouse cursor pos
    double cursorX, cursorY;
    int windowWidth, windowHeight;
    glfwPollEvents ();
    glfwGetCursorPos (WINDOWSYSTEM->Get_Window(), &cursorX, &cursorY);
    glfwGetWindowSize (WINDOWSYSTEM->Get_Window(), &windowWidth, &windowHeight);

    float destPosX = (float) (cursorX / (windowWidth) -0.5f) * 2.0f;
    float destPosY = (float) ((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;

    struct vertex4f* verticesPos = (struct vertex4f*) glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, particleCount * sizeof(vertex4f), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    for (int i = 0; i < particleCount; i++)
    {
      float rnd = (float) rand () / (float) (RAND_MAX);
      float rndVal = (float) rand () / (float) (RAND_MAX / (360.0f * 3.14f * 2.0f));
      float rndRad = (float) rand () / (float) (RAND_MAX) * 0.2f;
      verticesPos [i].x = destPosX + cos (rndVal) * rndRad;
      verticesPos [i].y = destPosY + sin (rndVal) * rndRad;
      verticesPos [i].z = 0.0f;
      verticesPos [i].w = 1.0f;
    }
    glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
  }


  void CLParticleRenderer::ResetVelocity ()
  {
    struct vertex4f* verticesVel = (struct vertex4f*) glMapBufferRange (GL_SHADER_STORAGE_BUFFER, 0, particleCount * sizeof(vertex4f), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    for (int i = 0; i < particleCount; i++)
    {
      verticesVel [i].x = 0.0f;
      verticesVel [i].y = 0.0f;
      verticesVel [i].z = 0.0f;
      verticesVel [i].w = 1.0f;
    }
    glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);
  }


  void CLParticleRenderer::Render ()
  {
    if (colorFade)
    {
      colorChangeTimer -= frameDelta * 25.0f;

      if ((colorChangeTimer <= 0.0f) & (colorChangeLength <= 0.0f))
      {
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
        }
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
    glfwGetCursorPos (WINDOWSYSTEM->Get_Window(), &cursorX, &cursorY);
    glfwGetWindowSize (WINDOWSYSTEM->Get_Window(), &windowWidth, &windowHeight);

    float destPosX = (float) (cursorX / (windowWidth) -0.5f) * 2.0f;
    float destPosY = (float) ((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;

    computeshader->Use ();
    computeshader->uni1f ("deltaT", frameDelta * speedMultiplier * (pause ? 0 : 1));
    computeshader->uni3f ("destPos", destPosX, destPosY, 0);
    computeshader->uni2f ("vpDim", 1, 1);
    computeshader->uni1i("borderClamp", (int) borderEnabled);

    int workingGroups = particleCount / 16;

    //glDispatchCompute (workingGroups, 1, 1);
    computeshader->Dispatch_Compute (workingGroups, 1, 1);

    computeshader->Disable ();

    // Set memory barrier on per vertex base to make sure we get what was written by the compute shaders
    glMemoryBarrier (GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    // Render scene

    shader->Use ();

    shader->uni4f ("Color", color [0] / 255.0f, color [1] / 255.0f, color [2] / 255.0f, 1.0f);

    glGetError ();

    texture->Bind ();
    GLuint posAttrib = shader->attribLocation ("position");

    glBindBuffer (GL_ARRAY_BUFFER, SSBOPos);
    shader->vertexAttribPtr (posAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
    shader->enableVertexAttribArray (posAttrib);
    glPointSize (particleSize);
    glDrawArrays (GL_POINTS, 0, particleCount);

    texture->Unbind ();
    shader->Disable ();

    frameDelta = (float) (glfwGetTime () - frameTimeStart) * 100.0f;
  }

}
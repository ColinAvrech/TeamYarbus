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
#include "EventSystem.h"
#include "BaseEvent.h"
#include "KeyEvent.h"
#include "Thermodynamics.h"

namespace Framework
{
  static double cursorX = 0, cursorY = 0;
  static GLuint fbo, rbo;
  static float decayRate = 2.0f;
  static float breathRate = 0.01f;
  static float offset = 0.75f;

  static float random(float fMin, float fMax)
  {
    float fRandNum = (float)rand() / RAND_MAX;
    return fMin + (fMax - fMin) * fRandNum;
  }


  CLParticleRenderer::CLParticleRenderer()
  {
    particleCount = 10000;
    particleSize = 5;
    srand((unsigned)time(NULL));
    color[0] = 255;
    color[1] = 80;
    color[2] = 0;
    color[3] = 0.1f;
    colorChangeTimer = 1000.0f;
  }

  CLParticleRenderer::~CLParticleRenderer()
  {
    delete SSBOPos, SSBOVel, vao;
  }

  void CLParticleRenderer::GenerateShaders()
  {
    shader = Resources::RS->Get_Shader("Simple");
    computeshader = Resources::RS->Get_ComputeShader("Velocities.cl.glsl");
  }


  void CLParticleRenderer::GenerateBuffers()
  {
    // OpenGL 3.3+
    // Create a VAO and never use it again!!!
    vao = new VAO();
    SSBOPos = new SSBO(particleCount * sizeof(glm::vec4));

    // Fill
    ResetPosition();
    // Bind buffer to target index 0
    SSBOPos->BindBufferBase(0);


    SSBOVel = new SSBO(particleCount * sizeof(glm::vec4));
    ResetVelocity();
    // Bind buffer to target index 1
    SSBOVel->BindBufferBase(1);
  }

  void CLParticleRenderer::ResetBuffers()
  {
    SSBOPos->BindBuffer();
    ResetPosition();
    SSBOVel->BindBuffer();
    ResetVelocity();
  }


  void CLParticleRenderer::OnKeyPressed(KeyEvent* key)
  {
    std::cout << cursorX << std::endl;
    if (key->KeyDown)
    {
      if (key->KeyValue == GLFW_KEY_R)
        ResetBuffers();
      if (key->KeyValue == GLFW_KEY_A)
        speedMultiplier -= 0.1f;
      else if (key->KeyValue == GLFW_KEY_D)
        speedMultiplier += 0.1f;
      //else if (key->KeyValue == GLFW_KEY_D)
      //  cursorX += 5.f;

      //if (key->KeyValue == GLFW_KEY_D)
      //  cursorY -= 5.f;
      //else if (key->KeyValue == GLFW_KEY_W)
      //  cursorY += 5.f;
    }
  }


  void CLParticleRenderer::GenerateTextures()
  {
    texture = Resources::RS->Get_Texture("Particle.bmp");
    cursorX = WINDOWSYSTEM->Get_Width() / 2;
    cursorY = WINDOWSYSTEM->Get_Height() / 2;

    EVENTSYSTEM->mConnect<KeyEvent, CLParticleRenderer>(Events::KEY_ANY, this, &CLParticleRenderer::OnKeyPressed);
  }

  void CLParticleRenderer::ResetPosition()
  {
    // Reset to mouse cursor pos
    //double cursorX, cursorY;
    int windowWidth, windowHeight;
    glfwPollEvents();
    glfwGetCursorPos(WINDOWSYSTEM->Get_Window(), &cursorX, &cursorY);
    glfwGetWindowSize(WINDOWSYSTEM->Get_Window(), &windowWidth, &windowHeight);

    float destPosX = (float)(cursorX / (windowWidth)-0.5f) * 2.0f;
    float destPosY = (float)((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;

    glm::vec4* verticesPos = (glm::vec4*) SSBOPos->MapBufferRange<glm::vec4>(0, particleCount);
    for (int i = 0; i < particleCount; i++)
    {
      float rnd = (float)rand() / (float)(RAND_MAX);
      float rndVal = (float)rand() / (float)(RAND_MAX / (360.0f * 3.14f * 2.0f));
      float rndRad = (float)rand() / (float)(RAND_MAX)* 0.2f;
      radius = rndRad;
      verticesPos[i].x = destPosX + cos(rndVal) * rndRad;
      verticesPos[i].y = destPosY + sin(rndVal) * rndRad;
      verticesPos[i].z = 0.0f;
      verticesPos[i].w = 1.0f;
    }
    SSBOPos->UnMapBuffer();
  }


  void CLParticleRenderer::ResetVelocity()
  {
    glm::vec4* verticesVel = SSBOVel->MapBufferRange<glm::vec4>(0, particleCount);
    for (int i = 0; i < particleCount; i++)
    {
      verticesVel[i].x = 0.0f;
      verticesVel[i].y = 0.0f;
      verticesVel[i].z = 0.0f;
      verticesVel[i].w = 1.0f;
    }
    SSBOVel->UnMapBuffer();
  }


  void CLParticleRenderer::Render()
  {
    Interpolate_Colors();
    vao->bindVAO();
    SSBOPos->BindBufferBase(0);
    SSBOVel->BindBufferBase(1);

    if (AUDIOSYSTEM->input.peaklevel[0] > 0.05f)
    {
      if (color[3] < 1) color[3] += AUDIOSYSTEM->input.peaklevel[0] * 0.1f;
      //printf("%f\n", color[3]);
    }
    else
    {
      if (color[3] > 0.1f) color[3] -= 0.016f;
    }
    double frameTimeStart = glfwGetTime() * 1000;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Run compute shader

    //double cursorX, cursorY;
    int windowWidth, windowHeight;
    glfwGetCursorPos(WINDOWSYSTEM->Get_Window(), &cursorX, &cursorY);
    glfwGetWindowSize(WINDOWSYSTEM->Get_Window(), &windowWidth, &windowHeight);

    float destPosX = (float)(cursorX / (windowWidth)-0.5f) * 2.0f;
    float destPosY = (float)((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;

    computeshader->Use();
    computeshader->uni1f("deltaT", 2 * speedMultiplier * (pause ? 0 : 1));
    computeshader->uni3f("destPos", destPosX, destPosY, 0);
    computeshader->uni2f("vpDim", 1, 1);
    computeshader->uni1i("borderClamp", (int)borderEnabled);
    //std::cout << "{ " << Physics::THERMODYNAMICS->GetCellVelocity(20, 20).x << ", " << Physics::THERMODYNAMICS->GetCellVelocity(20, 20).y << " }\n";
    computeshader->uni2fv("cellVelocity", glm::value_ptr(Physics::THERMODYNAMICS->GetCellVelocity(20, 20)));
    radius = 0.1f;
    destPosY = -destPosY;
    float peak = AUDIOSYSTEM->input.peaklevel[0];
    //std::cout << Physics::THERMODYNAMICS->GetCellTemperature (20, 20) << "\n";
    Physics::THERMODYNAMICS->SetCellTemperature(destPosX, destPosY, 2250.0f, 0.016);
    Physics::THERMODYNAMICS->SetCellTemperature(destPosX + radius, destPosY, 2250.0f, 0.016);
    Physics::THERMODYNAMICS->SetCellTemperature(destPosX - radius, destPosY, 2250.0f, 0.016);
    Physics::THERMODYNAMICS->SetCellTemperature(destPosX, destPosY + radius, 2250.0f, 0.016);
    Physics::THERMODYNAMICS->SetCellTemperature(destPosX, destPosY - radius, 2250.0f, 0.016);
    Physics::THERMODYNAMICS->SetCellTemperature(destPosX + radius, destPosY + radius, 2250.0f, 0.016);
    Physics::THERMODYNAMICS->SetCellTemperature(destPosX - radius, destPosY - radius, 2250.0f, 0.016);
    Physics::THERMODYNAMICS->SetCellTemperature(destPosX + radius, destPosY - radius, 2250.0f, 0.016);
    Physics::THERMODYNAMICS->SetCellTemperature(destPosX - radius, destPosY + radius, 2250.0f, 0.016);

    int workingGroups = particleCount / 16;

    computeshader->Dispatch_Compute(workingGroups + 1, 1, 1);

    computeshader->Disable();

    // Set memory barrier on per vertex base to make sure we get what was written by the compute shaders
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    // Render scene

    shader->Use();

    shader->uni4f("Color", color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0f);

    glGetError();

    texture->Bind();
    GLuint posAttrib = shader->attribLocation("position");

    glBindBuffer(GL_ARRAY_BUFFER, SSBOPos->Get_POS());
    shader->vertexAttribPtr(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
    shader->enableVertexAttribArray(posAttrib);
    glPointSize(particleSize);
    glDrawArrays(GL_POINTS, 0, particleCount);

    texture->Unbind();
    shader->Disable();

    frameDelta = (float)(glfwGetTime() - frameTimeStart) * 1000.0f;
  }

  void CLParticleRenderer::Interpolate_Colors()
  {
    if (colorFade)
    {
      colorChangeTimer -= frameDelta * 25.0f;

      if ((colorChangeTimer <= 0.0f) & (colorChangeLength <= 0.0f))
      {
        if (random(0.0f, 100.0f) < 50.0f)
        {
          colVec[0] = (int)random(0.0f, 8.0f) * 32.0f - color[0];
          colVec[1] = (int)random(0.0f, 8.0f) * 32.0f - color[1];
          colVec[2] = (int)random(0.0f, 8.0f) * 32.0f - color[2];

          float vlength = sqrt(colVec[0] * colVec[0] + colVec[1] * colVec[1] + colVec[2] * colVec[2]);
          colorChangeLength = vlength * 2.0f;

          colVec[0] = colVec[0] / vlength;
          colVec[1] = colVec[2] / vlength;
          colVec[2] = colVec[2] / vlength;
        }
        colorChangeTimer = 1000.0f;
      }

      if (colorChangeLength > 0.0f)
      {
        color[0] += colVec[0] * frameDelta;
        color[1] += colVec[1] * frameDelta;
        color[2] += colVec[2] * frameDelta;
        colorChangeLength -= frameDelta;
      }
    }
    else
    {
      color[0] = 255.0f;
      color[1] = 64.0f;
      color[2] = 0.0f;
    }
  }

}
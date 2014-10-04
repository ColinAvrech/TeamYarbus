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
  static CLParticleRenderer clRenderer;

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
/******************************************************************************/
/*!
\file   SceneManager.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "EditorUI.h"
#include "SceneManager.h"
#include <ctime>
#include "Scene_SplashScreens.h"
#include "ParticleEditor.h"
#include "Scene_ComputeTest.h"
#include "Scene_CollisionTest.h"

namespace Framework
{
  Scene* SceneManager::current = NULL;
  Scene_SplashScreens* splashScreen;
  ParticleEditor* particleEditor;
  Scene_ComputeTest* computeTest;
  Scene_CollisionTest* collisionTest;

  SceneManager* SCENEMANAGER = NULL;

  // Constructor
  SceneManager::SceneManager ()
  {
    SCENEMANAGER = this;
  }
  
  
  // Called once when System is Initialized
  bool SceneManager::Initialize ()
  {
    std::cout << GetName () << " initialized\n";
    Load_Scene (collisionTest);
    return true;
  }

  // HARD CODED FOR NOW
  void SceneManager::Load_Scene (Scene* scene)
  {
    scene = new Scene_CollisionTest ();
    scene->Load_Scene (NULL);
    current = scene;
  }


  void SceneManager::Load_Next_Scene ()
  {
    Destroy (current);
    current = new ParticleEditor ();
    current->Load_Scene (NULL);
  }
  
  // Called Every Frame
  void SceneManager::Update (const double dt)
  {
    current->Update (dt);
    current->Draw ();
  }


  void SceneManager::Destroy (Scene* scene)
  {
    delete scene;
    scene = NULL;
  }
  
  // Destructor
  SceneManager::~SceneManager ()
  {}

  void SceneManager::Change_Size (int w, int h)
  {
    glViewport (0, 0, w, h);
    current->Change_Size (w, h);
  }

}
/******************************************************************************/
/*!
\file   SceneManager.cpp
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "SceneManager.h"
#include <ctime>

namespace Framework
{
  Scene* SceneManager::current = NULL;
  Scene_SplashScreens* splashScreen;
  ParticleEditor* particleEditor;

  SceneManager* SCENEMANAGER = NULL;

  // Constructor
  SceneManager::SceneManager ()
  {
    SCENEMANAGER = this;
    // random:
    srand ((unsigned int) time (NULL));
  }
  
  
  // Called once when System is Initialized
  bool SceneManager::Initialize ()
  {
    std::cout << GetName () << " initialized\n";
    Load_Scene (particleEditor);
    return true;
  }

  // HARD CODED FOR NOW
  void SceneManager::Load_Scene (Scene* scene)
  {
    scene = new ParticleEditor ();
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
    current->Change_Size (w, h);
  }

}
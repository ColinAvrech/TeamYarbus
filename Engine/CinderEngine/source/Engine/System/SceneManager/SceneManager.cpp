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

namespace Framework
{
  Scene* SceneManager::current = NULL;
  Scene_SplashScreens* splashScreen;

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
    Load_Scene (splashScreen);
    return true;
  }

  // HARD CODED FOR NOW
  void SceneManager::Load_Scene (Scene* scene)
  {
    scene = new Scene_SplashScreens ();
    scene->Load_Scene (NULL);
    current = scene;
  }


  void SceneManager::Load_Next_Scene ()
  {}
  
  // Called Every Frame
  void SceneManager::Update (const double dt)
  {
    current->Update (dt);
    current->Draw ();
  }
  
  // Destructor
  SceneManager::~SceneManager ()
  {}

}
/******************************************************************************/
/*!
\file   SceneManager.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SCENE_MANAGER_H
#define _SCENE_MANAGER_H

#include "BaseSystem.h"
#include "Scene_SplashScreens.h"
#include "ParticleEditor.h"
#include "glfw3.h"

namespace Framework
{

  class SceneManager : public BaseSystem
  {
  public:
  SceneManager ();
  ~SceneManager ();
  
  virtual bool Initialize ();
  virtual void Update(const double dt);
  virtual const std::string GetName() { return "SceneManager"; }

  void Load_Scene (Scene* scene);
  void Load_Next_Scene ();
  void Change_Size (int w, int h);
  void Key_Pressed (int key, int scanCode, int state, int mods);
  void Mouse_Position (double xPos, double yPos);
  void Mouse_Input (int button, int action, int mods);

  private:
    static Scene* current;
    void Destroy (Scene*);
  }; 

  extern SceneManager* SCENEMANAGER;
}

#endif
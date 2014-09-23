/******************************************************************************/
/*!
\file   Scene_.h
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SCENE_H
#define _SCENE_H

#include "Scene.h"

namespace Framework
{

  class Scene_ : public Scene
  {
  public:
  Scene_SplashScreens ();
  ~Scene_SplashScreens ();

  private:
    virtual void Key_Pressed (GLFWwindow* window, int key, int state, int action, int mods) override;
    virtual void Mouse_Button (GLFWwindow* window, int button, int state, int mods) override;
    virtual void Mouse_Moved (float xPos, float yPos) override;
    virtual void Update (const double dt) override;
    virtual void Draw () override;
    virtual void Load_Scene (const char* filename) override;
  }; 
}

#endif
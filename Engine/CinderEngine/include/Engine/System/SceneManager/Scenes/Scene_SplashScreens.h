/******************************************************************************/
/*!
\file   Scene_SplashScreens.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SCENE_SPLASH_H
#define _SCENE_SPLASH_H

#include "Scene.h"


namespace Framework
{

  class Scene_SplashScreens : public Scene
  {
  public:
  Scene_SplashScreens ();
  ~Scene_SplashScreens ();

  private:
    virtual void Update (const double dt) override;
    virtual void Draw () override;
    virtual void Load_Scene (const char* filename) override;
    virtual void Change_Size (int w, int h) override;
  };  
}

#endif
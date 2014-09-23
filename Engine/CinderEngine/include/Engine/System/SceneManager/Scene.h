/******************************************************************************/
/*!
\file   Scene.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SCENE_BASE_H
#define _SCENE_BASE_H


#include "GraphicsCommon.h"
#include "Resources.h"

namespace Framework
{

  class Scene
  {
  public:
  Scene () {};
  virtual ~Scene () {}

  friend class SceneManager;
  protected:
    virtual void Key_Pressed (GLFWwindow* window, int key, int state, int action, int mods) {}
    virtual void Mouse_Button (GLFWwindow* window, int button, int state, int mods) {}
    virtual void Mouse_Moved (float xPos, float yPos) {}
    virtual void Update (const double dt) = 0;
    virtual void Draw () = 0;
    virtual void Load_Scene (const char* filename) = 0;
  };  
}

#endif
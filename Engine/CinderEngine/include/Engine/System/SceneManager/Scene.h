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
#include "GameObject.h"
#include <vector>

namespace Framework
{

  class Scene
  {
  public:
    Scene ()
    {
    }
    virtual ~Scene () {}

  friend class SceneManager;
  protected:
    Camera camera;
    std::vector <GameObject*> sceneObjectList;
    virtual void Update (const double dt) = 0;
    virtual void Draw () = 0;
    virtual void Load_Scene (const char* filename) = 0;
    virtual void Change_Size (int, int) = 0;
  };  
}

#endif
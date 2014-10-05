/******************************************************************************/
/*!
\file   Scene_CollisionTest.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SCENE_COLLISION_TEST_H
#define _SCENE_COLLISION_TEST_H

#include "Scene.h"

namespace Framework
{

  class Scene_CollisionTest : public Scene
  {
  public:
  Scene_CollisionTest ();
  ~Scene_CollisionTest ();

  private:
    virtual void Update (const double dt) override;
    virtual void Draw () override;
    virtual void Load_Scene (const char* filename) override;
    virtual void Change_Size (int x, int y) override;
  }; 
}

#endif
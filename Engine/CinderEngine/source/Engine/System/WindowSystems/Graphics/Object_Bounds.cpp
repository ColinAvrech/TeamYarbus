/******************************************************************************/
/*!
\file   Pipeline.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Object_Bounds.h"

namespace Framework
{
  void Object_Bounds::Set_Bounds(GameObject *g, float l, float r, float t, float b)
  {
    obj = g;
    left = l;
    right = r;
    top = t;
    bottom = b;
  }

  bool Object_Bounds::Check_Bounds()
  {
    glm::vec2 center = static_cast<glm::vec2>(Camera::main->gameObject->Transform->GetPosition());
    glm::vec3 obj_pos = obj->Transform->GetPosition();
    glm::mat4 mvp = obj->Transform->GetModelViewProjectionMatrix();
    //glm::vec4 screen_pos = mvp * glm::vec4()
    return true;
  }

}

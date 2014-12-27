/******************************************************************************/
/*!
\file   Object_Bounds.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#pragma once

namespace Framework
{
  //Forward declaration
  class GameObject;

  class Object_Bounds
  {
  public:
    void Set_Bounds(GameObject *g, float l, float r, float t, float b);
    bool Check_Bounds();

  private:
    GameObject *obj;
    float left;
    float right;
    float bottom;
    float top;
  };
}

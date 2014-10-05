/******************************************************************************/
/*!
\file   DebugCircle.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _DEBUG_CIRCLE_H
#define _DEBUG_CIRCLE_H

#include "GraphicsCommon.h"
#include "ComponentInclude.h"

namespace Framework
{
  class DebugCircleRenderer
  {
  public:
    DebugCircleRenderer ();
    ~DebugCircleRenderer ();
    void Generate_Shaders ();
    void Generate_Buffers ();
    void Draw (CircleCollider* circle);

  private:
    Shader* dShader;
    GLuint vao;
    GLuint VBO [2];
    int circleDivisions = 40;
    bool wireframe = true;
  };
}

#endif
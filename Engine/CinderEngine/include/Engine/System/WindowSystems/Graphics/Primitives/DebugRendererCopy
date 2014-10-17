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
  class DebugRenderer
  {
  public:
    DebugRenderer ();
    ~DebugRenderer ();
    void Generate_Shaders ();
    void Generate_Buffers ();
    void Draw (CircleCollider* circle);
    void Draw (LineCollider* circle);
    void Draw (PointCollider* point);
    void Draw (ShapeCollider* shape, int sides = 3);

    template <typename T>
    void DrawT (T* shape);

    void Initialize ();
  private:
    Shader* circleShader;
    Shader* lineShader;
    Shader* pointShader;
    VAO* vao;
    VBO* vbo;
    int circleDivisions = 40;
    bool wireframe = true;

    inline void EnableVertexArrays ()
    {
      vao->bindVAO ();
      circleShader->enableVertexAttribArray (circleShader->attribLocation ("position"));
      circleShader->enableVertexAttribArray (circleShader->attribLocation ("color"));
    }

  };
}

#endif
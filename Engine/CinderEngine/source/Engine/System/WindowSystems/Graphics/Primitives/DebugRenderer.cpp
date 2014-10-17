/******************************************************************************/
/*!
\file   DebugCircle.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "DebugRenderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

namespace Framework
{
  static glm::mat4 Circle_Matrix (CircleCollider* circle)
  {
    float radius = circle->GetRadius ();
    glm::vec2 offset = circle->getOffset ();

    glm::mat4 cm = glm::translate (glm::vec3 (offset, -1.0f)) * glm::scale (glm::vec3 (radius, radius, 1.0f)); //* glm::rotate (circle->gameObject->Transform->GetRotation (), glm::vec3 (0, 0, 1));
    return Camera::GetViewToProjectionMatrix () * Camera::GetWorldToViewMatrix () * cm;
  }


  DebugRenderer::DebugRenderer ()
  {
  }


  DebugRenderer::~DebugRenderer ()
  {
  }


  void DebugRenderer::Initialize ()
  {
    Generate_Shaders ();
    Generate_Buffers ();
  }


  void DebugRenderer::Generate_Shaders ()
  {
    circleShader = Resources::RS->Get_Shader ("DebugCircle");
    lineShader = Resources::RS->Get_Shader ("DebugLine");
    pointShader = Resources::RS->Get_Shader ("DebugPoint");
    circleShader->bindAttribLocation (0, "position");
    circleShader->bindAttribLocation (1, "color");
  }

  void DebugRenderer::Generate_Buffers ()
  {
    // Default VAO needed for OpenGL 3.3+ core profiles
    vao = new VAO ();


    GLfloat points [] =
    {
      //  Coordinates  Color
      -0.45f, 0.45f, 1.0, 1.0f, 0.0f, 0.0f,
    };

    // Create VBO with point coordinates
    vbo = new VBO (sizeof(points), points);

    // Specify layout of point data
    GLint posAttrib = circleShader->attribLocation ("position");
    circleShader->enableVertexAttribArray (posAttrib);
    //glEnableVertexAttribArray (posAttrib);
    circleShader->vertexAttribPtr (posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

    GLint colAttrib = circleShader->attribLocation ("color");
    circleShader->enableVertexAttribArray (colAttrib);
    circleShader->vertexAttribPtr (colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));
  }

  void DebugRenderer::Draw (CircleCollider* circle)
  {
    EnableVertexArrays ();

    circleShader->Use ();

    circleShader->uni1f ("radius", 0.1f * 10);
    circleShader->uni1i ("divisions", (GLint) circleDivisions);

    if (circle != nullptr)
      circleShader->uniMat4 ("modelViewProjectionMatrix", glm::value_ptr (Circle_Matrix(circle)));
    else
      circleShader->uniMat4 ("modelViewProjectionMatrix", glm::value_ptr (glm::mat4 (1)));

    glPointSize (16.0f);
    glDrawArrays (GL_POINTS, 0, 1);

    circleShader->Disable ();
    vao->unbindVAO ();
  }


  void DebugRenderer::Draw (LineCollider* line)
  {
    EnableVertexArrays ();

    lineShader->Use ();

    if (line != nullptr)
      lineShader->uniMat4 ("modelViewProjectionMatrix", glm::value_ptr (line->gameObject->Transform->GetModelViewProjectionMatrix()));
    else
      lineShader->uniMat4 ("modelViewProjectionMatrix", glm::value_ptr (glm::mat4(1)));

    glPointSize (16.0f);
    glDrawArrays (GL_POINTS, 0, 1);

    lineShader->Disable ();
    vao->unbindVAO ();
  }


  void DebugRenderer::Draw (PointCollider* point)
  {
    EnableVertexArrays ();

    pointShader->Use ();

    if (point != nullptr)
      pointShader->uniMat4 ("modelViewProjectionMatrix", glm::value_ptr (point->gameObject->Transform->GetModelViewProjectionMatrix()));
    else
      pointShader->uniMat4 ("modelViewProjectionMatrix", glm::value_ptr (glm::mat4 (1)));

    glPointSize (16.0f);
    glDrawArrays (GL_POINTS, 0, 1);

    pointShader->Disable ();
    vao->unbindVAO ();
  }


  void DebugRenderer::Draw (ShapeCollider* shape, int sides /*= 3*/)
  {
    EnableVertexArrays ();

    circleShader->Use ();

    circleShader->uni1f ("radius", 0.1f * 10);
    circleShader->uni1i ("divisions", sides);

    if (shape != nullptr)
      circleShader->uniMat4 ("modelViewProjectionMatrix", glm::value_ptr (shape->gameObject->Transform->GetModelViewProjectionMatrix()));
    else
      circleShader->uniMat4 ("modelViewProjectionMatrix", glm::value_ptr (glm::mat4 (1)));

    glPointSize (16.0f);
    glDrawArrays (GL_POINTS, 0, 1);

    circleShader->Disable ();
    vao->unbindVAO ();
  }

}
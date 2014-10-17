/******************************************************************************/
/*!
\file   DebugCircle.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "DebugCircle.h"
#include "ResourceManager.h"
#include "GameObject.h"

namespace Framework
{
  DebugCircleRenderer::DebugCircleRenderer ()
  {
  }


  DebugCircleRenderer::~DebugCircleRenderer ()
  {
  }


  void DebugCircleRenderer::Generate_Shaders ()
  {
    dShader = Resources::RS->Get_Shader ("DebugCircle");
    dShader->bindAttribLocation (0, "position");
    dShader->bindAttribLocation (1, "color");
  }

  void DebugCircleRenderer::Generate_Buffers ()
  {
    // Default VAO needed for OpenGL 3.3+ core profiles
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);

    // Position data
    const GLfloat pointPos [] =
    {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f, 0.5f, 0.0f,
    };

    // Color data
    const GLfloat pointCol [] =
    {
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f
    };

    glGenBuffers (2, VBO);

    // VBO for position data
    glBindBuffer (GL_ARRAY_BUFFER, VBO [0]);
    glBufferData (GL_ARRAY_BUFFER, sizeof(pointPos), pointPos, GL_STATIC_DRAW);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray (0);

    // VBO for color data
    glBindBuffer (GL_ARRAY_BUFFER, VBO [1]);
    glBufferData (GL_ARRAY_BUFFER, sizeof(pointCol), pointCol, GL_STATIC_DRAW);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray (1);
  }

  void DebugCircleRenderer::Draw(CircleCollider* circle)
  {
    glBindVertexArray (vao);
    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);

    glPolygonMode (GL_FRONT_AND_BACK, (wireframe ? GL_LINE : GL_FILL));

    dShader->Use ();

    dShader->uni1f ("radius", circle->GetRadius () * 10);
    dShader->uni1i ("divisions", (GLint) circleDivisions);
    dShader->uniMat4 ("modelViewProjectionmatrix", glm::value_ptr (circle->gameObject->Transform->GetModelViewProjectionMatrix()));

    glPointSize (16.0f);
    glDrawArrays (GL_POINTS, 0, 1);

    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  }

  void DebugCircleRenderer::Draw(LineCollider* line)
  {
	  glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_INT, 0);
  }
}
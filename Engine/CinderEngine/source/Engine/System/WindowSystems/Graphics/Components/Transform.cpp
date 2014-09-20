/******************************************************************************/
/*!
\file   Transform.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Transform.h"

namespace Framework
{

  // Constructor
  Transform::Transform(GameObject* obj) : GameComponent(obj), position (0), scale (1)
  {
    modelMatrix.push_back (glm::mat4 (1));
    viewMatrix.push_back (glm::mat4 (1));
    projectionMatrix.push_back (glm::mat4 (1));

    modelViewMatrix = glm::mat4 (1);
    modelViewProjectionMatrix = glm::mat4 (1);
    normalMatrix = glm::mat3 (1);
    matricesReady = true;
    currentMatrix = 0;
  }
  
  void Transform::Load_Identity ()
  {
    if (currentMatrix == MODEL_MATRIX || currentMatrix == VIEW_MATRIX)
    {
      modelMatrix [modelMatrix.size () - 1] = glm::mat4 (1.0);
      viewMatrix [modelMatrix.size () - 1] = glm::mat4 (1.0);
      position = glm::vec3 (0);
      scale = glm::vec3 (1);
    }
    else
    {
      projectionMatrix [viewMatrix.size () - 1] = glm::mat4 (1.0);
    }
    matricesReady = false;
  }

  bool Transform::MatrixMode (int m)
  {
    if (m == MODEL_MATRIX || m == VIEW_MATRIX || m == PROJECTION_MATRIX)
    {
      currentMatrix = m;
      return true;
    }

    return false;
  }


  // Orthographic Projection
  void Transform::ortho (float left, float right, float bottom, float top, float near, float far)
  {
    projectionMatrix [projectionMatrix.size () - 1] = glm::ortho (left, right, bottom, top, near, far);
    matricesReady = false;
  }

  // Perspective Projection
  void Transform::perspective (float angle, float aRatio, float near, float far)
  {
    projectionMatrix [projectionMatrix.size () - 1] = glm::perspective (angle, aRatio, near, far);
    matricesReady = false;
  }

  // Replace the Fixed Functionality glTranslatef, glScalef,...
  void Transform::Translate (float x, float y, float z)
  {
    if (currentMatrix == MODEL_MATRIX)
    {
      modelMatrix [modelMatrix.size () - 1] *= glm::translate (glm::vec3 (x, y, z));
      position += glm::vec3 (x, y, z);
    }
    else if (currentMatrix == VIEW_MATRIX)
    {
      viewMatrix [viewMatrix.size () - 1] *= glm::translate (glm::vec3 (-x, -y, -z));
    }
    matricesReady = false;
  }

  // Non-Uniform Scale
  void Transform::Scale (float x, float y, float z)
  {
    if (currentMatrix == MODEL_MATRIX)
    {
      modelMatrix [modelMatrix.size () - 1] *= glm::scale (glm::vec3 (x, y, z));
    }
    else if (currentMatrix == VIEW_MATRIX)
    {
      viewMatrix [viewMatrix.size () - 1] *= glm::scale (-glm::vec3 (x, y, z));
    }
    matricesReady = false;
  }

  // Uniform Scale
  void Transform::Scale (float v)
  {
    if (currentMatrix == MODEL_MATRIX)
    {
      modelMatrix [modelMatrix.size () - 1] *= glm::scale (glm::vec3 (v, v, v));
      scale = glm::vec3 (v);
    }
    else if (currentMatrix == VIEW_MATRIX)
    {
      viewMatrix [viewMatrix.size () - 1] *= glm::scale (-glm::vec3 (v, v, v));
    }
    matricesReady = false;
  }


  void Transform::Rotate (float angle)
  {
    if (currentMatrix == MODEL_MATRIX)
    {
      modelMatrix [modelMatrix.size () - 1] *= glm::rotate (angle, glm::vec3 (0.0f, 0.0f, 1.0f));
    }
    else if (currentMatrix == VIEW_MATRIX)
    {
      viewMatrix [viewMatrix.size () - 1] *= glm::rotate (-angle, glm::vec3 (0.0f, 0.0f, 1.0f));
    }
    matricesReady = false;
  }

  //getters
  glm::mat4& Transform::GetModelMatrix()
  {
    return modelMatrix [modelMatrix.size () - 1];
  }

  glm::mat4& Transform::GetViewMatrix()
  {
    return viewMatrix [viewMatrix.size () - 1];
  }


  glm::mat4& Transform::GetProjectionMatrix()
  {
    return projectionMatrix [projectionMatrix.size () - 1];
  }

  glm::mat4& Transform::GetModelViewMatrix ()
  {
    if (!matricesReady)
      return viewMatrix [viewMatrix.size () - 1] * modelMatrix [modelMatrix.size () - 1];
    else
      return modelViewMatrix;
  }

  glm::mat4& Transform::GetModelViewProjectionMatrix()
  {
    if (!matricesReady)
      return viewMatrix [viewMatrix.size () - 1] * modelMatrix [modelMatrix.size () - 1] * projectionMatrix [projectionMatrix.size () - 1];
    else
      return modelViewProjectionMatrix;
  }

  glm::vec3 Transform::GetPosition()
  {
    //if (currentMatrix == 0)
    {
      //return glm::vec3 (GetModelMatrix()[3][0], GetModelMatrix() [3][1], GetModelMatrix() [3][2]);
      return position;
    }
  }


  glm::vec3 Transform::GetScale ()
  {
    return scale;
  }

  //GLSL
  void Transform::UpdateMatrices (GLuint programId)
  {
    if (!matricesReady)
    {
      modelViewMatrix = viewMatrix [viewMatrix.size () - 1] * modelMatrix [modelMatrix.size () - 1];
      modelViewProjectionMatrix = projectionMatrix [projectionMatrix.size () - 1] * viewMatrix [viewMatrix.size () - 1] * modelMatrix [modelMatrix.size () - 1];
      normalMatrix = glm::mat3 (modelViewMatrix);
    }

    glUniformMatrix4fv (glGetUniformLocation (programId, "modelMatrix"), 1, GL_FALSE, &modelMatrix [modelMatrix.size () - 1][0][0]);
    glUniformMatrix4fv (glGetUniformLocation (programId, "viewMatrix"), 1, GL_FALSE, &viewMatrix [viewMatrix.size () - 1][0][0]);
    glUniformMatrix4fv (glGetUniformLocation (programId, "modelViewMatrix"), 1, GL_FALSE, &modelViewMatrix [0][0]);
    glUniformMatrix4fv (glGetUniformLocation (programId, "modelViewProjectionMatrix"), 1, GL_FALSE, &modelViewProjectionMatrix [0][0]);
    glUniformMatrix3fv (glGetUniformLocation (programId, "normalMatrix"), 1, GL_FALSE, &normalMatrix [0][0]);
  }

  // PRIVATE METHODS

  void Transform::push_matrix ()
  {
  }

  void Transform::pop_matrix ()
  {
  }

  Transform::~Transform ()
  {}

}
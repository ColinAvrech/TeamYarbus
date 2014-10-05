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
  DefineComponentName (Transform);
  // Constructor

  Transform::Transform (GameObject* go) : position (0), scale (1), rotation (0)
  {
    modelMatrix.push_back (glm::mat4 (1));
    modelViewProjectionmatrix.push_back (glm::mat4 (1));
    normalMatrix = glm::mat3 (1);
    matricesReady = true;
    currentMatrix = 0;
    gameObject = go;
  }

  void Transform::Load_Identity ()
  {
    if (currentMatrix == MODEL_MATRIX || currentMatrix == VIEW_MATRIX)
    {
      modelMatrix [currentMatrix] = glm::mat4 (1.0);
      position = glm::vec3 (0);
      scale = glm::vec3 (1);
      rotation = 0;
    }
    matricesReady = false;
  }


  void Transform::Initalize ()
  {
  }


  void Transform::Serialize ()
  {
    //////////////////////////////////////////////////////////////////////////
    // DATA TO BE SERIALIZED
    // position : glm::vec3 (Serialized Data)
    // scale    : glm::vec3 (Serialized Data)
    // rotation : float (Serialized Data)
    //////////////////////////////////////////////////////////////////////////
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


  // Replace the Fixed Functionality glTranslatef, glScalef,...
  void Transform::Translate (float x, float y, float z)
  {
    position += glm::vec3 (x, y, z);
    matricesReady = false;
  }

  // Non-Uniform Scale
  void Transform::Scale (float x, float y, float z)
  {
    scale = glm::vec3 (x, y, z);
    matricesReady = false;
  }

  // Uniform Scale
  void Transform::Scale (float v)
  {
    scale = glm::vec3 (v);
    matricesReady = false;
  }


  void Transform::Rotate (float angle)
  {
    rotation += angle;
    matricesReady = false;
  }

  //getters
  glm::mat4 Transform::GetModelMatrix ()
  {
    return modelMatrix [currentMatrix];
  }

  glm::mat4 Transform::GetModelViewProjectionMatrix ()
  {
    if (!matricesReady)
      return Camera::GetViewToProjectionMatrix () * Camera::GetWorldToViewMatrix () * modelMatrix [currentMatrix];

    return modelViewProjectionmatrix [currentMatrix];
  }


  glm::vec3 Transform::GetPosition()
  {
    return position;
  }


  glm::vec3 Transform::GetScale ()
  {
    return scale;
  }

  //GLSL
  void Transform::UpdateMatrices()
{
    if (!matricesReady)
    {
      modelMatrix [currentMatrix] =
        glm::translate (position) *
        glm::rotate (rotation, glm::vec3 (0, 0, 1)) *
        glm::scale (scale);

      modelViewProjectionmatrix [currentMatrix] =
        Camera::GetViewToProjectionMatrix () *
        Camera::GetWorldToViewMatrix () *
        modelMatrix [currentMatrix];

      matricesReady = true;
    }
  }

  // PRIVATE METHODS

  void Transform::push_matrix ()
  {
    ++currentMatrix;
    modelMatrix.push_back (glm::mat4 (1));
  }

  void Transform::pop_matrix ()
  {
    modelMatrix.pop_back ();
    --currentMatrix;
  }

  Transform::~Transform ()
  {}

  void Transform::Print (glm::vec3 position)
  {
    std::cout << "( " << position.x << ", " << position.y << ", " << position.z << " )\n";
  }

}
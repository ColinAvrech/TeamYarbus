/******************************************************************************/
/*!
\file   Camera.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Camera.h"


namespace Framework
{
  std::vector<Camera*> Camera::allCameras;
  Camera* Camera::current = NULL;
  Camera* Camera::main = NULL;

  Camera::Camera (bool _main) : viewDirection (0.0f, 0.0f, -1.0f), up (0.0f, 1.0f, 0.0f)
  {
    size = 10.0f;
    aspect = 16.f/9;
    nearPlane = 0.1f;
    farPlane = 10.0f;

    allCameras.push_back (this);
    if (_main)
    {
      Camera::current = this;
      Camera::main = this;
      worldToView = glm::lookAt (position, position + viewDirection, up);
      viewToProjection = glm::ortho (-size, size, -size / aspect, size / aspect, nearPlane, farPlane);
    }
    matricesReady = true;
  }

  Camera::~Camera ()
  {}

  void Camera::MouseUpdate (const glm::vec2& newPosition)
  {
    glm::vec2 mouseDelta = newPosition - oldPosition;
    viewDirection = glm::mat3 (glm::rotate (mouseDelta.x * 0.01f, up)) * viewDirection;
    oldPosition = newPosition;
    matricesReady = false;
  }

  void Camera::UpdatePosition(const glm::vec3& deltaPos)
{
    position += deltaPos;
    matricesReady = false;
  }

  void Camera::Zoom (float zoom)
  {
    size += zoom;
    matricesReady = false;
  }


  glm::mat4 Camera::GetWorldToViewMatrix()
  {
    if (!Camera::main->matricesReady)
    {
      Camera::main->worldToView = glm::lookAt
        (
        Camera::current->position,
        Camera::current->position + Camera::current->viewDirection,
        Camera::current->up
        );
    }

    return Camera::main->worldToView;
  }

  glm::mat4 Camera::GetViewToProjectionMatrix ()
  {
    if (!Camera::main->matricesReady)
    {
      Camera::main->viewToProjection =
        glm::ortho
        (
        -Camera::main->size,
        Camera::main->size,
        -Camera::main->size / Camera::main->aspect,
        Camera::main->size / Camera::main->aspect,
        Camera::main->nearPlane,
        Camera::main->farPlane
        );
    }

    return Camera::main->viewToProjection;
  }

}
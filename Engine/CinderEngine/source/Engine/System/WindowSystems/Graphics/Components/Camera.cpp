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
  DefineComponentName (Camera);

  std::vector<Camera*> Camera::allCameras;
  Camera* Camera::current = NULL;
  Camera* Camera::main = NULL;

  Camera::Camera (GameObject* go) : up (0, 1, 0)
  {
    gameObject = go;
  }

  void Camera::Initalize ()
  {
    viewDirection = glm::vec3 (0.0f, 0.0f, -1.0f);
    aspect = 1920.f / 1280;
    nearPlane = 0.1f;
    farPlane = 10.0f;
    fov = 60.0f;

    allCameras.push_back (this);
    //if (_main)
    {
      Camera::main = this;
      worldToView = glm::lookAt (position, position + viewDirection, up);
      viewToProjection = glm::perspective (fov * M_PI / 180, aspect, nearPlane, farPlane);
    }
    Camera::current = this;
    matricesReady = true;
  }

  void Camera::Serialize ()
  {
    //////////////////////////////////////////////////////////////////////////
    // DATA TO BE SERIALIZED
    // viewDirection  : glm::vec3 (Serialized Data)
    // aspect         : float     (Serialized Data)
    // nearPlane      : float     (Serialized Data)
    // farPlane       : float     (Serialized Data)
    // fov            : float     (Serialized Data)
    // main           : bool      (Serialized Data)
    //////////////////////////////////////////////////////////////////////////
  }


  Camera::~Camera ()
  {}

  void Camera::MouseUpdate (const glm::vec2& newPosition)
  {
    glm::vec2 mouseDelta = newPosition - oldPosition;

    const float ROTATION_SPEED = 0.01f;
    const float MAX_DELTA = 150.0f;

    if (mouseDelta.length () < MAX_DELTA)
    {
      glm::vec3 vRotation = glm::cross (viewDirection, up);
      glm::mat4 rotator = glm::mat4
        (glm::rotate (mouseDelta.x * ROTATION_SPEED, up) * glm::rotate (mouseDelta.y * ROTATION_SPEED, vRotation));
      viewDirection = glm::mat3 (rotator) * viewDirection;
    }
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
    fov += zoom;
    matricesReady = false;
  }

}
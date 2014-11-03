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
#include "GameObject.h"


namespace Framework
{
  DefineComponentName(Camera);

  std::list<Camera*> Camera::allCameras;
  Camera* Camera::current = NULL;
  Camera* Camera::main = NULL;

  Camera::~Camera ()
  {
    allCameras.remove (this);
    gameObject->Camera = nullptr;
  }


  void Camera::Initialize ()
  {
    gameObject->Camera = this;
    allCameras.push_back(this);
    if (mainCamera)
    {
      Camera::main = this;
      worldToView = glm::lookAt(size * viewDirection, gameObject->Transform->GetPosition(), up);
      viewToProjection = glm::perspective(fov * M_PI / 180, aspect, nearPlane, farPlane);
    }
    Camera::current = this;
    matricesReady = true;
  }

  void Camera::Serialize(Serializer::DataNode* data)
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

    // Main?
    mainCamera = data->value_.Bool_;

    Serializer::DataNode* value = data->FindElement(data, "Facing");
    value->GetValue(&viewDirection);

    value = data->FindElement(data, "FieldOfView");
    value->GetValue(&fov);

    value = data->FindElement(data, "NearPlane");
    value->GetValue(&nearPlane);

    value = data->FindElement(data, "FarPlane");
    value->GetValue(&farPlane);

    value = data->FindElement (data, "Size");
    value->GetValue (&size);
	  aspect = 16.f / 9;
  }


  void Camera::MouseUpdate(const glm::vec2& newPosition)
  {
    glm::vec2 mouseDelta = newPosition - oldPosition;

    const float ROTATION_SPEED = 0.01f;
    const float MAX_DELTA = 150.0f;

    if (mouseDelta.length() < MAX_DELTA)
    {
      glm::vec3 vRotation = glm::cross(viewDirection, up);
      glm::mat4 rotator = glm::mat4
        (glm::rotate(mouseDelta.x * ROTATION_SPEED, up) * glm::rotate(mouseDelta.y * ROTATION_SPEED, vRotation));
      viewDirection = glm::mat3(rotator) * viewDirection;
    }
    oldPosition = newPosition;
    matricesReady = false;
  }

  void Camera::UpdatePosition(const glm::vec3& deltaPos)
  {
    position += deltaPos;
    matricesReady = false;
  }

  void Camera::Zoom(float zoom)
  {
    size += zoom;
    matricesReady = false;
  }

}
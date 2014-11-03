/******************************************************************************/
/*!
\file   Camera.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _CAMERA_H
#define _CAMERA_H

#include "GraphicsCommon.h"
#include "ComponentInclude.h"

namespace Framework
{

  class Camera : public Component
  {
  public:
    Camera () : up (0, 1, 0) {}
    Camera (GameObject* go);
    virtual ~Camera ();

    virtual void Initialize ();
    virtual void Serialize (Serializer::DataNode* data);

    void MouseUpdate (const glm::vec2& newPosition);
    void MouseUpdateY (const glm::vec2& newPosition);
    void UpdatePosition (const glm::vec3& deltaPos);
    void Zoom (float zoom);

    glm::mat4 WorldToViewMatrix ();
    glm::mat4 ViewToProjectionMatrix ();

    inline static glm::mat4 GetWorldToViewMatrix ();
    inline static glm::mat4 GetViewToProjectionMatrix ();
    static std::list <Camera*> allCameras;
    static Camera* current;
    static Camera* main;

    glm::vec3 position;
    glm::vec3 viewDirection;
    glm::mat4 worldToView;
    glm::mat4 viewToProjection;

    // The non-base component usees DefineComponentName macro to name component
    const static std::string Name;
  private:
    const glm::vec3 up;
    glm:: vec2 oldPosition;
    bool matricesReady;

    bool mainCamera;
    float fov;
    float size;
    float aspect;
    float nearPlane;
    float farPlane;

  };

  //////////////////////////////////////////////////////////////////////////
  // Inline Methods
  //////////////////////////////////////////////////////////////////////////

  inline glm::mat4 Camera::GetWorldToViewMatrix ()
  {
    if (!Camera::main->matricesReady)
    {
      Camera::main->worldToView = glm::lookAt
        (
        Camera::main->size * Camera::main->viewDirection,
        Camera::main->position,
        Camera::main->up
        );
    }

    return Camera::main->worldToView;
  }

  inline glm::mat4 Camera::GetViewToProjectionMatrix ()
  {
    if (!Camera::main->matricesReady)
    {
      Camera::main->viewToProjection =
        glm::perspective
        (
        Camera::main->fov,
        Camera::main->aspect,
        Camera::main->nearPlane,
        Camera::main->farPlane
        );
    }

    return Camera::main->viewToProjection;
  }
}

#endif
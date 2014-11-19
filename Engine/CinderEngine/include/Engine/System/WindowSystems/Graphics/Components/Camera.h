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
  class KeyEvent;
  class Camera : public Component
  {
  public:
    Camera () : up (0, 1, 0) {}
    Camera (GameObject* go);
    virtual ~Camera ();

    virtual void Initialize ();
    virtual void Serialize (Serializer::DataNode* data);

    void MouseUpdate (const vec2& newPosition);
    void MouseUpdateY (const vec2& newPosition);
    void UpdatePosition (const vec3& deltaPos);
    void Zoom (float zoom);

    glm::mat4 WorldToViewMatrix ();
    glm::mat4 ViewToProjectionMatrix ();

    static glm::mat4 GetWorldToViewMatrix ();
    static glm::mat4 GetViewToProjectionMatrix ();
    void OnKeyPressed (KeyEvent* key);
    static std::list <Camera*> allCameras;
    static Camera* current;
    static Camera* main;

    vec3 position;
    vec3 viewDirection;
    glm::mat4 worldToView;
    glm::mat4 viewToProjection;

    // The non-base component usees DefineComponentName macro to name component
    const static string Name;
  private:
    const vec3 up;
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
}

#endif
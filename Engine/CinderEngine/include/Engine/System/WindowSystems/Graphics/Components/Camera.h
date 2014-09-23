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
    Camera (GameObject* obj, bool main = false);
    void MouseUpdate (const glm::vec2& newPosition);
    void MouseUpdateY (const glm::vec2& newPosition);
    void UpdatePosition (const glm::vec3& deltaPos);
    void Zoom (float zoom);

    static glm::mat4 GetWorldToViewMatrix ();
    static glm::mat4 GetViewToProjectionMatrix ();
    static float GetOrthographicSize ();
    static std::vector <Camera*> allCameras;
    static Camera* current;
    static Camera* main;

    ~Camera ();
    glm::vec3 position;
    glm::vec3 viewDirection;
    glm::mat4 worldToView;
    glm::mat4 viewToProjection;
    const glm::vec3 up;
    // The non-base component usees DefineComponentName macro to name component
    const static std::string Name;
  private:
    glm:: vec2 oldPosition;
    bool matricesReady;

    float fov;
    float size;
    float aspect;
    float nearPlane;
    float farPlane;

  };  
}

#endif
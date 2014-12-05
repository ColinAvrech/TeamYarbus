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

#include "ComponentInclude.h"
#include "CinderMath.h"
#include "ZilchCompiledLib.h"

namespace Framework
{
  class KeyEvent;
  class Pipeline;

  enum CAMERA_ZOOM_STATE
  {
    CZ_NONE,
    CZ_ZOOMOUT,
    CZ_ZOOMIN,
  };

  class Camera : public Component
  {
  public:
	ZilchDeclareDerivedType(Camera, Component);
    Camera () : up (0, 1, 0) {}
    Camera (GameObject* go);
    virtual ~Camera ();

    virtual void Initialize ();
    virtual void Serialize (Serializer::DataNode* data);

    void UpdateCamera (Pipeline* p);
    void MouseUpdate (const vec2& newPosition);
    void MouseUpdateY (const vec2& newPosition);
    void UpdatePosition (const vec3& deltaPos);
    void Zoom (float zoom);

    glm::mat4 WorldToViewMatrix ();
    glm::mat4 ViewToProjectionMatrix ();
	

    static glm::mat4 GetWorldToViewMatrix ();
    static glm::mat4 GetViewToProjectionMatrix ();
    static glm::vec2 GetWorldMousePosition ();
    void OnKeyPressed (KeyEvent* key);
    virtual void OnApplicationPause (PauseEvent* pause);
    float GetSize ();
    float GetFOV ();
    static std::list <Camera*> allCameras;
    static Camera* current;
    static Camera* main;
    CAMERA_ZOOM_STATE czs = CZ_NONE;
    float originalSize;
    float timer = 0.0f;
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // ZILCH FUNCTIONS
	static Zilch::Real2 ZGetWorldMousePosition();
    //////////////////////////////////////////////////////////////////////////
	float aspect;
    // The non-base component usees DefineComponentName macro to name component
    const static string Name;
  private:
    const vec3 up;
    glm:: vec2 oldPosition;
    bool matricesReady;

    vec3 position;
    vec3 viewDirection;
    glm::mat4 worldToView;
    glm::mat4 viewToProjection;

    bool mainCamera;
    float fov;
    float size;
    //ApectMovedUp
    float nearPlane;
    float farPlane;

  };

  //////////////////////////////////////////////////////////////////////////
  // Inline Methods
  //////////////////////////////////////////////////////////////////////////
}

#endif
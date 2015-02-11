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

#include "MetaCreator.h"

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
	  META_DECLARE( Camera );

    ZilchDeclareDerivedType(Camera, Component);
    Camera();
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
    std::pair<float, float> GetPlanes();
    int GetElementNum();
    float GetAperture();
    float FocalPoint();
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
  float size;
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
    float fov = 45.0f;
    //ApectMovedUp
    float nearPlane = 0;
    float farPlane = 10;
    //camera properties
    int Elements = 6;
    float f_stop = 2.8f;
    float FocalLength = 0.15f;
    float FocalPlane;
  };

  //////////////////////////////////////////////////////////////////////////
  // Inline Methods
  //////////////////////////////////////////////////////////////////////////
}

META_DEFINE( Framework::Camera, Camera )
{

}

#endif
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

namespace Framework
{

  class Camera
  {
  public:

    //////////////////////////////////////////////////////////////////////////
    // Constructors
    //////////////////////////////////////////////////////////////////////////
    Camera ();
    Camera (glm::vec3 location);
    Camera (glm::vec3 location, float yaw, float pitch);
    Camera (glm::vec3 location, float yaw, float pitch, float mv, float mov);
    //////////////////////////////////////////////////////////////////////////

    void UpdateCamera (Transform& p);
    glm::vec3 GetVector ();
    glm::vec3 GetLocation ();
    float GetPitch ();
    float GetYaw ();

    void SetLocation (glm::vec3 vec);
    void LookAt (float pitch, float yaw);

    ~Camera ();
  
  private:
    glm::vec3 location;
    float camPitch, camYaw;
    float movevel;
    float mousevel;
    bool mi, ismoved;


    void lockCamera ();
    void MoveCamera (float dir);
    void MoveCameraUp (float dir);

  };  
}

#endif
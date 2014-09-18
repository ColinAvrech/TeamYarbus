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

  void Camera::lockCamera ()
  {
    if (camPitch > 90)
      camPitch = 90;
    if (camPitch < -90)
      camPitch = -90;
    if (camYaw<0.0)
      camYaw += 360.0f;
    if (camYaw>360.0f)
      camYaw -= 360;
  }


  Camera::Camera ()
  {
    camPitch = 0;
    camYaw = 0;
    movevel = 0.2f;
    mousevel = 0.2f;
    mi = ismoved = false;
  }
  Camera::Camera (glm::vec3 l)
  {
    location = l;
    camPitch = 0;
    camYaw = 0;
    movevel = 0.2f;
    mousevel = 0.2f;
    mi = ismoved = false;
  }

  Camera::Camera (glm::vec3 l, float yaw, float pitch)
  {
    location = l;
    camPitch = pitch;
    camYaw = yaw;
    movevel = 0.2f;
    mousevel = 0.2f;
    mi = ismoved = false;
  }

  Camera::Camera (glm::vec3 l, float yaw, float pitch, float mv, float mov)
  {
    location = l;
    camPitch = pitch;
    camYaw = yaw;
    movevel = mv;
    mousevel = mov;
    mi = false;
  }

  void Camera::UpdateCamera (Transform& t)
  {
    t.MatrixMode (VIEW_MATRIX);
    t.Translate (location.x, location.y, location.z);
  }

  //change the spherical coordinate system to cartesian
  glm::vec3 Camera::GetVector ()
  {
    //Google->spherical to cartesian
    return (glm::vec3 (-cos (camPitch*3.1415f / 180.0)*sin (camYaw*3.1415f / 180.0), sin (camPitch*3.1415 / 180.0), -cos (camPitch*3.1415f / 180.0)*cos (camYaw*3.1415f / 180.0)));
  }
  glm::vec3 Camera::GetLocation ()
  {
    return location;
  }

  float Camera::GetPitch ()
  {
    return camPitch;
  }

  float Camera::GetYaw ()
  {
    return camYaw;
  }
}
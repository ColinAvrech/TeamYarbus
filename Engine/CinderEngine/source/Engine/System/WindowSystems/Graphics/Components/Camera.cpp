/******************************************************************************/
/*!
\file   Camera.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Camera.h"
#include "EventSystem.h"
#include "KeyEvent.h"
#include "BaseEvent.h"
#include "WindowSystem.h"
#include "GameObject.h"
#include "Pipeline.h"
#include "GameEvent.h"
#include "CharacterController.h"
#include "Core.h"
#include "Events.h"


namespace Framework
{

  ZilchDefineType(Camera, CinderZilch)
  {
    type->HandleManager = ZilchManagerId(Zilch::PointerManager);
    ZilchBindStaticMethodAs(ZGetWorldMousePosition, "GetWorldMousePosition");
    ZilchBindStaticFieldGetAs(main, "Main");
    ZilchBindFieldGetAs(aspect, "Aspect");
    ZilchBindFieldGetAs(size, "Size");
    //ZilchBindStaticMethodAs(ZGetCameraMousePosition, "GetCameraMousePosition");
  }
  DefineComponentName(Camera);

  std::list<Camera*> Camera::allCameras;
  Camera* Camera::current = nullptr;
  Camera* Camera::main = nullptr;

  Camera::Camera() : up(0, 1, 0)
  {
    fov = 45.0f;
    viewDirection = { 0, 0, 1 };
    aspect = (float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height();
  }

  Camera::~Camera ()
  {
    if (mainCamera)
    {
      Camera::main = nullptr;
    }
    allCameras.remove (this);
    OPENGL->cameras.remove (this);
    EVENTSYSTEM->mDisconnect <KeyEvent, Camera> (Events::KEY_ANY, this, &Camera::OnKeyPressed);
  }


  void Camera::OnKeyPressed (KeyEvent* key)
  {
    float camSpeed = 1.0f;
    float zoomSpeed = 1.0f;

    Transform* tform = static_cast<Transform*>(Camera::main->gameObject->GetComponent("Transform"));
    if (key->KeyDown)
      switch (key->KeyValue)
    {
      case GLFW_KEY_A:
        OPENGL->MatrixMode(VIEW);
        tform->Translate(-camSpeed, 0, 0);
        Camera::main->matricesReady = false;
        break;
      case GLFW_KEY_D:
        OPENGL->MatrixMode(VIEW);
        tform->Translate(camSpeed, 0, 0);
        Camera::main->matricesReady = false;
        break;
      case GLFW_KEY_S:
        OPENGL->MatrixMode(VIEW);
        tform->Translate(0, -camSpeed, 0);
        Camera::main->matricesReady = false;
        break;
      case GLFW_KEY_W:
        OPENGL->MatrixMode(VIEW);
        tform->Translate(0, camSpeed, 0);
        Camera::main->matricesReady = false;
        break;
      case GLFW_KEY_Z:
        Camera::main->Zoom(zoomSpeed);
        break;
      case GLFW_KEY_X:
        Camera::main->Zoom(-zoomSpeed);
        break;
      case GLFW_KEY_Q:
        OPENGL->MatrixMode(VIEW);
        tform->Translate(0, 0, camSpeed);
        Camera::main->matricesReady = false;
        break;
      case GLFW_KEY_E:
        OPENGL->MatrixMode(VIEW);
        tform->Translate(0, 0, -camSpeed);
        Camera::main->matricesReady = false;
        break;
      case GLFW_KEY_I:
        OPENGL->MatrixMode(VIEW);
        tform->Translate(0, 0, camSpeed);
        Camera::main->Zoom(-zoomSpeed);
        Camera::main->matricesReady = false;
        break;
      case GLFW_KEY_O:
        OPENGL->MatrixMode(VIEW);
        tform->Translate(0, 0, -camSpeed);
        Camera::main->Zoom(zoomSpeed);
        Camera::main->matricesReady = false;
        break;
      default:
        break;
    }
  }

  void Camera::OnApplicationPause (PauseEvent* pause)
  {
    timer = 0.0f;
    if (pause->Paused)
    {
      czs = CZ_ZOOMOUT;
    }
    else
    {
      czs = CZ_ZOOMIN;
    }
  }


  void Camera::Initialize ()
  {
#ifdef _DEBUG // Moving the camera using WASD, ZX for debug
    EVENTSYSTEM->mConnect <KeyEvent, Camera> (Events::KEY_ANY, this, &Camera::OnKeyPressed);
#endif
    EVENTSYSTEM->mConnect <PauseEvent, Camera> (Events::PAUSE, this, &Camera::OnApplicationPause);
    allCameras.push_back(this);
    Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
    if (mainCamera)
    {
      Camera::main = this;
      //Camera::main->Zoom(-100.0f);
      worldToView = glm::lookAt(size * viewDirection + vec3 (tform->GetPosition().x, tform->GetPosition().y, 0.0), tform->GetPosition(), up);
      viewToProjection = glm::perspective(fov * M_PI / 180, aspect, nearPlane, farPlane);
    }
    Camera::current = this;

    OPENGL->cameras.push_back(this);
    OPENGL->Perspective (fov, aspect, nearPlane, farPlane);
    OPENGL->MatrixMode (MODEL);
    OPENGL->Translatefv (const_cast <float*>(glm::value_ptr (tform->GetPosition ())));
    OPENGL->MatrixMode (VIEW);
    OPENGL->LookAt
      (size * viewDirection +
      glm::vec3 (tform->GetPosition ().x, tform->GetPosition ().y, 0.0),
      tform->GetPosition (), up);
    OPENGL->MatrixMode (MODEL);
    OPENGL->LoadIdentity ();
    matricesReady = true;

  }

  void Camera::Serialize(Serializer::DataNode* data)
  {
    //////////////////////////////////////////////////////////////////////////
    // DATA TO BE SERIALIZED
    // viewDirection  : vec3  
    // aspect         : float 
    // nearPlane      : float 
    // farPlane       : float 
    // fov            : float 
    // main           : bool  
    // diaphragm      : int    (number of elements in the diaphragm)  
    // f_stop         : float  (ratio of the lens's focal length to the diameter of the entrance pupil)
    // FocalLength    : float  (focal length of camera lens)
    //////////////////////////////////////////////////////////////////////////

    // Main?
    Serializer::DataNode* value;// = data->FindElement(data, "Facing");

    value = data->FindElement(data, "main");
    value->GetValue(&mainCamera);

    if (mainCamera)
    {
      gameObject->AddComponent("Follow");
    }

    //value->GetValue(&viewDirection);

    value = data->FindElement(data, "FieldOfView");
    value->GetValue(&fov);

    value = data->FindElement(data, "NearPlane");
    value->GetValue(&nearPlane);

    value = data->FindElement(data, "FarPlane");
    value->GetValue(&farPlane);

    value = data->FindElement (data, "Size");
    value->GetValue (&size);

    value = data->FindElement(data, "Elements");
    if (value != nullptr)
      value->GetValue(&Elements);
    
      value = data->FindElement(data, "f_stop");
    if (value != nullptr)
      value->GetValue(&f_stop);

    value = data->FindElement(data, "FocalLength");
    if (value != nullptr)
      value->GetValue(&FocalLength);
    else
      FocalLength = 0.15f;
  }


  void Camera::MouseUpdate(const vec2& newPosition)
  {
    vec2 mouseDelta = newPosition - oldPosition;

    const float ROTATION_SPEED = 0.01f;
    const float MAX_DELTA = 150.0f;

    if (mouseDelta.length() < MAX_DELTA)
    {
      vec3 vRotation = glm::cross(viewDirection, up);
      glm::mat4 rotator = glm::mat4
        (glm::rotate(mouseDelta.x * ROTATION_SPEED, up) * glm::rotate(mouseDelta.y * ROTATION_SPEED, vRotation));
      viewDirection = glm::mat3(rotator) * viewDirection;
    }
    oldPosition = newPosition;
    matricesReady = false;
  }

  void Camera::UpdateCamera (Pipeline* p)
  {
    if (glfwGetMouseButton (WINDOWSYSTEM->Get_Window (), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
      //MouseUpdate (WINDOWSYSTEM->Get_Mouse_Position ());
    }
  
    switch (czs)

    {
    case Framework::CZ_NONE:
      break;
    case Framework::CZ_ZOOMOUT:
      timer += 0.016f;
      if (size < 28.0f)
      {
        size = glm::mix (size, 28.0f, timer);
        std::cout << originalSize << "\n";
      }
      if (timer >= 1.0f)
      {
        czs = CZ_NONE;
        timer = 0.0f;
      }
      break;
    case Framework::CZ_ZOOMIN:
      timer += 0.016f;
      if (size > 16.0f)
      {
        size = glm::mix(size, 16.0f, timer);
        std::cout << originalSize << "\n";
      }
      if (timer >= 1.0f)
      {
        czs = CZ_NONE;
        timer = 0.0f;
      }
      break;
    default:
      break;
    }

    //if (enabled)
    {
      //gameObject->Transform->SetPosition (PLAYER->gameObject->Transform->GetPosition ().x, PLAYER->gameObject->Transform->GetPosition ().y);
      OPENGL->Perspective (fov, aspect, nearPlane, farPlane);
      Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
      vec3 eye = size * viewDirection + glm::vec3(tform->GetPosition().x, tform->GetPosition().y, 0.0);
      OPENGL->LookAt( eye, tform->GetPosition(), up);
      OPENGL->MatrixMode (MODEL);
      OPENGL->LoadIdentity ();
    }
  }

  void Camera::UpdatePosition(const vec3& deltaPos)
  {
    position += deltaPos;
    matricesReady = false;
  }

  void Camera::Zoom(float zoom)
  {
    //if (size + zoom > 0 && (size + zoom) < farPlane)
    //{
      fov += zoom;
      matricesReady = false;
    //}
  }

  glm::mat4 Camera::GetWorldToViewMatrix ()
  {
    //if (!Camera::main->matricesReady)
    if (Camera::main)
    {
      Transform* tform = static_cast<Transform*>(Camera::main->gameObject->GetComponent("Transform"));
      vec3 eye = Camera::main->size * Camera::main->viewDirection + vec3(tform->GetPosition().x, tform->GetPosition().y, 0.0);
      vec3 center = tform->GetPosition();
      Camera::main->worldToView = glm::lookAt( eye, center, Camera::main->up);
    }

    return Camera::main->worldToView;
  }

  glm::mat4 Camera::GetViewToProjectionMatrix ()
  {
    //if (!Camera::main->matricesReady)
    {
      Camera::main->viewToProjection =
      glm::perspective
      (
      Camera::main->fov * M_PI / 180,
      (float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height(),
      Camera::main->nearPlane,
      Camera::main->farPlane
      );
    }

    return Camera::main->viewToProjection;
  }

  float Camera::GetSize ()
  {
    return size;
  }

  float Camera::GetFOV ()
  {
    return fov;
  }

  std::pair<float, float> Camera::GetPlanes()
  {
    return std::make_pair(nearPlane, farPlane);
  }

  int Camera::GetElementNum()
  {
    return Elements;
  }

  float Camera::GetAperture()
  {
    return FocalLength / f_stop;
  }

  float Camera::FocalPoint()
  {
    return FocalPlane;
  }

  glm::vec2 Camera::GetWorldMousePosition()
  {
    glm::vec2 ndc = { (WINDOWSYSTEM->Get_Mouse_Position().x / WINDOWSYSTEM->Get_Width() - 0.5f) * 1.47f,
      ((WINDOWSYSTEM->Get_Height() - WINDOWSYSTEM->Get_Mouse_Position().y) / WINDOWSYSTEM->Get_Height() - 0.5f) * 1.47f };
    
    //
    /*
        destPosX = (float) (cursorX / (windowWidth) -0.5f) * 2.0f;//1.47
        destPosY = (float) ((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;
    */
    //
    return glm::vec2
      (
        ndc.x * Camera::main->size, ndc.y * Camera::main->size / Camera::main->aspect
      );
  }

  Zilch::Real2 Camera::ZGetWorldMousePosition()
  {
    glm::vec2 ndc = Camera::GetWorldMousePosition();

    //
    /*
    destPosX = (float) (cursorX / (windowWidth) -0.5f) * 2.0f;
    destPosY = (float) ((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;
    */
    //
    return Zilch::Real2(Zilch::Real(ndc.x), Zilch::Real(ndc.y));
      
  }


}
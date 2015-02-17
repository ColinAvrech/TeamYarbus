/******************************************************************************/
/*!
\file   Transform.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Transform.h"
#include "GameObject.h"
#include "WindowSystem.h"
#include "Pipeline.h"
#include "Collider2D.h"
#include "Thermodynamics.h"
#include "ZInterface.h"


using namespace Zilch;

namespace Framework
{
  DefineComponentName(Transform);
  // Constructor

  ZilchDefineType(Transform, CinderZilch)
  {
    type->HandleManager = ZilchManagerId(Zilch::PointerManager);
    ZilchBindConstructor();
    ZilchBindMethod(Initialize);
    ZilchBindMethodOverload(Scale, void, float, float, float);
    ZilchBindMethodOverload(Scale, void, float);
    ZilchBindMethodAs(ZGetTranslation, "GetTranslation");
    ZilchBindMethodAs(ZSetTranslation, "SetTranslation");
    ZilchBindMethod(GetRotation);
    ZilchBindMethodAs(ZGetScale, "GetScale");
    ZilchBindMethodAs(ZGetLocalScale, "GetLocalScale");
    ZilchBindMethodAs(ZGetLocalPosition, "GetLocalTranslation");
    ZilchBindMethodAs(GetLocalRotation, "GetLocalRotation");
    ZilchBindMethodAs(ZSetScale, "SetScale");
    ZilchBindMethodOverload(Translate, void, float, float, float);
    ZilchBindMethodAs(Rotate, "SetRotation");
    ZilchBindMethodAs(ZGetScreenPosition, "GetScreenPosition");
  }

  Transform::Transform()
  {
    localPosition = { 0, 0, 0 };
    localScale = { 1, 1, 1 };
    localRotation = 0.0f;
  }

  Transform::~Transform()
  {
    OPENGL->transforms.remove(this);
  }

  void Transform::Load_Identity()
  {
    modelMatrix = glm::mat4(1.0);
    position = vec3(0);
    scale = vec3(1);
    rotation = 0;
    matricesReady = false;
  }


  void Transform::Serialize(Serializer::DataNode* data)
  {
    //////////////////////////////////////////////////////////////////////////
    // DATA TO BE SERIALIZED
    // position : vec3 (Serialized Data)
    // scale    : vec3 (Serialized Data)
    // rotation : float (Serialized Data)
    //////////////////////////////////////////////////////////////////////////
    Serializer::DataNode* value = data->FindElement(data, "Translation");
    value->GetValue(&localPosition);
    position = localPosition;

    value = data->FindElement(data, "Scale");
    value->GetValue(&localScale);

    value = data->FindElement(data, "Rotation");
    if (value)
    {
      value->GetValue(&localRotation);
    }

    value = data->FindElement(data, "Angle");
    if (value)
    {
      value->GetValue(&localRotation);
    }
  }


  void Transform::Initialize()
  {
    if (gameObject->Parent)
    {
      Transform* parentPos = static_cast<Transform*>(gameObject->Parent->GetComponent("Transform"));
      UpdateRotation(parentPos);
      UpdateScale(parentPos);
      UpdatePosition(parentPos);
    }
    else
    {
      position = localPosition;
      rotation = localRotation;
      scale = localScale;
    }

    OPENGL->MatrixMode(MODEL);
    OPENGL->LoadIdentity();
    OPENGL->Scalefv(glm::value_ptr(scale));
    OPENGL->Rotatef(rotation, 0, 0, 1);
    OPENGL->Translatefv(glm::value_ptr(position));
    modelMatrix = OPENGL->GetModelMatrix();
    OPENGL->LoadIdentity();
    modelViewProjectionMatrix = glm::mat4(1);
    normalMatrix = glm::mat3(1);
    matricesReady = false;
    currentMatrix = 0;
    //UpdateMatrices ();

    OPENGL->transforms.push_back(this);
  }


  // Replace the Fixed Functionality glTranslatef, glScalef,...
  Zilch::Real3 Transform::ZGetTranslation()
  {
    return Zilch::Real3(Zilch::Real(position.x), Zilch::Real(position.y), Zilch::Real(position.z));
  }

  Zilch::Real3 Transform::ZGetScale()
  {
    return Zilch::Real3(Zilch::Real(scale.x), Zilch::Real(scale.y), Zilch::Real(scale.z));
  }

  void Transform::ZSetTranslation(Zilch::Real3 newpos)
  {
    localPosition = vec3(newpos.x, newpos.y, newpos.z);
    if (gameObject->Parent)
    {
      Transform* parentPos = static_cast<Transform*>(gameObject->Parent->GetComponent("Transform"));
      UpdatePosition(parentPos);
    }
    else
    {
      position = localPosition;
    }

    //UpdateChildren();
    matricesReady = false;
  }

  void Transform::ZSetScale(Zilch::Real3 newscale)
  {
    Scale(newscale.x, newscale.y, newscale.z);
  }

  void Transform::Translate(float x, float y, float z)
  {
    Translate(vec3(x, y, z));

  }

  void Transform::Translate(const vec3 &v)
  {
    localPosition += v;
    if (gameObject->Parent)
    {
      Transform* parentPos = static_cast<Transform*>(gameObject->Parent->GetComponent("Transform"));
      UpdatePosition(parentPos);
    }
    else
    {
      position = localPosition;
    }

    //UpdateChildren();
    matricesReady = false;
  }

  // Non-Uniform Scale
  void Transform::Scale(float x, float y, float z)
  {
    localScale = vec3(x, y, z);
    //Update model matrix
    if (gameObject->Parent)
    {
      Transform* parentPos = static_cast<Transform*>(gameObject->Parent->GetComponent("Transform"));
      vec3 parentScale = parentPos->GetScale();
      UpdateScale(parentPos);
    }
    else
    {
      scale = localScale;
    }

    OPENGL->Scalefv(glm::value_ptr(scale));
    OPENGL->Rotatef(rotation, 0, 0, 1);
    OPENGL->Translatefv(glm::value_ptr(position));
    modelMatrix = OPENGL->GetModelMatrix();

    //UpdateChildren();
    matricesReady = false;
  }

  // Uniform Scale
  void Transform::Scale(float v)
  {
    Scale(v, v, v);
    ShapeCollider2D* shape = static_cast<ShapeCollider2D*>(gameObject->Parent->GetComponent("ShapeCollider2D"));

    //TODO: Support universal scaling on all collider types
    if (shape && shape->GetType() == ShapeCollider2D::eCircle)
    {
      static_cast<CircleCollider2D*>(shape)->SetRadius( std::abs(scale.x) );
    }
  }


  void Transform::Rotate(float angle)
  {
    localRotation = angle;

    if (gameObject->Parent)
    {
      Transform* parentPos = static_cast<Transform*>(gameObject->Parent->GetComponent("Transform"));
      UpdateRotation(parentPos);
    }
    else
    {
      rotation = localRotation;
    }

    //UpdateChildren();
    matricesReady = false;
  }

  //getters
  glm::mat4 Transform::GetModelMatrix()
  {
    return modelMatrix;
  }

  glm::mat4 Transform::GetModelViewProjectionMatrix()
  {
    return modelViewProjectionMatrix;
  }


  vec2 Transform::GetNDCPosition()
  {
    return vec2(GetModelViewProjectionMatrix()[3][0] / GetModelViewProjectionMatrix()[3][3],
      GetModelViewProjectionMatrix()[3][1] / GetModelViewProjectionMatrix()[3][3]);
  }

  glm::vec2 Transform::GetNDCPosition(const glm::vec2& v)
  {
    glm::mat4 matrix = modelMatrix;
    matrix[3][0] = v.x;
    matrix[3][1] = v.y;
    glm::mat4 mvp = (modelViewProjectionMatrix / modelMatrix) * matrix;

    return glm::vec2(mvp[3][0] / mvp[3][3], mvp[3][1] / mvp[3][3]);
  }

  glm::vec2 Transform::GetGridPosition()
  {
    return glm::vec2
      (
      position.x + (Physics::THERMODYNAMICS->MapSize.x * 0.5f) - 2,
      position.y
      );
  }

  glm::vec2 Transform::GetGridPosition(glm::vec2 pos)
  {
    return glm::vec2
      (
      pos.x + (Physics::THERMODYNAMICS->MapSize.x),
      pos.y
      );
  }

  glm::vec2 Transform::GetScreenPosition()
  {
    //glm::vec2 screenPos;
    glm::vec3 screenPos = glm::project
      (
      position,
      Camera::GetWorldToViewMatrix() * modelMatrix,
      Camera::GetViewToProjectionMatrix(),
      glm::vec4(0, 0, WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height())
      );

    //destPosX = (float) (cursorX / (windowWidth) -0.5f) * 2.0f;
    //destPosY = (float) ((windowHeight - cursorY) / windowHeight - 0.5f) * 2.0f;

    screenPos.x = (float)(screenPos.x / (WINDOWSYSTEM->Get_Width())) * 2.0f;
    screenPos.y = (float)(screenPos.y / (WINDOWSYSTEM->Get_Height())) * 2.0f;
    //screenPos.x = (position.x / Camera::main->GetSize () - 0.5f) * 2.0f;
    //screenPos.y = position.y / (Camera::main->GetSize() * ((float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height()))
    return glm::vec2(screenPos);
  }

  Zilch::Real2 Transform::ZGetScreenPosition(Zilch::Real2 pos)
  {
    glm::mat4 matrix = modelMatrix;
    matrix[3][0] = pos.x;
    matrix[3][1] = pos.y;
    glm::mat4 mvp = (modelViewProjectionMatrix / modelMatrix) * matrix;

    return Zilch::Real2(mvp[3][0] / mvp[3][3], mvp[3][1] / mvp[3][3]);
  }
  /*
  Zilch::Real2 WindowSystem::ZGetMouseScreenPosition()
  {
  glm::vec2 pos = WindowSystem::Get_Normalized_Mouse_Position();

  glm::mat4 matrix = modelMatrix;
  matrix[3][0] = pos.x;
  matrix[3][1] = pos.y;
  glm::mat4 mvp = (modelViewProjectionMatrix * modelMatrix) / matrix;

  return Zilch::Real2(mvp[3][0] / mvp[3][3], mvp[3][1] / mvp[3][3]);
  }
  */

  vec3 Transform::GetPosition()
  {
    return position;
  }

  vec2 Transform::GetPosition2D()
  {
    return vec2(position.x, position.y);
  }

  vec3 Transform::GetScale()
  {
    return scale;
  }

  float Transform::GetRotation()
  {
    return rotation;
  }

  vec3 Transform::GetLocalPosition()
  {
    return localPosition;
  }

  vec3 Transform::GetLocalScale()
  {
    return localScale;
  }

  float Transform::GetLocalRotation()
  {
    return localRotation;
  }

  Real3 Transform::ZGetLocalPosition()
  {
    return *ZInterface::VecToReal(&localPosition);
  }

  Real3 Transform::ZGetLocalScale()
  {
    return *ZInterface::VecToReal(&localScale);
  }

  void Transform::UpdatePosition(Transform* trans)
  {
    assert(gameObject && "Missing gameObject*");
    if (gameObject->InheritPosition)
    {
      vec3 positionAdd = trans->GetPosition();
      /*
      float parentAngle = trans->GetRotation();
      float cosX = 1;
      float sinX = 1;
      if (gameObject->InheritRotation)
      {
        //positionAdd[0] *= cos(parentAngle);
        //positionAdd[1] *= sin(parentAngle);
        //DOES NOT DEAL WITH Z POSITION
      }
      */
      position = localPosition + positionAdd;
    }
    else
    {
      position = localPosition;
    }

    Camera* cam = gameObject->C<Camera>();
    if (cam)
    {
      cam->UpdatePosition(position);
    }
  }
  void Transform::UpdateScale(Transform* trans)
  {
    if (gameObject->InheritScale)
    {
      vec3 parentScale = trans->GetScale();
      scale[0] = localScale[0] * parentScale[0];
      scale[1] = localScale[1] * parentScale[1];
      scale[2] = localScale[2] * parentScale[2];
    }
    else
    {
      scale = localScale;
    }

  }
  void Transform::UpdateRotation(Transform* trans)
  {
    if (gameObject->InheritRotation)
    {
      rotation = localRotation + trans->GetRotation();
    }
    else
    {
      rotation = localRotation;
    }
  }

  //void Transform::UpdateChildren()
  //{
  // for (auto i : gameObject->children)
  // {
  //  //i->Transform->matricesReady = false;
  // }
  //}

  //GLSL
  void Transform::UpdateMatrices()
  {
    //if (!matricesReady)
    {
      OPENGL->MatrixMode(MODEL);
      if (gameObject->Parent)
      {
        Transform* parentPos = static_cast<Transform*>(gameObject->Parent->GetComponent("Transform"));
        UpdateRotation(parentPos);
        UpdateScale(parentPos);
        UpdatePosition(parentPos);
      }
      else
      {
        position = localPosition;
        rotation = localRotation;
        scale = localScale;
      }

      OPENGL->Translatefv(glm::value_ptr(position));
      OPENGL->Scalefv(glm::value_ptr(scale));
      OPENGL->Rotatef(rotation, 0, 0, 1);

      modelMatrix = OPENGL->GetModelMatrix();
      //modelViewProjectionMatrix = OPENGL->GetModelViewProjectionMatrix();
      OPENGL->LoadIdentity();

      matricesReady = true;
    }

    //matricesReady = true;
  }

  void Transform::Print(vec3 position)
  {
    std::cout << "( " << position.x << ", " << position.y << ", " << position.z << " )\n";
  }

  void Transform::SetPosition(const vec2& v)
  {
    localPosition.x = v.x;
    localPosition.y = v.y;

    matricesReady = false;
  }

  void Transform::SetMVP(glm::mat4 &mvp)
  {
    //perspective * lookat * model
    modelViewProjectionMatrix = mvp * modelMatrix;
  }

}
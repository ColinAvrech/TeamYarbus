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


using namespace Zilch;

namespace Framework
{
  DefineComponentName (Transform);
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
	ZilchBindMethodAs(ZSetScale, "SetScale");
	  ZilchBindMethodOverload(Translate, void, float, float, float);
    ZilchBindMethodAs(Rotate, "SetRotation");
	ZilchBindMethodAs(ZGetScreenPosition, "GetScreenPosition");
  }

  Transform::Transform ()
  {
    position = { 0, 0, 0 };
    scale = { 1, 1, 1 };
    rotation = 0.0f;
  }

  Transform::~Transform ()
  {
    OPENGL->transforms.remove (this);
    gameObject->Transform = nullptr;
  }

  void Transform::Load_Identity ()
  {
    modelMatrix = glm::mat4 (1.0);
    position = vec3 (0);
    scale = vec3 (1);
    rotation = 0;
    matricesReady = false;
  }


  void Transform::Serialize (Serializer::DataNode* data)
  {
    //////////////////////////////////////////////////////////////////////////
    // DATA TO BE SERIALIZED
    // position : vec3 (Serialized Data)
    // scale    : vec3 (Serialized Data)
    // rotation : float (Serialized Data)
    //////////////////////////////////////////////////////////////////////////
    Serializer::DataNode* value = data->FindElement (data, "Translation");
    value->GetValue (&position);

    value = data->FindElement (data, "Scale");
    value->GetValue (&scale);

    value = data->FindElement (data, "Rotation");
    value->GetValue (&rotation);
  }


  void Transform::Initialize ()
  {
    gameObject->Transform = this;
    OPENGL->MatrixMode (MODEL);
    OPENGL->LoadIdentity ();
    OPENGL->Scalefv (glm::value_ptr (scale));
    OPENGL->Rotatef (rotation, 0, 0, 1);
    OPENGL->Translatefv (glm::value_ptr (position));
    modelMatrix = OPENGL->GetModelMatrix ();
    OPENGL->LoadIdentity ();
    modelViewProjectionMatrix = (glm::mat4 (1));
    normalMatrix = glm::mat3 (1);
    matricesReady = false;
    currentMatrix = 0;
    //UpdateMatrices ();

    gameObject->Transform = this;

    OPENGL->transforms.push_back (this);
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
	  position = vec3(newpos.x, newpos.y, newpos.z);
	  matricesReady = false;
  }

  void Transform::ZSetScale(Zilch::Real3 newscale)
  {
	  scale = vec3(newscale.x, newscale.y, newscale.z);
	  matricesReady = false;
  }

  void Transform::Translate (float x, float y, float z)
  {
    position += vec3 (x, y, z);
    matricesReady = false;
  }

  void Transform::Translate(const vec3 &v)
  {
	  position += v;
	  matricesReady = false;
  }

  // Non-Uniform Scale
  void Transform::Scale (float x, float y, float z)
  {
    scale = vec3 (x, y, z);
    //Update model matrix
    OPENGL->Scalefv(glm::value_ptr(scale));
    OPENGL->Rotatef(rotation, 0, 0, 1);
    OPENGL->Translatefv(glm::value_ptr(position));
    modelMatrix = OPENGL->GetModelMatrix();
    matricesReady = false;
  }

  // Uniform Scale
  void Transform::Scale (float v)
  {
    scale = vec3 (v);

    if (gameObject->RigidBody2D != nullptr)
    {
      gameObject->RigidBody2D->shape->radius = std::abs (scale.x);
    }
    matricesReady = false;
  }


  void Transform::Rotate (float angle)
  {
    rotation = angle;
    matricesReady = false;
  }

  //getters
  glm::mat4 Transform::GetModelMatrix ()
  {
    return modelMatrix;
  }

  glm::mat4 Transform::GetModelViewProjectionMatrix ()
  {
    return modelViewProjectionMatrix;
  }


  vec2 Transform::GetNDCPosition ()
  {
    return vec2 (GetModelViewProjectionMatrix () [3][0] / GetModelViewProjectionMatrix () [3][3],
      GetModelViewProjectionMatrix () [3][1] / GetModelViewProjectionMatrix () [3][3]);
  }

  glm::vec2 Transform::GetNDCPosition (const glm::vec2& v)
  {
    glm::mat4 matrix = modelMatrix;
    matrix [3][0] = v.x;
    matrix [3][1] = v.y;
    glm::mat4 mvp = (modelViewProjectionMatrix / modelMatrix) * matrix;

    return glm::vec2 (mvp [3][0] / mvp [3][3], mvp [3][1] / mvp [3][3]);
  }

  glm::vec2 Transform::GetGridPosition ()
  {
    return glm::vec2
    (
      position.x + (Physics::THERMODYNAMICS->MapSize.x * 0.5f) - 2,
      position.y
    );
  }

  glm::vec2 Transform::GetGridPosition (glm::vec2 pos)
  {
    return glm::vec2
      (
      pos.x + (Physics::THERMODYNAMICS->MapSize.x),
      pos.y
      );
  }

  glm::vec2 Transform::GetScreenPosition ()
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

    screenPos.x = (float) (screenPos.x / (WINDOWSYSTEM->Get_Width ())) * 2.0f;
    screenPos.y = (float) (screenPos.y / (WINDOWSYSTEM->Get_Height ())) * 2.0f;
    //screenPos.x = (position.x / Camera::main->GetSize () - 0.5f) * 2.0f;
    //screenPos.y = position.y / (Camera::main->GetSize() * ((float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height()))
    return glm::vec2 (screenPos);
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

  vec3 Transform::GetPosition ()
  {
    return position;
  }

  vec3 Transform::GetScale ()
  {
    return scale;
  }

  float Transform::GetRotation ()
  {
    return rotation;
  }


  //GLSL
  void Transform::UpdateMatrices()
  {
    //if (!matricesReady)
    {
      OPENGL->MatrixMode (MODEL);
      OPENGL->Translatefv (glm::value_ptr (position));
      OPENGL->Scalefv (glm::value_ptr (scale));
      OPENGL->Rotatef (rotation, 0, 0, 1);

      modelMatrix = OPENGL->GetModelMatrix ();
      modelViewProjectionMatrix = OPENGL->GetModelViewProjectionMatrix ();
      OPENGL->LoadIdentity ();

      matricesReady = true;
    }

    //matricesReady = true;
  }

  void Transform::Print (vec3 position)
  {
    std::cout << "( " << position.x << ", " << position.y << ", " << position.z << " )\n";
  }

  void Transform::SetPosition (float x, float y)
  {
    position.x = x;
    position.y = y;
  }

}
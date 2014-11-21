/******************************************************************************/
/*!
\file   Transform.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Transform.h"
#include "GameObject.h"
#include "WindowSystem.h"
#include "Pipeline.h"


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
	  ZilchBindMethodOverload(Translate, void, float, float, float);
    ZilchBindMethod(Rotate);
	  //ZilchBindMethod(GetScreenPosition);
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
    modelMatrix = (glm::translate (position) *
      glm::rotate (rotation, vec3 (0, 0, 1)) *
      glm::scale (scale));
    modelViewProjectionMatrix = (glm::mat4 (1));
    normalMatrix = glm::mat3 (1);
    matricesReady = false;
    currentMatrix = 0;
    //UpdateMatrices ();

    gameObject->Transform = this;

    OPENGL->transforms.push_back (this);
  }


  // Replace the Fixed Functionality glTranslatef, glScalef,...
  void Transform::Translate (float x, float y, float z)
  {
    position += vec3 (x, y, z);
    matricesReady = false;
  }

  void Transform::Translate(vec3 &v)
  {
	  position += v;
	  matricesReady = false;
  }

  // Non-Uniform Scale
  void Transform::Scale (float x, float y, float z)
  {
    scale = vec3 (x, y, z);
    matricesReady = false;
  }

  // Uniform Scale
  void Transform::Scale (float v)
  {
    scale = vec3 (v);
    matricesReady = false;
  }


  void Transform::Rotate (float angle)
  {
    rotation += angle;
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


  vec2 Transform::GetScreenPosition ()
  {
    return vec2 (GetModelViewProjectionMatrix () [3][0] / GetModelViewProjectionMatrix () [3][3],
      GetModelViewProjectionMatrix () [3][1] / GetModelViewProjectionMatrix () [3][3]);
  }

  glm::vec2 Transform::GetScreenPosition (glm::vec2 v)
  {
    glm::mat4 matrix = modelMatrix;
    matrix [3][0] = v.x;
    matrix [3][1] = v.y;
    glm::mat4 mvp = (modelViewProjectionMatrix / modelMatrix) * matrix;

    return glm::vec2 (mvp [3][0] / mvp [3][3], mvp [3][1] / mvp [3][3]);
  }


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

}
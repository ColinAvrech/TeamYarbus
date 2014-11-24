/******************************************************************************/
/*!
\file   Transform.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#pragma once
#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "GraphicsCommon.h"
#include "ComponentInclude.h"
#include "JSONSerializer.h"

#include "ZilchCompiledLib.h"




namespace Framework
{

  class Transform : public Component
  {
  public:
    ZilchDeclareBaseType(Transform, Zilch::TypeCopyMode::ReferenceType);
    
    Transform ();
    virtual ~Transform();
    virtual void Initialize ();
    virtual void Serialize (Serializer::DataNode* data);
    //////////////////////////////////////////////////////////////////////////
    // Transformations
    void Load_Identity ();
    void Translate (const float x, const float y, const float z);
	void Translate(const vec3 &v);
    void Scale (const float x, const float y, const float z);
    void Scale (const float v);
    void Rotate (const float angle);
    //////////////////////////////////////////////////////////////////////////
    bool MatrixMode (int m);
    void UpdateMatrices ();

    //getters
    glm::mat4 GetModelMatrix ();
    glm::mat4 GetModelViewProjectionMatrix ();
    /*inline*/ vec3 GetPosition ();
    /*inline*/ vec3 GetScale ();
    /*inline*/ float GetRotation ();
	Zilch::Real3 Transform::ZGetTranslation();
	Zilch::Real3 Transform::ZGetScale();
	void Transform::ZSetTranslation(Zilch::Real3 newpos);
	void Transform::ZSetScale(Zilch::Real3 newscale);
	Zilch::Real2 ZGetScreenPosition(Zilch::Real2);
    vec2 GetScreenPosition ();
    glm::vec2 GetScreenPosition (const glm::vec2& v);

    static void Print (vec3 position);

    // The non-base component usees DefineComponentName macro to name component
    const static string Name;


  private:
    int currentMatrix;

    glm::mat4 modelMatrix;
    glm::mat4 modelViewProjectionMatrix;
    vec3 position;
    vec3 scale;
    float rotation;
    glm::mat3 normalMatrix;

    // To avoid Unnecesary calculation in Update Matrices
    bool matricesReady;
  };
}

#endif
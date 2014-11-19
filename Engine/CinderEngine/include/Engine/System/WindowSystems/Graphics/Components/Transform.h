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
  enum MatrixModes
  {
    MODEL_MATRIX = 0,
    VIEW_MATRIX,
    PROJECTION_MATRIX
  };


  class Transform : public Component
  {
  public:
    ZilchDeclareBaseType(Transform, Zilch::TypeCopyMode::ReferenceType);
    
    Transform () {}
    virtual ~Transform();
    virtual void Initialize ();
    virtual void Serialize (Serializer::DataNode* data);
    //////////////////////////////////////////////////////////////////////////
    // Transformations
    void Load_Identity ();
    void Translate (float x, float y, float z);
	void Translate(vec3 &v);
    void Scale (float x, float y, float z);
    void Scale (float v);
    void Rotate (float angle);
    //////////////////////////////////////////////////////////////////////////
    bool MatrixMode (int m);
    void UpdateMatrices ();

    //getters
    glm::mat4 GetModelMatrix ();
    glm::mat4 GetModelViewProjectionMatrix ();
    inline vec3 GetPosition ();
    inline vec3 GetScale ();
    inline float GetRotation ();
    vec2 GetScreenPosition ();
    static void Print (vec3 position);

    // The non-base component usees DefineComponentName macro to name component
    const static string Name;



    

  private:
    int currentMatrix;
    //Matrix Stack
    // Useful When Using Parent-Child Structure For Complex Objects
    // Eg. Robot, Objects with many parts as children
    std::vector <glm::mat4> modelMatrix;
    std::vector <glm::mat4> modelViewProjectionmatrix;
    void push_matrix ();
    void pop_matrix ();
    vec3 position;
    vec3 scale;
    float rotation;
    glm::mat3 normalMatrix;

    // To avoid Unnecesary calculation in Update Matrices
    bool matricesReady;
  };

  inline vec3 Transform::GetPosition ()
  {
    return position;
  }

  inline vec3 Transform::GetScale ()
  {
    return scale;
  }

  inline float Transform::GetRotation ()
  {
    return rotation;
  }
}

#endif
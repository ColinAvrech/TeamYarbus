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

#include "MetaTypeDB.h"
#include "MetaCreator.h"

namespace Framework
{

  class Transform : public Component
  {
  public:
	  META_DECLARE( Transform );
    ZilchDeclareDerivedType(Transform, Component);
    
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
    void Rotate (float angle);
    //////////////////////////////////////////////////////////////////////////
    bool MatrixMode (int m);
    void UpdateMatrices ();
    void SetMVP(const glm::mat4 &mvp);

    //getters
    glm::mat4 GetModelMatrix ();
    glm::mat4 GetModelViewProjectionMatrix ();
    /*inline*/ vec3 GetPosition ();
    /*inline*/ vec3 GetScale ();
    /*inline*/ float GetRotation ();
	vec3 GetLocalPosition();
	vec3 GetLocalScale();
	Real3 ZGetLocalPosition();
	Real3 ZGetLocalScale();
	float GetLocalRotation();
    vec2 GetNDCPosition ();
    glm::vec2 GetGridPosition ();
    glm::vec2 GetGridPosition (glm::vec2 pos);
    glm::vec2 GetNDCPosition (const glm::vec2& v);
    glm::vec2 GetScreenPosition ();

    void SetPosition (float x, float y);

	//void UpdateChildren();
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // ZILCH FUNCTIONS
    Zilch::Real3 Transform::ZGetTranslation ();
    Zilch::Real3 Transform::ZGetScale ();
    void Transform::ZSetTranslation (Zilch::Real3 newpos);
    void Transform::ZSetScale (Zilch::Real3 newscale);
    Zilch::Real2 ZGetScreenPosition (Zilch::Real2);
    //////////////////////////////////////////////////////////////////////////

    static void Print (vec3 position);

    // The non-base component usees DefineComponentName macro to name component
    const static string Name;
	
	bool matricesReady;

  private:
    int currentMatrix;

	//These functions assume that there is a parent.
	void UpdatePosition(Transform* trans);
	void UpdateScale(Transform* trans);
	void UpdateRotation(Transform* trans);

    glm::mat4 modelMatrix;
    glm::mat4 modelViewProjectionMatrix;
    vec3 position;
    vec3 scale;
    float rotation;

	vec3 localPosition;
	vec3 localScale;
	float localRotation;
    glm::mat3 normalMatrix;

    // To avoid Unnecesary calculation in Update Matrices
    
  };

}

META_DEFINE( Framework::Transform, Transform )
{
	MEMBER( localPosition );
	MEMBER( position );
	MEMBER( scale );
	MEMBER( rotation );
	MEMBER( localRotation );
	MEMBER( localScale );
}


#endif
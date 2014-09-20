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

namespace Framework
{
  enum MatrixModes
  {
    MODEL_MATRIX = 0,
    VIEW_MATRIX,
    PROJECTION_MATRIX
  };

  class Transform : public GameComponent
  {
  public:
    Transform (GameObject* obj);
    ~Transform ();


    void Load_Identity ();

    // Type of Projection to Be Used
    // Mostly Orthographic
    void Ortho (float left, float right, float bottom, float top, float near, float far);
    void Perspective (float angle, float aRatio, float near, float far);

    // Replace the Fixed Functionality glTranslatef, glScalef,...
    void Translate (float x, float y, float z);
    void Scale (float x, float y, float z);
    void Scale (float v);
    void Rotate (float angle);

    //getters
    glm::mat4& GetModelMatrix ();
    glm::mat4& GetViewMatrix ();
    glm::mat4& GetModelViewMatrix ();
    glm::mat4& GetProjectionMatrix ();
    glm::mat4& GetModelViewProjectionMatrix ();
    glm::vec3 GetPosition ();
    glm::vec3 GetScale ();

    bool MatrixMode (int m);
  
    //GLSL
    void UpdateMatrices (GLuint programId);


    // The non-base component usees DefineComponentName macro to name component
    const static std::string Name;
  private:
    int currentMatrix;
    //Matrix Stack
    // Useful When Using Parent-Child Structure For Complex Objects
    // Eg. Robot, Objects with many parts as children
    std::vector<glm::mat4> modelMatrix;
    std::vector<glm::mat4> viewMatrix;
    std::vector<glm::mat4> projectionMatrix;

    // Replace the Fixed Functionality glPushMatrix, glPopMatrix
    void push_matrix ();
    void pop_matrix ();

    // To avoid Unnecesary calculation in Update Matrices
    bool matricesReady;
    glm::vec3 position;
    glm::vec3 scale;
    float rotation;
    // This Data Will Be Calculate By CPU instead of GPU
    glm::mat4 modelViewMatrix;
    glm::mat4 modelViewProjectionMatrix;
    glm::mat3 normalMatrix;

  };
}

#endif
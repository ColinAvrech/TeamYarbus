/******************************************************************************/
/*!
\file   Pipeline.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _PIPELINE_H
#define _PIPELINE_H

#include "GraphicsCommon.h"

namespace Framework
{
  class Transform;
  class IGraphicsObject;
  class Camera;

  enum MATRIX_MODE
  {
    MODEL = 0,
    VIEW,
    PROJECTION,
  };

  class Pipeline
  {
  public:
    Pipeline ();
    ~Pipeline ();

    void LoadIdentity ();
    void MatrixMode (int mode);
    void Translatef (float x, float y, float z);
    void Translatefv (float* v);
    void Scalef (float x, float y, float z);
    void Scalef (float s);
    void Scalefv (float* v);
    void Rotatef (float z);
    void Rotatefv (float* v);
    void Ortho (float left, float right, float bottom, float top, float near = 0.0f, float farPlane = 1.0f);
    void Perspective (float fov, float aspect, float nearPlane, float farPlane);
    void LookAt (glm::vec3 eye, glm::vec3 center, glm::vec3 up);
    void UpdateMatrices ();
    void UploadMatrices (Shader* shader);
    void Update ();
    void Draw ();
    void PushMatrix ();
    void PopMatrix ();

    int GetMatrixMode ();
    glm::mat4 GetModelMatrix ();
    glm::mat4 GetViewMatrix ();
    glm::mat4 GetProjectionMatrix ();
    glm::mat4 GetModelViewMatrix ();
    glm::mat4 GetModelViewProjectionMatrix ();
  
    static std::list <Transform*> transforms;
    static std::list <IGraphicsObject*> graphicsObjects;
    static Camera* camera;

  private:
    std::vector <glm::mat4> modelMatrix;
    std::vector <glm::mat4> viewMatrix;
    std::vector <glm::mat4> projectionMatrix;

    glm::mat4 modelViewMatrix;
    glm::mat4 viewProjectionMatrix;
    glm::mat4 modelViewProjectionMatrix;

    int currentMatrix;
    bool matricesReady;
  };

  extern Pipeline* OPENGL;
}

#endif
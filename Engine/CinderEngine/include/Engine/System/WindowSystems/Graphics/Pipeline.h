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

#include "glew.h"
#include "CinderMath.h"
#include "FrameBufferObject.h"
#include <vector>
#include <unordered_map>
#include <list>

namespace Framework
{
  class Transform;
  class IGraphicsObject;
  class Camera;
  class UIComponent;
  class ShapeCollider;
  class Shader;
  class PointLight;
  class PauseEvent;
  class GUIText;

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
	
	Shader* sceneShader;

    void LoadIdentity ();
    void MatrixMode (int mode);
    void Translatef (float x, float y, float z);
    void Translatefv (float* v);
    void Scalef (float x, float y, float z);
    void Scalef (float s);
    void Scalefv (float* v);
    void Rotatef (float r, float x, float y, float z);
    void Rotatefv (float r, float* v);
    void Ortho (float left, float right, float bottom, float top, float near = 0.0f, float farPlane = 1.0f);
    void Perspective (float fov, float aspect, float nearPlane, float farPlane);
    void LookAt (glm::vec3 eye, glm::vec3 center, glm::vec3 up);
    void UpdateMatrices ();
    void UploadMatrices (Shader* shader);
    void Update ();
    void Draw ();
    void ResetBlendMode ();
    void PushMatrix ();
    void PopMatrix ();
    void ToggleDebugDraw();

    void ResizeBuffer (const int w, const int h);
    //void OnApplicationPause (UpdateEvent* pause);

    int GetMatrixMode ();
    glm::mat4 GetModelMatrix ();
    glm::mat4 GetViewMatrix ();
    glm::mat4 GetProjectionMatrix ();
    glm::mat4 GetModelViewMatrix ();
    glm::mat4 GetModelViewProjectionMatrix ();
  
    static std::list <Transform*> transforms;
    static std::unordered_map <int, std::list <IGraphicsObject*>> graphicsObjects;
    static std::list <UIComponent*> uiObjects;
    static std::list <Camera*> cameras;
    static std::list <ShapeCollider*> debugColliders;
    static std::list <GUIText*> textObjects;
    static std::list <PointLight*> pointLights;

  private:
    std::vector <glm::mat4> modelMatrix;
    std::vector <glm::mat4> viewMatrix;
    std::vector <glm::mat4> projectionMatrix;

    glm::mat4 modelViewMatrix;
    glm::mat4 viewProjectionMatrix;
    glm::mat4 modelViewProjectionMatrix;

    GLenum sFactor;
    GLenum dFactor;
    int currentMatrix;
    bool matricesReady;

    void RenderToTexture (FBO* fbo, GLuint tex, Shader* shader);

    bool useDebugDraw;
  };

  extern Pipeline* OPENGL;
}

#endif
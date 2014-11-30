/******************************************************************************/
/*!
\file   Pipeline.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "Pipeline.h"
#include "Transform.h"
#include "IGraphicsObject.h"
#include "Camera.h"
#include "ShapeCollider.h"
#include "CinderEngine_UI.h"
#include "PhysicsSystemNew.h"
#include "RigidBody2D.h"
#include "Collider2D.h"
#include "MathExtensionLibrary.h"
#include "WindowSystem.h"
#include "PointLight.h"
#include "FrameBufferObject.h"
#include "ResourceManager.h"
#include "glut.h"
#include "Thermodynamics.h"
#include "HeatMap.h"

namespace Framework
{
  using namespace Physics;
  enum COLOR_STATE
  {
    IDLE,
    INTERPOLATE,
  };

  COLOR_STATE cState = IDLE;

  static float t = 0.0f;
  glm::vec4 startColor, endColor;
  glm::vec4 color;
  VAO* vao;
  VBO* vbo;
  FBO* fbo;
  Shader* sceneShader;
  GLuint renderTexture;

  void Interpolate_Background ()
  {
    switch (cState)
    {
    case Framework::IDLE:
      t += 0.016f;
      if (t > 1.0f)
      {
        cState = INTERPOLATE;
        t = 0.0f;
        startColor = color;
        endColor = glm::linearRand (glm::vec4 (0, 0, 0, 0), glm::vec4 (1.0f, 1.0f, 1.0f, 1.0f));
      }
      break;
    case Framework::INTERPOLATE:
      t += 0.016f;
      color = glm::mix (startColor, endColor, t * 5);
      if (t > 0.2f)
      {
        t = 0.0f;
        cState = IDLE;
      }
      break;
    default:
      break;
    }
  }

  //! Global pointer to  the Pipeline.
  Pipeline* OPENGL = NULL;

  std::list <Transform*> Pipeline::transforms;
  std::list <IGraphicsObject*> Pipeline::graphicsObjects;
  std::list <UIComponent*> Pipeline::uiObjects;
  std::list <Camera*> Pipeline::cameras;
  std::list <PointLight*> Pipeline::pointLights;
  std::list <ShapeCollider*> Pipeline::debugColliders;

  Pipeline::Pipeline ()
  {
    OPENGL = this;
    modelMatrix.push_back (glm::mat4 (1.0));
    viewMatrix.push_back (glm::mat4 (1.0));
    projectionMatrix.push_back (glm::mat4 (1.0));

    modelViewMatrix = glm::mat4 (1.0);
    modelViewProjectionMatrix = glm::mat4 (1.0);
    matricesReady = true;
    currentMatrix = 0;

    //RigidBody2D* b;
    ////CircleCollider2D c (1.0f);
    ////b = PHYSICS->Add (&c, 2, 1);

    //PolygonCollider2D poly;
    //poly.SetBox (0.5f, 0.5f);
    //b = PHYSICS->Add (&poly, -9, 0);
    //b->SetStatic ();
    //b->SetOrient (0);

    //PolygonCollider2D poly1;
    //poly1.SetBox (0.5f, 0.5f);
    //b = PHYSICS->Add (&poly1, 15, -2);
    //b->SetStatic ();
    //b->SetOrient (0);
    //b->dynamicFriction = 0.0f;
    //b->staticFriction = 0.0f;

    //GLfloat vertices [] =
    //{
    //  -1.0f, 1.0f, 0.0f, 1.0f,
    //    1.0f, 1.0f, 1.0f, 1.0f,
    //    1.0f, -1.0f, 1.0f, 0.0f,

    //    1.0f, -1.0f, 1.0f, 0.0f,
    //    -1.0f, -1.0f, 0.0f, 0.0f,
    //    -1.0f, 1.0f, 0.0f, 1.0f
    //};

    //sceneShader = Resources::RS->Get_Shader ("PointLight");

    //vao = new VAO ();
    //vbo = new VBO (sizeof(vertices), vertices);

    //sceneShader->Use ();
    //GLint posAttrib = sceneShader->attribLocation ("position");
    //sceneShader->enableVertexAttribArray (posAttrib);
    //sceneShader->vertexAttribPtr (posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof GLfloat, 0);
    //GLint texAttrib = sceneShader->attribLocation ("texcoord");
    //sceneShader->enableVertexAttribArray (texAttrib);
    //sceneShader->vertexAttribPtr (texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof GLfloat, 2 * sizeof GLfloat);
    //sceneShader->Disable ();
    //vao->unbindVAO ();

    //fbo = new FBO ();
    //glGenTextures (1, &renderTexture);
    //glBindTexture (GL_TEXTURE_2D, renderTexture);
    //glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
    //glBindTexture (GL_TEXTURE_2D, 0);
    //fbo->unBind ();
  }

  Pipeline::~Pipeline ()
  {
    OPENGL = nullptr;
  }

  void Pipeline::LoadIdentity ()
  {
    if (currentMatrix == MODEL || currentMatrix == VIEW)
    {
      modelMatrix [modelMatrix.size () - 1] = glm::mat4 (1.0);
      viewMatrix [modelMatrix.size () - 1] = glm::mat4 (1.0);
    }
    else
    {
      projectionMatrix [viewMatrix.size () - 1] = glm::mat4 (1.0);
    }

    matricesReady = false;
  }

  void Pipeline::MatrixMode (int mode)
  {
    if (mode == MODEL || mode == VIEW || mode == PROJECTION)
    {
      currentMatrix = mode;
    }
  }

  void Pipeline::Translatef (float x, float y, float z)
  {
    if (currentMatrix == MODEL)
    {
      modelMatrix [modelMatrix.size () - 1] *= glm::translate (glm::vec3 (x, y, z));
    }
    else if (currentMatrix == VIEW)
    {
      viewMatrix [viewMatrix.size () - 1] *= glm::translate (glm::vec3 (-x, -y, -z));
    }

    matricesReady = false;
  }

  void Pipeline::Translatefv (float* v)
  {
    if (currentMatrix == MODEL)
    {
      modelMatrix [modelMatrix.size () - 1] *= glm::translate (glm::vec3 (v [0], v [1], v [2]));
    }
    else if (currentMatrix == VIEW)
    {
      viewMatrix [viewMatrix.size () - 1] *= glm::translate (glm::vec3 (-v [0], -v [1], -v [2]));
    }

    matricesReady = false;
  }

  void Pipeline::Scalef (float x, float y, float z)
  {
    if (currentMatrix == MODEL)
    {
      modelMatrix [modelMatrix.size () - 1] *= glm::scale (glm::vec3 (x, y, z));
    }
    else if (currentMatrix == VIEW)
    {
      viewMatrix [viewMatrix.size () - 1] *= glm::scale (glm::vec3 (x, y, z));
    }

    matricesReady = false;
  }

  void Pipeline::Scalef (float s)
  {
    if (currentMatrix == MODEL)
    {
      modelMatrix [modelMatrix.size () - 1] *= glm::scale (glm::vec3 (s, s, s));
    }
    else if (currentMatrix == VIEW)
    {
      viewMatrix [viewMatrix.size () - 1] *= glm::scale (glm::vec3 (s, s, s));
    }

    matricesReady = false;
  }

  void Pipeline::Scalefv (float* v)
  {
    if (currentMatrix == MODEL)
    {
      modelMatrix [modelMatrix.size () - 1] *= glm::scale (glm::vec3 (v [0], v [1], v [2]));
    }
    else if (currentMatrix == VIEW)
    {
      viewMatrix [viewMatrix.size () - 1] *= glm::scale (glm::vec3 (v [0], v [1], v [2]));
    }

    matricesReady = false;
  }

  void Pipeline::Rotatef (float r, float x, float y, float z)
  {
    if (currentMatrix == MODEL)
    {
      modelMatrix [modelMatrix.size () - 1] *= glm::rotate (r * M_PI / 180, glm::vec3 (x, y, z));
    }
    else if (currentMatrix == VIEW)
    {
      viewMatrix [viewMatrix.size () - 1] *= glm::rotate (r * M_PI / 180, glm::vec3 (x, y, z));
    }

    matricesReady = false;
  }

  void Pipeline::LookAt (glm::vec3 eye, glm::vec3 center, glm::vec3 up)
  {
    viewMatrix [viewMatrix.size () - 1] = glm::lookAt (eye, center, up);
  }

  void Pipeline::Ortho (float left, float right, float bottom, float top, float nearPlane /* = 0.0f */, float farPlane /* = 1.0f */)
  {
    projectionMatrix [projectionMatrix.size () - 1] = glm::ortho (left, right, bottom, top, nearPlane, farPlane);
    matricesReady = false;
  }

  void Pipeline::Perspective (float fov, float aspect, float nearPlane, float farPlane)
  {
    projectionMatrix [projectionMatrix.size () - 1] = glm::perspective (fov * M_PI / 180, aspect, nearPlane, farPlane);
    matricesReady = false;
  }

  static void Draw_Quad ()
  {
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    glBegin (GL_QUADS);
    glColor4d (0.2f, 0.2f, 0.5, 0.8f);
    glVertex2f (-1, -1);
    glVertex2f (1, -1);
    glColor4d (0, 0, 0.3, 0.5f);
    glVertex2f (1, 1);
    glVertex2f (-1, 1);
    glEnd ();
  }

  void Pipeline::Update ()
  {
    //glBindTexture (GL_TEXTURE_2D, renderTexture);
    //glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, WINDOWSYSTEM->Get_Width (), WINDOWSYSTEM->Get_Height (), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    //fbo->bind ();

//    Interpolate_Background ();
//
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor (color.r, color.g, color.b, 1.0f);
    glEnable (GL_BLEND);
    sFactor = GL_SRC_ALPHA;
    dFactor = GL_ONE_MINUS_SRC_ALPHA;
    glBlendFunc (sFactor, dFactor);

    for (auto* i : transforms)
    {
      i->UpdateMatrices ();
    }

    for (auto* i : cameras)
    {
      i->UpdateCamera (this);
    }

    //Draw_Quad ();
    for (auto* i : graphicsObjects)
    {
      i->Update ();
      i->Draw ();
    }

    for (auto* i : uiObjects)
    {
      i->UIDraw ();
    }

    //sFactor = GL_ONE;
    //dFactor = GL_ONE;
    //glBlendFunc (sFactor, dFactor);

    //vao->bindVAO ();
    //fbo->unBind ();
    //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //sceneShader->Use ();
    //glBindTexture (GL_TEXTURE_2D, renderTexture);

    //for (auto& it : pointLights)
    //{
    //  it->Draw ();
    //  glDrawArrays (GL_TRIANGLES, 0, 6);
    //}
    //sceneShader->Disable ();
    //glBindTexture (GL_TEXTURE_2D, 0);

#ifdef _DEBUG
    THERMODYNAMICS->Draw ();
    //PHYSICS->Render ();
#endif
  }

  void Pipeline::UpdateMatrices ()
  {
    if (!matricesReady)
    {
      modelViewMatrix = viewMatrix [viewMatrix.size () - 1] * modelMatrix [modelMatrix.size () - 1];
      modelViewProjectionMatrix = projectionMatrix [projectionMatrix.size () - 1] *
        viewMatrix [viewMatrix.size () - 1] * modelMatrix [modelMatrix.size () - 1];
    }
  }

  void Pipeline::UploadMatrices (Shader* shader)
  {
    shader->Use ();
    shader->uniMat4 ("modelMatrix",     glm::value_ptr (modelMatrix [modelMatrix.size () - 1]));
    shader->uniMat4 ("viewMatrix",      glm::value_ptr (viewMatrix [viewMatrix.size () - 1]));
    shader->uniMat4 ("modelViewMatrix", glm::value_ptr (modelViewMatrix));
    shader->uniMat4 ("mvp",             glm::value_ptr (modelViewProjectionMatrix));
    shader->Disable ();
  }

  void Pipeline::PushMatrix ()
  {
    if (currentMatrix == MODEL)
    {
      modelMatrix.push_back (modelMatrix [modelMatrix.size () - 1]);
    }
    else if (currentMatrix == VIEW)
    {
      viewMatrix.push_back (modelMatrix [modelMatrix.size () - 1]);
    }
    else if (currentMatrix == PROJECTION)
    {
      projectionMatrix.push_back (viewMatrix [viewMatrix.size () - 1]);
    }

    matricesReady = false;
  }

  void Pipeline::PopMatrix ()
  {
    if (currentMatrix == MODEL)
    {
      if (modelMatrix.size () > 1)
      {
        modelMatrix.pop_back ();
      }
    }
    else if (currentMatrix == VIEW)
    {
      if (viewMatrix.size () > 1)
      {
        viewMatrix.pop_back ();
      }
    }
    else if (currentMatrix == PROJECTION)
    {
      if (projectionMatrix.size () > 1)
      {
        projectionMatrix.pop_back ();
      }
    }

    matricesReady = false;
  }

  int Pipeline::GetMatrixMode ()
  {
    return currentMatrix;
  }

  glm::mat4 Pipeline::GetModelMatrix ()
  {
    return modelMatrix [modelMatrix.size () - 1];
  }

  glm::mat4 Pipeline::GetViewMatrix ()
  {
    return viewMatrix [viewMatrix.size () - 1];
  }

  glm::mat4 Pipeline::GetProjectionMatrix ()
  {
    return projectionMatrix [projectionMatrix.size () - 1];
  }

  glm::mat4 Pipeline::GetModelViewMatrix ()
  {
    if (!matricesReady)
    {
      return viewMatrix [viewMatrix.size () - 1] * modelMatrix [modelMatrix.size () - 1];
    }
    else
    {
      return modelViewMatrix;
    }
  }

  glm::mat4 Pipeline::GetModelViewProjectionMatrix ()
  {
    if (!matricesReady)
    {
      return Camera::GetViewToProjectionMatrix () *
        Camera::GetWorldToViewMatrix () *
        modelMatrix [modelMatrix.size () - 1];
    }

    return modelViewProjectionMatrix;
  }

  void Pipeline::ResetBlendMode ()
  {
    glBlendFunc (sFactor, dFactor);
  }

}
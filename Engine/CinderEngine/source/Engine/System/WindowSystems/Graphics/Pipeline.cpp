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

namespace Framework
{
  //! Global pointer to  the windows system.
  Pipeline* OPENGL = NULL;

  std::list <Transform*> Pipeline::transforms;
  std::list <IGraphicsObject*> Pipeline::graphicsObjects;
  Camera* Pipeline::camera;

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
    glColor4d (1, 1, 1, 0.8f);
    glVertex2f (-1, -1);
    glVertex2f (1, -1);
    glColor4d (1, 1, 0.7, 0.5f);
    glVertex2f (1, 1);
    glVertex2f (-1, 1);
    glEnd ();
  }

  void Pipeline::Update ()
  {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor (0, 0, 0, 0);
    glEnable (GL_BLEND);
    sFactor = GL_SRC_ALPHA;
    dFactor = GL_ONE_MINUS_SRC_ALPHA;
    glBlendFunc (sFactor, dFactor);

    for (auto* i : transforms)
    {
      i->UpdateMatrices ();
    }

    camera->UpdateCamera (this);

    Draw_Quad ();

    for (auto* i : graphicsObjects)
    {
      i->Update ();
      i->Draw ();
    }

    //for (auto* i : uiObjects)
    //{
    //  i->UIDraw ();
    //}

    //for (auto* i : debugColliders)
    //{
    //  i->Draw ();
    //}
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
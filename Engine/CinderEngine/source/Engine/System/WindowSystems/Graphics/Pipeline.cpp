/******************************************************************************/
/*!
\file   Pipeline.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>


namespace Framework
{
  using namespace Physics;

  static float t = 0.0f;
  VAO* vao;
  VBO* vbo;
  FBO* fbo;
  FBO* dsfbo;
  //FBO* efbo;
  //moved scene shader to Pipeline.cpp
  GLuint sourceTexture;     //offscreen texture
  GLuint renderTexture;     //final texture
  GLuint dsTexture;         //downsampled texture for post processing
  GLuint depthrenderbuffer; //depth buffer

  //! Global pointer to  the Pipeline.
  Pipeline* OPENGL = NULL;

  std::list <Transform*> Pipeline::transforms;
  std::unordered_map <int, std::list <IGraphicsObject*>> Pipeline::graphicsObjects;
  std::list <UIComponent*> Pipeline::uiObjects;
  std::list <Camera*> Pipeline::cameras;
  std::list <PointLight*> Pipeline::pointLights;
  std::list <GUIText*> Pipeline::textObjects;
  std::list <ShapeCollider*> Pipeline::debugColliders;

  //Shadertoy test=================================
  Shader *rainshader;
  static float timer = 0.f;
  //===============================================
  //Post process test==============================
  Shader *bnw;
  //===============================================

  Pipeline::Pipeline ()
  {
    int W = WINDOWSYSTEM->Get_Width();
    int H = WINDOWSYSTEM->Get_Height();
    glViewport (0, 0, WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height());
    useDebugDraw = false;
    OPENGL = this;
    modelMatrix.push_back (glm::mat4 (1.0));
    viewMatrix.push_back (glm::mat4 (1.0));
    projectionMatrix.push_back (glm::mat4 (1.0));

    modelViewMatrix = glm::mat4 (1.0);
    modelViewProjectionMatrix = glm::mat4 (1.0);
    matricesReady = true;
    currentMatrix = 0;

    //EVENTSYSTEM->mConnect<UpdateEvent, Pipeline> (Events::UPDATEEVENT, this, &Pipeline::OnApplicationPause);

    GLfloat vertices [] =
    {
      -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f
    };

    //Shadertoy test===============================================
    rainshader = Resources::RS->Get_Shader("Storm");
    //=============================================================
    //Post process test============================================
    bnw = Resources::RS->Get_Shader("GrayScale");
    //=============================================================
    Change_Shader ("Passthrough", (int) SS_DEFAULT);

    vao = new VAO ();
    vbo = new VBO (sizeof(vertices), vertices);

    sceneShader->Use ();
    GLint posAttrib = sceneShader->attribLocation ("position");
    sceneShader->enableVertexAttribArray (posAttrib);
    sceneShader->vertexAttribPtr (posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof GLfloat, 0);
    GLint texAttrib = sceneShader->attribLocation ("texcoord");
    sceneShader->enableVertexAttribArray (texAttrib);
    sceneShader->vertexAttribPtr (texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof GLfloat, 2 * sizeof GLfloat);
    sceneShader->Disable ();
    vao->unbindVAO ();

    fbo = new FBO ();
    //MSAA test
    //////////////////////////////
    renderTexture = GenerateTextureMultiSampled(W, H); //primary texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, renderTexture, 0);
    // Generate a depth buffer
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, W, H);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    fbo->unBind();
    ////////////////////////////////////////////////////
    //Test offscreen rendering
    //=================================================
    dsfbo = new FBO();
    sourceTexture = GenerateTexture(W, H); //post process texture
    dsTexture = GenerateTexture(W, H);     //downsampled version of original texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dsTexture, 0);
    //===================================================
    
    glEnable(GL_MULTISAMPLE);
  }

  Pipeline::~Pipeline ()
  {
    OPENGL = nullptr;
  }

  static void Draw_Quad ()
  {
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    glBegin (GL_QUADS);
    glColor4d (1, 1, 1, 1);
    glVertex2f (-1, -1);
    glVertex2f (1, -1);
    glColor4d (1, 1, 1, 1);
    glVertex2f (1, 1);
    glVertex2f (-1, 1);
    glEnd ();
  }

  void Pipeline::Update ()
  {
    //clear post process buffer//
    dsfbo->bind();
    glClearColor(0.f, 0.f, 0.f, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    /////////////////////////////

    //Start rendering scene/////
    fbo->bind();

    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClearDepth(1.0f);	// Set Depth Buffer to Max Dist. when cleared	
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
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

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // SCENE DRAW
    //////////////////////////////////////////////////////////////////////////
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it's closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Enable alpha blending for scene
    glEnable(GL_BLEND);
    
    //camera properties
    int n = 1;// Camera::main->GetElementNum();
    float aperture = Camera::main->GetAperture();

    //projection matrix
    glm::mat4 projection = glm::perspective
      (
      Camera::main->GetFOV() * M_PI / 180,
      (float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height(),
      Camera::main->GetPlanes().first,
      Camera::main->GetPlanes().second
      );
    MatrixMode(PROJECTION);
    LoadIdentity();
    Perspective(
      Camera::main->GetFOV(),
      (float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height(),
      Camera::main->GetPlanes().first,
      Camera::main->GetPlanes().second
      );
    
    glm::vec3 campos = Camera::main->gameObject->C<Transform>()->GetPosition();
    glm::vec3 up = glm::vec3(0.f, -1.f, 0.f);
    //eye and object
    glm::vec3 eye = campos;
    glm::vec3 object = campos + Camera::main->GetViewDirection() * Camera::main->FocalPoint();

    //right and up vectors
    glm::vec3 right = glm::normalize(glm::cross(object - eye, up));
    glm::vec3 p_up = glm::normalize(glm::cross(object - eye, right));
    //DOF loop
    for (int i = 0; i < n; ++i)
    {
      glm::vec3 bokeh = right * std::cosf(i * M_2_PI / n) + p_up * std::sinf(i * M_2_PI / n);
      MatrixMode(VIEW);
      LoadIdentity();
      LookAt(eye + aperture * bokeh, object, p_up);
      //Translatefv (&pos.x);
      Rotatef(Camera::main->gameObject->C<Transform>()->GetRotation(), 0, 0, 1);
      glm::mat4 modelview = GetViewMatrix ();
      MatrixMode(MODEL);
      LoadIdentity();


      for (auto* it : transforms)
      {
        glm::mat4 mat = projection * modelview;
        it->SetMVP(mat);
      }

      std::vector<IGraphicsObject *>last;
      for (auto* it : graphicsObjects[DEFAULT])
      {
        if (it->Draw_Last())
          last.push_back(it);
        else
        {
          it->Draw();
        }
      }
      //disable depth test
      glDisable(GL_DEPTH_TEST);
      //draw stuff that should be drawn last
      for (auto* it : last)
      {
        it->Draw();
      }
    } //end DOF loop
    //=======================================================================
    //Downsample texture
    //=======================================================================
    DownSample(fbo, dsfbo);
    //=======================================================================
    //Shadertoy test
    //=======================================================================
    //dsfbo->bind();
    //rainshader->Use();
    //glm::vec3 resolution(WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height(), 0.f);
    //rainshader->uni3fv("iResolution", glm::value_ptr(resolution));
    //timer += dt;
    //rainshader->uni1f("iGlobalTime", timer);
    ////draw quad with shadertoy over scene, currently results in a black screen.
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //=========================================================================

    ///////////////////////////////////////////////////////////////////////////
    //Do post processing here
    ///////////////////////////////////////////////////////////////////////////
    //RenderToTexture(fbo, renderTexture, sceneShader);
    dsfbo->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sourceTexture, 0);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, dsTexture);
    bnw->Use();
    bnw->uni1i("image", 0);
    //Draw quad
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ////Swap active textures
    SwapTextureIds(&dsTexture, &sourceTexture);
    bnw->Disable();
    ///////////////////////////////////////////////////////////////////////////
    //glDisable(GL_BLEND);
    //glBlendFunc(sFactor, dFactor);

    //for (auto* i : textObjects)
    //{
    //  i->Draw();
    //}

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // UI DRAW
    //////////////////////////////////////////////////////////////////////////

    for (auto* i : graphicsObjects [PAUSE])
    {
      i->Draw ();
    }

    for (auto* i : uiObjects)
    {
      i->UIDraw ();
    }


    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // DEBUG DRAW
    //////////////////////////////////////////////////////////////////////////
    if (useDebugDraw)
    {
      //glEnable(GL_BLEND);
      //THERMODYNAMICS->Draw ();
      PHYSICS->Render ();
      ResetBlendMode();
    }

    DownSample(dsfbo, 0);
    Panel::PanelManager::Draw( );
    glfwSwapBuffers(WINDOWSYSTEM->Get_Window());
  }

#pragma region TRANSFORMATIONS
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

#pragma endregion

  void Pipeline::ResetBlendMode ()
  {
    glBlendFunc (sFactor, dFactor);
  }

  bool Pipeline::IsDebugDrawEnabled() const
  {
    return useDebugDraw;
  }

  void Pipeline::ToggleDebugDraw ()
  {
    if (useDebugDraw)
    {
      std::cout << "Ending DebugDraw" << std::endl;
      useDebugDraw = false;
    }
    else
    {
      std::cout << "Starting DebugDraw" << std::endl;
      useDebugDraw = true;
    }
  }

  void Pipeline::RenderToTexture(FBO* source, GLuint tex, Shader* shader)
  {
    //Manas' code========================================
    /*vao->bindVAO();
    source->unBind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->Use();
    glBindTexture(GL_TEXTURE_2D, tex);

    switch (shaderState)
    {
    case Framework::SS_DEFAULT:
      ALPHA = 1.0f;
      shader->uni1i("image", 0);
      break;

    case Framework::SS_LIGHTING:
      ALPHA = 1.0f;
      shader->uni1i("image", 0);
      break;

    case Framework::SS_FADE_IN:
      ALPHA += 0.007f;
      if (ALPHA >= 1.0f)
      {
        ALPHA = 1.0f;
        Change_Shader("Passthrough", (int)SS_DEFAULT);
      }
      break;

    case Framework::SS_FADE_OUT:
      ALPHA -= 0.007f;
      if (ALPHA <= 0.0f)
      {
        ALPHA = 0.0f;
        Change_Shader("Passthrough", (int)SS_DEFAULT);
      }
      break;
    default:
      ALPHA = 1.0f;
      break;
    }
    shader->uni1i("image", 0);
    shader->uni1f("alpha", ALPHA);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    shader->Disable();
    glBindTexture(GL_TEXTURE_2D, 0);
    vao->unbindVAO();*/
    //thecppguy code==================================
    vao->bindVAO();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    shader->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    shader->uni1i("image", 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    shader->Disable();
    vao->unbindVAO();
  }

  void Pipeline::DownSample(FBO *source, FBO *dest)
  {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, source ? source->getBufferPos() : 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest ? dest->getBufferPos() : 0);
    glBlitFramebuffer(0, 0, WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height(),
      0, 0, WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
  }

  GLuint Pipeline::GenerateTextureMultiSampled(int _width, int _height, int samples)
  {
    GLuint iTexture;
    glGenTextures(1, &iTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, iTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 1920, 1080, GL_TRUE);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 1920, 1080);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    return iTexture;
  }

  GLuint Pipeline::GenerateTexture(int w, int h)
  {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //unbind and return
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
  }

  void Pipeline::SwapTextureIds(GLuint *tex1, GLuint *tex2)
  {
    GLuint temptex = *tex1;
    *tex1 = *tex2;
    *tex2 = temptex;
  }

  void Pipeline::ResizeBuffer (const int w, const int h)
  {
    glViewport (0, 0, w, h);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, w, h, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, dsTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindTexture(GL_TEXTURE_2D, sourceTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, w, h);

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void Pipeline::Change_Shader (std::string sh, int id /* = 0*/)
  {
    sceneShader = Resources::RS->Get_Shader (sh);
    shaderState = (SHADER_STATE)id;

    if (id == (int) SS_FADE_IN)
      ALPHA = 0.0f;
    else if (id == (int) SS_FADE_OUT)
      ALPHA = 1.0f;
  }


}

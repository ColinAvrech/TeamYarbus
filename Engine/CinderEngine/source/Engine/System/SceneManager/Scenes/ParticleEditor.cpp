/******************************************************************************/
/*!
\file   ParticleEditor.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "ParticleEditor.h"
#include "ComponentInclude.h"
#include "ResourceManager.h"
#include "Particles.h"
#include "ParticleUpdaters.h"
#include "particleGenerators.h"
#include "ParticleRenderer.h"
#include "TimeQuery.h"
#include "Effect.h"
#include "AntTweakBar.h"
#include "WindowSystem.h"

typedef CTwBar TwBar;
TwBar* MainTweakBar;

template <typename T> struct TypeToTW
{
  static const TwType value{ TW_TYPE_UNDEF };
};

template <>	struct TypeToTW<int> { static const TwType value{ TW_TYPE_INT32 }; };
template <>	struct TypeToTW<bool> { static const TwType value{ TW_TYPE_BOOLCPP }; };
template <>	struct TypeToTW<float> { static const TwType value{ TW_TYPE_FLOAT }; };
template <>	struct TypeToTW<double> { static const TwType value{ TW_TYPE_DOUBLE }; };

template <typename T> void AddTweak (const char *name, T *var, const char *def)
{
  TwAddVarRW (MainTweakBar, name, TypeToTW<T>::value, var, def);
}

inline void AddTweakColor3f (const char *name, float *col, const char *def)
{
  TwAddVarRW (MainTweakBar, name, TW_TYPE_COLOR3F, col, def);
}

inline void AddTweakColor4f (const char *name, float *col, const char *def)
{
  TwAddVarRW (MainTweakBar, name, TW_TYPE_COLOR4F, col, def);
}

inline void AddTweakDir3f (const char *name, float *dir, const char *def)
{
  TwAddVarRW (MainTweakBar, name, TW_TYPE_DIR3F, dir, def);
}

template <typename T> void AddVar (const char *name, T *var, const char *def)
{
  TwAddVarRO (MainTweakBar, name, TypeToTW<T>::value, var, def);
}

inline void RemoveVar (const char *name)
{
  TwRemoveVar (MainTweakBar, name);
}

inline void RemoveAllVars ()
{
  TwRemoveAllVars (MainTweakBar);
}

inline void AddSeparator ()
{
  TwAddSeparator (MainTweakBar, "sep", "");
}


namespace Framework
{
  std::shared_ptr<IEffect> gEffects [3];
  IEffect *gCurrentEffect = nullptr;
  int gCurrentEffectID = 0;
  int gSelectedEffect = 0;
  int gNumParticles = 0;
  int gNumAlive = 0;

  unsigned int gParticleTexture = 0;
  Shader* mProgram;
  struct CAMERA
  {
    float camDistance;
    glm::vec3 cameraDir;
    glm::mat4 modelviewMatrix;
    glm::mat4 projectionMatrix;
  } camera_;


  Camera camera1 (NULL, true);
  CpuTimeQuery cpuParticlesUpdate;
  CpuTimeQuery cpuBuffersUpdate;
  GpuTimerQuery gpuUpdate;
  double gpuUpdateTime = 0;
  GpuTimerQuery gpuRender;
  double gpuRenderTime = 0;

  bool gAnimationOn = true;

  // Constructor
  ParticleEditor::ParticleEditor ()
  {
  }

  // Destructor
  ParticleEditor::~ParticleEditor ()
  {
    gCurrentEffect->clean ();
    glDeleteTextures (1, &gParticleTexture);
  }


  void ParticleEditor::Key_Pressed(int key, int scanCode, int action, int mods)
  {
  }


  void ParticleEditor::Mouse_Button(int button, int state, int mods)
{
  }


  void ParticleEditor::Mouse_Moved(double xPos, double yPos)
{
  }


  void ParticleEditor::Load_Scene (const char* filename)
  {
    Change_Size (WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height());
    //
    // some global GL states
    //
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.f, 0.f, 0.f, 0.0f);

    //camera_.cameraDir [0] = 0.0f;
    //camera_.cameraDir [1] = 0.0f;
    //camera_.cameraDir [2] = 1.0f;
    //camera_.camDistance = 0.5f;

    gParticleTexture = Resources::RS->Get_Texture ("Particle2.png")->Get_ID ();
    mProgram = Resources::RS->Get_Shader ("Particle");
    mProgram->Use ();
    mProgram->uni1i ("tex", 0);
    mProgram->Disable ();


    gEffects [0] = EffectFactory::create ("attractors");
    gEffects [0]->initialize (IEffect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffects [0]->initializeRenderer ();
    gEffects [1] = EffectFactory::create ("attractors");
    gEffects [1]->initialize (IEffect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffects [1]->initializeRenderer ();
    gEffects [2] = EffectFactory::create ("fountain");
    gEffects [2]->initialize (IEffect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffects [2]->initializeRenderer ();
    gCurrentEffectID = 0;
    gCurrentEffect = gEffects [0].get ();

    gpuUpdate.init ();
    gpuRender.init ();

    //AddTweak ("animate", &gAnimationOn, "");
    //AddVar ("particles", &gNumParticles, "");
    //AddVar ("alive", &gNumAlive, "");
    //AddVar ("cpu particles", &cpuParticlesUpdate.m_time, "precision=3 group=timers");
    //AddVar ("cpu buffers", &cpuBuffersUpdate.m_time, "precision=3 group=timers");

    //AddVar ("gpu buffer", &gpuUpdate.getTime (), "precision=3 group=timers");
    //AddVar ("gpu render", &gpuRender.getTime (), "precision=3 group=timers");

    //AddTweakDir3f ("camera", &camera_.cameraDir.x, "");
    //AddTweak ("camera distance", &camera_.camDistance, "min=0.05 max=4.0 step=0.01");
    //AddSeparator ();
    //AddTweak ("effect id", &gSelectedEffect, "min=0 max=2");

    Camera::main->worldToView = glm::lookAt (Camera::main->viewDirection * 0.5f, Camera::main->position, Camera::main->up);
  }


  void ParticleEditor::Update (const double dt)
  {
    const double deltaTime = 0.016;
    if (!gAnimationOn)
      return;

    if (gSelectedEffect != gCurrentEffectID)
    {
      gCurrentEffect->removeUI (MainTweakBar);
      gCurrentEffectID = gSelectedEffect;
      gCurrentEffect = gEffects [gCurrentEffectID].get ();
      gCurrentEffect->addUI (MainTweakBar);
    }

    gCurrentEffect->update (deltaTime);

    cpuParticlesUpdate.begin ();
    gCurrentEffect->cpuUpdate (deltaTime);
    gNumAlive = 0;// gParticleSystem->numAliveParticles();
    cpuParticlesUpdate.end ();

    cpuBuffersUpdate.begin ();
    gpuUpdate.begin ();
    gCurrentEffect->gpuUpdate (deltaTime);
    gpuUpdate.end ();
    cpuBuffersUpdate.end ();
    gpuUpdate.updateResults (GpuTimerQuery::WaitOption::WaitForResults);

    gNumParticles = gCurrentEffect->numAllParticles ();
    gNumAlive = gCurrentEffect->numAliveParticles ();
  }


  void ParticleEditor::Draw ()
  {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable (GL_DEPTH_TEST);

    //
    // camera
    //
    //camera_.modelviewMatrix = glm::lookAt (Camera::main->viewDirection * camera_.camDistance, glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 1.0f, 0.0f));

    glEnable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, gParticleTexture);

    glEnable (GL_PROGRAM_POINT_SIZE);

    mProgram->Use ();
    mProgram->uniMat4 ("matProjection", glm::value_ptr (Camera::main->GetViewToProjectionMatrix()));
    mProgram->uniMat4 ("matModelview", glm::value_ptr (Camera::main->GetWorldToViewMatrix()));

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);

    gpuRender.begin ();
    gCurrentEffect->render ();
    gpuRender.end ();

    glDisable (GL_BLEND);

    mProgram->Disable ();

    gpuRender.updateResults (GpuTimerQuery::WaitOption::WaitForResults);
  }

  void ParticleEditor::Change_Size (int w, int h)
  {
    float aspect = (float) w / (float) h;

    // Set the viewport to be the entire window
    glViewport (0, 0, w, h);

    // setup projection matrix
    Camera::main->viewToProjection = glm::perspective (45.0f, aspect, 0.1f, 10.0f);
  }


}
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
#include "ParticleGenerators.h"
#include "ParticleRenderer.h"
#include "TimeQuery.h"
#include "Effect.h"
#include "AntTweakBar.h"
#include "WindowSystem.h"
#include "EditorUI.h"
#include "EventSystem.h"
#include "BaseEvent.h"
#include "KeyEvent.h"



namespace Framework
{
  static std::string s = "Hello";
  static int reverbPreset, oldReverbPreset;
  static float LPFcutOff = 6000, LPFresonance = 1;
  static float HPFcutOff = 5000, HPFresonance = 1;
  static float oldLPFCutOff = LPFcutOff, oldLPFResonance = LPFresonance;
  static float oldHPFCutOff = HPFcutOff, oldHPFResonance = HPFresonance;
  static bool lpf = false, oldLPF = false, hpf = false, oldHPF = false, reverb = false, oldReverb = false;
  static bool song = false, oldSong = false, noise = false, oldNoise = false;
  static bool panLeft = false, oldPanLeft = false; bool panRight = false, oldPanRight = false;
  static Sound* bg;
  static TwBar *myBar;
  static TwBar* audioBar;
  static std::shared_ptr<IEffect> gEffects [4];
  static IEffect *gCurrentEffect = nullptr;
  static int gCurrentEffectID = 0;
  static int gSelectedEffect = 0;
  static int gNumParticles = 0;
  static int gNumAlive = 0;
  static float volume = 1.0f;
  static float Fps = 0.0f;
  static float fps = 0.0f;
  static double AppTime = 0;

  static unsigned int gParticleTexture = 0;
  static Shader* mProgram;
  static struct CAMERA
  {
    float camDistance;
    glm::vec3 cameraDir;
    glm::mat4 modelviewMatrix;
    glm::mat4 projectionMatrix;
  } camera_;

  static CpuTimeQuery cpuParticlesUpdate;
  static CpuTimeQuery cpuBuffersUpdate;
  static GpuTimerQuery gpuUpdate;
  static double gpuUpdateTime = 0;
  static GpuTimerQuery gpuRender;
  static double gpuRenderTime = 0;

  static bool gAnimationOn = true;

  // Constructor
  ParticleEditor::ParticleEditor ()
  {
  }

  // Destructor
  ParticleEditor::~ParticleEditor ()
  {
    TwTerminate ();
    gCurrentEffect->clean ();
    glDeleteTextures (1, &gParticleTexture);
  }


  static void OnKeyPressed (KeyEvent* key)
  {
    if (key->KeyDown)
    switch (key->KeyValue)
    {
    case GLFW_KEY_0:
      gSelectedEffect = 0;
      break;
    case GLFW_KEY_1:
      gSelectedEffect = 1;
      break;
    case GLFW_KEY_2:
      gSelectedEffect = 2;
      break;
    case GLFW_KEY_W:
      Camera::main->worldToView = glm::translate (Camera::main->worldToView, glm::vec3 (0, -0.01f, 0));
      break;
    case GLFW_KEY_S:
      Camera::main->worldToView = glm::translate (Camera::main->worldToView, glm::vec3 (0, 0.01f, 0));
      break;
    case GLFW_KEY_D:
      Camera::main->worldToView = glm::translate (Camera::main->worldToView, glm::vec3 (-0.01f, 0, 0));
      break;
    case GLFW_KEY_A:
      Camera::main->worldToView = glm::translate (Camera::main->worldToView, glm::vec3 (0.01f, 0, 0));
      break;
    default:
      break;
    }
  }

  void ParticleEditor::Load_Scene (const char* filename)
  {
    //EVENTSYSTEM->gConnect<KeyEvent>(Events::KEY_ANY, &OnKeyPressed);

    //TwInit (TW_OPENGL, NULL);
    // or
#ifdef _USE_ANTWEAK
    TwInit(TW_OPENGL_CORE, NULL); // for core profile
#endif

    Change_Size (WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height());
    //
    // some global GL states
    //
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.f, 0.f, 0.f, 0.0f);
    TwWindowSize (WINDOWSYSTEM->Get_Width(), WINDOWSYSTEM->Get_Height());
    camera_.cameraDir [0] = 0.0f;
    camera_.cameraDir [1] = 0.0f;
    camera_.cameraDir [2] = 1.0f;
    camera_.camDistance = 0.5f;

    gParticleTexture = Resources::RS->Get_Texture ("Fire1.png")->Get_ID ();
    mProgram = Resources::RS->Get_Shader ("Particle");
    mProgram->Use ();
    mProgram->uni1i ("tex", 0);
    mProgram->uni1f ("size", 15.0f);
    mProgram->Disable ();


    gEffects [0] = EffectFactory::create ("fountain");
    gEffects [0]->initialize (IEffect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffects [0]->initializeRenderer ();
    gEffects [1] = EffectFactory::create ("attractors");
    gEffects [1]->initialize (IEffect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffects [1]->initializeRenderer ();
    gEffects [2] = EffectFactory::create ("tunnel");
    gEffects [2]->initialize (IEffect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffects [2]->initializeRenderer ();
    gEffects [3] = EffectFactory::create ("explosion");
    gEffects [3]->initialize (IEffect::DEFAULT_PARTICLE_NUM_FLAG);
    gEffects [3]->initializeRenderer ();
    gCurrentEffectID = 0;
    gCurrentEffect = gEffects [0].get ();

    gpuUpdate.init ();
    gpuRender.init ();

    myBar = TwNewBar ("Particle System");

    audioBar = TwNewBar("Audio");

    TwAddVarRO (myBar, "FPS", TW_TYPE_FLOAT, &fps, NULL);
    Editor::AddTweak (myBar, "animate", &gAnimationOn, "");
    Editor::AddVar (myBar, "particles", &gNumParticles, "");
    Editor::AddVar (myBar, "alive", &gNumAlive, "");
    Editor::AddVar (myBar, "cpu particles", &cpuParticlesUpdate.m_time, "precision=3 group=timers");
    Editor::AddVar (myBar, "cpu buffers", &cpuBuffersUpdate.m_time, "precision=3 group=timers");
    //Editor::      myBar, 
    Editor::AddVar (myBar, "gpu buffer", &gpuUpdate.getTime (), "precision=3 group=timers");
    Editor::AddVar (myBar, "gpu render", &gpuRender.getTime (), "precision=3 group=timers");
    //Editor::
    //Editor::AddTweakDir3f (myBar, "camera", &camera1.viewDirection.x, "");
    //Editor::AddTweak (myBar, "camera distance", &camera1.fov, "min=0.05 max=4.0 step=0.01");
    Editor::AddSeparator (myBar);
    Editor::AddTweak (myBar, "effect id", &gSelectedEffect, "min=0 max=3");
    gCurrentEffect->addUI (myBar);

    bg = Resources::RS->Get_Sound("music2.mp3");
    Editor::AddTweakText(audioBar, "O HAI ENGINE PROOF", &s, "");
    Editor::AddTweak(audioBar, "Play Song", &song, "group=Play");
    Editor::AddTweak(audioBar, "Generate Noise", &noise, "group=Play");
    Editor::AddTweak(audioBar, "Volume", bg->GetVolumePtr (), "min=0.0 step=0.01 max=1.0 group=Attributes");
    Editor::AddTweak(audioBar, "Pan Left", &panLeft, "group=Attributes");
    Editor::AddTweak(audioBar, "Pan Right", &panRight, "group=Attributes");
    Editor::AddTweak(audioBar, "LPF On/Off", &lpf, "group=LowPassFilter");
    Editor::AddTweak(audioBar, "LPF Cutoff", &LPFcutOff, "min=10.0 step=50 max=22000.0 group=LowPassFilter");
    Editor::AddTweak(audioBar, "LPF Resonance", &LPFresonance, "min=1.0 step=0.5 max=10.0 group=LowPassFilter");
    Editor::AddTweak(audioBar, "HPF On/Off", &hpf, "group=HighPassFilter");
    Editor::AddTweak(audioBar, "HPF Cutoff", &HPFcutOff, "min=10.0 step=50 max=22000.0 group=HighPassFilter");
    Editor::AddTweak(audioBar, "HPF Resonance", &HPFresonance, "min=1.0 step=0.5 max=10.0 group=HighPassFilter");
    Editor::AddTweak(audioBar, "Reverb On/Off", &reverb, "group=Reverb");
    Editor::AddTweak(audioBar, "Set Preset", &reverbPreset, "min=0 max=23 group=Reverb");
    Editor::AddSeparator(audioBar);
    TwDefine (" Audio position='20 500' ");
    //Camera::main->worldToView = glm::lookAt (Camera::main->viewDirection * 0.5f, Camera::main->position, Camera::main->up);
  }


  void ParticleEditor::Update (const double dt)
  {
    double deltaTime;
    Utils::updateTimer (&deltaTime, &AppTime);
    Utils::calculateFps (&Fps);
    fps = Fps * 2;
    if (!gAnimationOn)
      return;

    if (gSelectedEffect != gCurrentEffectID)
    {
      gCurrentEffect->removeUI (myBar);
      gCurrentEffectID = gSelectedEffect;
      gCurrentEffect = gEffects [gCurrentEffectID].get ();
      gCurrentEffect->addUI (myBar);
    }

    gCurrentEffect->update (deltaTime);

    //cpuParticlesUpdate.begin ();
    gCurrentEffect->cpuUpdate (deltaTime);
    gNumAlive = 0;// gParticleSystem->numAliveParticles();
    //cpuParticlesUpdate.end ();

    //cpuBuffersUpdate.begin ();
    //gpuUpdate.begin ();
    gCurrentEffect->gpuUpdate (deltaTime);
    //gpuUpdate.end ();
    //cpuBuffersUpdate.end ();
    //gpuUpdate.updateResults (GpuTimerQuery::WaitOption::WaitForResults);

    gNumParticles = gCurrentEffect->numAllParticles ();
    gNumAlive = gCurrentEffect->numAliveParticles ();

    if (song != oldSong)
    {
      if (song)
        bg->Play();
      else
        bg->Stop();
      oldSong = song;
    }
    if (noise != oldNoise)
    {
      if (noise)
        bg->GenerateNoise();
      else
        bg->Stop();
      oldNoise = noise;
    }
    if (panLeft != oldPanLeft)
    {
      if (panLeft)
        bg->SetPan('L');
      else
        bg->SetPan('C');
      oldPanLeft = panLeft;
    }
    if (panRight != oldPanRight)
    {
      if (panRight)
        bg->SetPan('R');
      else
        bg->SetPan('C');
      oldPanRight = panRight;
    }
    if (lpf != oldLPF)
    {
      if (lpf)
        bg->LowPassFilter();
      else
        bg->Get_Channel()->removeDSP (bg->Get_LPF ());
      oldLPF = lpf;      
    }
    if (lpf && oldLPFResonance != LPFresonance || oldLPFCutOff != LPFcutOff)
    {
      bg->SetLPF(LPFcutOff, LPFresonance);
      oldLPFCutOff = LPFcutOff;
      oldLPFResonance = LPFresonance;
    }

    if (hpf != oldHPF)
    {
      if (hpf)
        bg->HighPassFilter();
      else
        bg->Get_Channel()->removeDSP(bg->Get_HPF());
      oldHPF = hpf;
    }
    if (hpf && oldHPFResonance != HPFresonance || oldHPFCutOff != HPFcutOff)
    {
      bg->SetHPF(HPFcutOff, HPFresonance);
      oldHPFCutOff = HPFcutOff;
      oldHPFResonance = HPFresonance;
    }

    if (reverb != oldReverb)
    {
      if (reverb)
      {
        bg->Reverb();
        bg->AddReverbPreset(Sound::ReverbPresetName(reverbPreset));
      }
      else
        bg->Get_Channel()->removeDSP(bg->Get_RVRB());
      oldReverb = reverb;
    }
    if (reverb && oldReverbPreset != reverbPreset)
    {
      bg->AddReverbPreset(Sound::ReverbPresetName (reverbPreset));
      oldReverbPreset = reverbPreset;
    }
  }


  void ParticleEditor::Draw ()
  {
    //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDisable (GL_DEPTH_TEST);

    
    // camera
    
    camera_.modelviewMatrix = glm::lookAt (camera_.cameraDir * camera_.camDistance, glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3 (0.0f, 1.0f, 0.0f));

    glEnable (GL_POINT_SPRITE);
    glEnable (GL_PROGRAM_POINT_SIZE);
    mProgram->Use ();
    mProgram->uniMat4 ("matProjection", glm::value_ptr (camera_.projectionMatrix));
    mProgram->uniMat4 ("matModelview", glm::value_ptr (camera_.modelviewMatrix));

    glEnable (GL_BLEND);
    //glEnable (GL_DEPTH_TEST);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);

    gpuRender.begin ();
    gCurrentEffect->render ();
    gpuRender.end ();

    glDisable (GL_BLEND);
    glDisable (GL_DEPTH_TEST);

    mProgram->Disable ();

    //gpuRender.updateResults (GpuTimerQuery::WaitOption::WaitForResults);

    TwDraw ();
  }

  void ParticleEditor::Change_Size (int w, int h)
  {
    float aspect = (float) w / (float) h;

    // Set the viewport to be the entire window
    glViewport (0, 0, w, h);

    // setup projection matrix
    camera_.projectionMatrix = glm::perspective (45.0f, aspect, 0.1f, 1000.0f);
    TwWindowSize (w, h);
  }


}
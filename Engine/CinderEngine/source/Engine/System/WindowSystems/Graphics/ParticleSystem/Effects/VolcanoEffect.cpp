/******************************************************************************/
/*!
\file   FountainEffect.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "glew.h"
#include "EditorUI.h"
#include "VolcanoEffect.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "Pipeline.h"
#include "EventSystem.h"
#include "KeyEvent.h"
#include "Collider2D.h"
#include "GameEvent.h"
#include "Core.h"

namespace Framework
{
  DefineComponentName (FountainEffect);
  static float maxSize;
  static float minSize;

  FountainEffect::~FountainEffect ()
  {

    //EVENTSYSTEM->mDisconnect<KeyEvent, FountainEffect>(Events::KEY_ESCAPE, this, &FountainEffect::OnKeyPressed);
    //EVENTSYSTEM->mDisconnect<PauseEvent, FountainEffect>(Events::PAUSE, this, &FountainEffect::OnApplicationPause);
  }


  void FountainEffect::Serialize (Serializer::DataNode* data)
  {
    Component::Get_Enabled (data);
    Serializer::DataNode* value = data->FindElement (data, "ParticleSize");
    value->GetValue (&size);
  }


  void FountainEffect::Initialize ()
  {
    initialize (0);
    initializeRenderer ();

    //EVENTSYSTEM->mConnect<KeyEvent, FountainEffect> (Events::KEY_ANY, this, &FountainEffect::OnKeyPressed);
    //EVENTSYSTEM->mConnect<PauseEvent, FountainEffect> (Events::PAUSE, this, &FountainEffect::OnApplicationPause);

  }


  bool FountainEffect::initialize (size_t numParticles)
  {
    texture = Resources::RS->Get_Texture ("Particle.bmp");
    shader = Resources::RS->Get_Shader ("Particle");

    //
    // particles
    //
    const size_t NUM_PARTICLES = numParticles == 0 ? 10000 : numParticles;
    m_system = std::make_shared<ParticleSystem> (NUM_PARTICLES);
    m_system->init (NUM_PARTICLES);

    //
    // emitter:
    //
    {auto particleEmitter = std::make_shared<ParticleEmitter> ();

    particleEmitter->m_emitRate = (float) NUM_PARTICLES * 0.05f;
    glm::vec3 pos = gameObject->Transform->GetPosition ();
    particleEmitter->position = pos;
    // pos:
    m_posGenerator = std::make_shared<BoxPosGen> ();
    m_posGenerator->m_pos = glm::vec4{ pos.x, pos.y, pos.z, 0.0 };
    m_posGenerator->m_maxStartPosOffset = glm::vec4{ 10.0, 0.0, 0.0, 0.0 };
    particleEmitter->addGenerator (m_posGenerator);

    m_colGenerator = std::make_shared<BasicColorGen> ();
    m_colGenerator->m_minStartCol = vec4{ 255.0 / 255, 64.0 / 255, 00.0 / 255, 0.0 };
    m_colGenerator->m_maxStartCol = vec4{ 255.0 / 255, 64.0 / 255, 0.0 / 255, 0.4 };
    m_colGenerator->m_minEndCol = vec4{ 0.0, 0.0, 0.0, 1.0 };
    m_colGenerator->m_maxEndCol = vec4{ 0.0, 0.0, 0.0, 0.0 };
    particleEmitter->addGenerator (m_colGenerator);

    auto velGenerator = std::make_shared<BasicVelGen> ();
    velGenerator->m_minStartVel = glm::vec4{ -0.05f * 100.0f, 0.0f * 40.0f, -0.05f * 5.0f, 0.0f };
    velGenerator->m_maxStartVel = glm::vec4{ 0.05f * 100.0f, 0.25f * 40.0f, 0.05f * 5.0f, 0.0f };
    particleEmitter->addGenerator (velGenerator);

    auto timeGenerator = std::make_shared<BasicTimeGen> ();
    timeGenerator->m_minTime = 0.0f;
    timeGenerator->m_maxTime = 1.0f;
    particleEmitter->addGenerator (timeGenerator);

    //auto generator = std::make_shared<VelFromPosGen> ();
    //generator->m_maxScale = 5.0f;
    //generator->m_minScale = -3.0f;
    //generator->m_offset = glm::vec4 (0, -1, 1, 1);
    //particleEmitter->addGenerator (generator);

    m_system->addEmitter (particleEmitter);
    }
    {
      auto particleEmitter = std::make_shared<ParticleEmitter> ();

      particleEmitter->m_emitRate = (float) NUM_PARTICLES * 0.05f;
      glm::vec3 pos = gameObject->Transform->GetPosition ();
      particleEmitter->position = pos;
      // pos:
      auto m_posGenerator = std::make_shared<BoxPosGen> ();
      m_posGenerator->m_pos = glm::vec4{ pos.x, pos.y, pos.z, 0.0 };
      m_posGenerator->m_maxStartPosOffset = glm::vec4{ 10.0, 0.0, 0.0, 0.0 };
      particleEmitter->addGenerator (m_posGenerator);

      auto m_colGenerator = std::make_shared<BasicColorGen> ();
      m_colGenerator->m_minStartCol = vec4{ 255.0 / 255, 64.0 / 255, 00.0 / 255, 0.0 };
      m_colGenerator->m_maxStartCol = vec4{ 255.0 / 255, 64.0 / 255, 0.0 / 255, 0.4 };
      m_colGenerator->m_minEndCol = vec4{ 0.0, 0.0, 0.0, 1.0 };
      m_colGenerator->m_maxEndCol = vec4{ 0.0, 0.0, 0.0, 0.0 };
      particleEmitter->addGenerator (m_colGenerator);

      auto velGenerator = std::make_shared<BasicVelGen> ();
      velGenerator->m_minStartVel = glm::vec4{ -0.05f * 200.0f, 0.0f * 100.0f, -0.05f * 5.0f, 0.0f };
      velGenerator->m_maxStartVel = glm::vec4{ 0.05f * 200.0f, 0.25f * 200.0f, 0.05f * 5.0f, 0.0f };
      particleEmitter->addGenerator (velGenerator);

      auto timeGenerator = std::make_shared<BasicTimeGen> ();
      timeGenerator->m_minTime = 1.0f;
      timeGenerator->m_maxTime = 2.0f;
      particleEmitter->addGenerator (timeGenerator);

      //auto generator = std::make_shared<VelFromPosGen> ();
      //generator->m_maxScale = 5.0f;
      //generator->m_minScale = -3.0f;
      //generator->m_offset = glm::vec4 (0, -1, 1, 1);
      //particleEmitter->addGenerator (generator);

      m_system->addEmitter (particleEmitter);
    }

    auto timeUpdater = std::make_shared<BasicTimeUpdater> ();
    m_system->addUpdater (timeUpdater);

    auto colorUpdater = std::make_shared<BasicColorUpdater> ();
    m_system->addUpdater (colorUpdater);

    m_eulerUpdater = std::make_shared<EulerUpdater> ();
    m_eulerUpdater->m_globalAcceleration = glm::vec4{ 0.0, -100.0, 0.0, 0.0 };
    m_system->addUpdater (m_eulerUpdater);

    m_floorUpdater = std::make_shared<FloorUpdater> ();
    m_floorUpdater->m_floorY = gameObject->Transform->GetPosition().y;
    m_system->addUpdater (m_floorUpdater);

    return true;
  }

  bool FountainEffect::initializeRenderer ()
  {
    m_renderer = RendererFactory::create ("gl");
    m_renderer->generate (m_system.get (), false);

    return true;
  }

  void FountainEffect::clean ()
  {
    if (m_renderer) m_renderer->destroy ();
  }

  void FountainEffect::addUI (TwBar* tw)
  {
    //Editor::AddTweakColor4f (tw, "start col min", &m_colGenerator->m_minStartCol.x, "group=effect");
    //Editor::AddTweakColor4f (tw, "start col max", &m_colGenerator->m_maxStartCol.x, "group=effect");
    //Editor::AddTweakColor4f (tw, "end col min", &m_colGenerator->m_minEndCol.x, "group=effect");
    //Editor::AddTweakColor4f (tw, "end col max", &m_colGenerator->m_maxEndCol.x, "group=effect");
    //Editor::AddTweak (tw, "gravity", &m_eulerUpdater->m_globalAcceleration.y, "group=effect min=-20 max=0 step=0.05");
    //Editor::AddTweak (tw, "bounce", &m_floorUpdater->m_bounceFactor, "group=effect min=0 max=1 step=0.05");
  }

  void FountainEffect::removeUI (TwBar* tw)
  {
    //Editor::RemoveVar (tw, "start col min");
    //Editor::RemoveVar (tw, "start col max");
    //Editor::RemoveVar (tw, "end col min");
    //Editor::RemoveVar (tw, "end col max");
    //Editor::RemoveVar (tw, "gravity");
    //Editor::RemoveVar (tw, "bounce");
  }

  void FountainEffect::update (double dt)
  {
    //static double time = 0.0;
    //time += dt;
    //std::cout << particleEmitter->m_emitRate << "\n";
    /*if (getmic() > 0.1f)
    {
    if (velGenerator->m_maxStartVel.y < 0.9f)
    velGenerator->m_maxStartVel.y += getmic() * 0.016f;

    if (particleEmitter->m_emitRate < 100)
    particleEmitter->m_emitRate += getmic();

    if (size < maxSize)
    size += getmic() * 0.16f;
    }
    else
    {
    if (velGenerator->m_maxStartVel.y > 0.01f)
    velGenerator->m_maxStartVel.y -= 0.016f;

    if (particleEmitter->m_emitRate > 10)
    particleEmitter->m_emitRate -= 0.2f;


    if (size > minSize)
    size -= 0.016f;
    }*/
    //velGenerator->m_maxStartVel.x = sin(getmic()) * 0.0001f;
    //m_posGenerator->m_pos.x = 0.1f*sin ((float) time*2.5f);
    //m_posGenerator->m_pos.z = 0.1f*cos ((float) time*2.5f);
  }

  void FountainEffect::cpuUpdate (double dt)
  {
    static double time = 0.0;
    time += dt;

    m_posGenerator->m_pos.x = gameObject->Transform->GetPosition ().x + 10.0f * sin ((float) time * 2.5f * 2.5f);
    m_posGenerator->m_pos.z = gameObject->Transform->GetPosition ().z + 10.0f * cos ((float) time * 2.5f * 2.5f);

    m_system->update (dt);
  }

  void FountainEffect::gpuUpdate (double dt)
  {
    m_renderer->update ();
  }

  void FountainEffect::Draw ()
  {
    paused = CORE->IsPaused ();
    if (enabled)
    {
      if (!paused)
      {
        cpuUpdate (0.016);
        gpuUpdate (0.016);
      }
      render ();
    }
  }

  void FountainEffect::render ()
  {
    glEnable (GL_POINT_SPRITE);
    glEnable (GL_PROGRAM_POINT_SIZE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glEnable (GL_TEXTURE_2D);
    texture->Bind ();
    shader->Use ();
    shader->uni1i ("tex", 0);
    shader->uni1f ("size", size);

    shader->uniMat4 ("matModel", glm::value_ptr (gameObject->Transform->GetModelMatrix ()));
    shader->uniMat4 ("matProjection", glm::value_ptr (Camera::GetViewToProjectionMatrix ()));
    shader->uniMat4 ("matModelview", glm::value_ptr (Camera::GetWorldToViewMatrix ()));
    m_renderer->render ();
    shader->Disable ();
    texture->Unbind ();
  }

  void FountainEffect::OnApplicationPause (PauseEvent* pauseEvent)
  {
    paused = pauseEvent->Paused;
  }
}
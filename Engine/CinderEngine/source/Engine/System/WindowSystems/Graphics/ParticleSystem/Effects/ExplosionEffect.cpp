/******************************************************************************/
/*!
\file   ExplosionEffect.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "EditorUI.h"
#include "ExplosionEffect.h"

namespace Framework
{

  bool ExplosionEffect::initialize (size_t numParticles)
  {
    //
    // particles
    //
    const size_t NUM_PARTICLES = numParticles == 0 ? 10000 : numParticles;
    m_system = std::make_shared<ParticleSystem> (NUM_PARTICLES);

    //
    // emitter:
    //
    auto particleEmitter = std::make_shared<ParticleEmitter> ();
    {
      particleEmitter->m_emitRate = (float) NUM_PARTICLES*0.5f;

      // pos:
      m_posGenerator = std::make_shared<BoxPosGen> ();
      m_posGenerator->m_pos = glm::vec4{ 0.0, 0.0, 0.0, 0.0 };
      m_posGenerator->m_maxStartPosOffset = glm::vec4{ 0.0, 0.0, 0.0, 0.0 };
      particleEmitter->addGenerator (m_posGenerator);

      m_colGenerator = std::make_shared<BasicColorGen> ();
      m_colGenerator->m_minStartCol = glm::vec4{ 0.7, 0.4, 0.2, 1.0 };
      m_colGenerator->m_maxStartCol = glm::vec4{ 1.0, 1.0, 1.0, 1.0 };
      m_colGenerator->m_minEndCol = glm::vec4{ 1.0, 0.0, 0.6, 0.0 };
      m_colGenerator->m_maxEndCol = glm::vec4{ 1.0, 0.5, 0.0, 0.0 };
      particleEmitter->addGenerator (m_colGenerator);

      auto velGenerator = std::make_shared<BasicVelGen> ();
      velGenerator->m_minStartVel = glm::vec4{ -0.05f, -0.05f, -0.05f, 0.0f };
      velGenerator->m_maxStartVel = glm::vec4{ 0.05f, 0.05f, 0.05f, 0.0f };
      particleEmitter->addGenerator (velGenerator);

      auto timeGenerator = std::make_shared<BasicTimeGen> ();
      timeGenerator->m_minTime = 3.0f;
      timeGenerator->m_maxTime = 4.0f;
      particleEmitter->addGenerator (timeGenerator);
    }
    m_system->addEmitter (particleEmitter);

    auto timeUpdater = std::make_shared<BasicTimeUpdater> ();
    m_system->addUpdater (timeUpdater);

    auto colorUpdater = std::make_shared<BasicColorUpdater> ();
    m_system->addUpdater (colorUpdater);

    m_eulerUpdater = std::make_shared<EulerUpdater> ();
    m_eulerUpdater->m_globalAcceleration = glm::vec4{ 0.0, 0.0, 0.0, 0.0 };
    m_system->addUpdater (m_eulerUpdater);

    //m_floorUpdater = std::make_shared<FloorUpdater> ();
    //m_system->addUpdater (m_floorUpdater);

    return true;
  }

  bool ExplosionEffect::initializeRenderer ()
  {
    m_renderer = RendererFactory::create ("gl");
    m_renderer->generate (m_system.get (), false);

    return true;
  }

  void ExplosionEffect::clean ()
  {
    if (m_renderer) m_renderer->destroy ();
  }

  void ExplosionEffect::addUI (TwBar* tw)
  {
    Editor::AddTweakColor4f (tw, "start col min", &m_colGenerator->m_minStartCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "start col max", &m_colGenerator->m_maxStartCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "end col min", &m_colGenerator->m_minEndCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "end col max", &m_colGenerator->m_maxEndCol.x, "group=effect");
    Editor::AddTweak (tw, "gravity", &m_eulerUpdater->m_globalAcceleration.y, "group=effect min=-20 max=0 step=0.05");
    //Editor::AddTweak (tw, "bounce", &m_floorUpdater->m_bounceFactor, "group=effect min=0 max=1 step=0.05");
  }

  void ExplosionEffect::removeUI (TwBar* tw)
  {
    Editor::RemoveVar (tw, "start col min");
    Editor::RemoveVar (tw, "start col max");
    Editor::RemoveVar (tw, "end col min");
    Editor::RemoveVar (tw, "end col max");
    Editor::RemoveVar (tw, "gravity");
    Editor::RemoveVar (tw, "bounce");
  }

  void ExplosionEffect::update (double dt)
  {
    static double time = 0.0;
    time += dt;

    m_posGenerator->m_pos.x = 0.1f * cosf ((float)time * 2.5f);
    m_posGenerator->m_pos.y = 0.1f * sinf ((float)time * 2.5f);
    m_posGenerator->m_pos.z = 0.1f*cos ((float) time*2.5f);
  }

  void ExplosionEffect::cpuUpdate (double dt)
  {
    m_system->update (dt);
  }

  void ExplosionEffect::gpuUpdate (double dt)
  {
    m_renderer->update ();
  }

  void ExplosionEffect::render ()
  {
    m_renderer->render ();
  }

}
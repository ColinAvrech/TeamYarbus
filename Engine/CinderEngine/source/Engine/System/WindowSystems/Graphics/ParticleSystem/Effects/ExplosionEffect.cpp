/******************************************************************************/
/*!
\file   ExplosionEffect.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "glew.h"
#include "EditorUI.h"
#include "ExplosionEffect.h"
#include "ResourceManager.h"

namespace Framework
{

  bool ExplosionEffect::initialize (size_t numParticles)
  {
    texture = Resources::RS->Get_Texture ("Fire1.png");
    shader = Resources::RS->Get_Shader ("Particle");
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
  }

  void ExplosionEffect::removeUI (TwBar* tw)
  {
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
    glEnable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, texture->Get_ID());
    shader->uni1f ("size", 15.0f);
    m_renderer->render ();
  }

}
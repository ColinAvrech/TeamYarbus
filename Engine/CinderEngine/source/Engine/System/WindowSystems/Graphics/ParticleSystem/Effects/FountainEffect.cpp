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
#include "FountainEffect.h"
#include "ResourceManager.h"

namespace Framework
{
  bool FountainEffect::initialize (size_t numParticles)
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
      particleEmitter->m_emitRate = (float) NUM_PARTICLES*0.25f;

      // pos:
      m_posGenerator = std::make_shared<BoxPosGen> ();
      m_posGenerator->m_pos = glm::vec4{ 0.0, 0.0, 0.0, 0.0 };
      m_posGenerator->m_maxStartPosOffset = glm::vec4{ 0.0, 0.0, 0.0, 0.0 };
      particleEmitter->addGenerator (m_posGenerator);

      m_colGenerator = std::make_shared<BasicColorGen> ();
      m_colGenerator->m_minStartCol = glm::vec4{ 255.0/255, 120.0/255, 50.0/255, 0.0 };
      m_colGenerator->m_maxStartCol = glm::vec4{ 255.0 / 255, 120.0 / 255, 50.0 / 255, 1.0 };
      m_colGenerator->m_minEndCol = glm::vec4{ 255.0 / 255, 120.0 / 255, 50.0 / 255, 0.0 };
      m_colGenerator->m_maxEndCol = glm::vec4{ 255.0 / 255, 120.0 / 255, 50.0 / 255, 0.0 };
      particleEmitter->addGenerator (m_colGenerator);

      auto velGenerator = std::make_shared<BasicVelGen> ();
      velGenerator->m_minStartVel = glm::vec4{ -0.05f, 0.22f, -0.05f, 0.0f };
      velGenerator->m_maxStartVel = glm::vec4{ 0.05f, 0.25f, 0.05f, 0.0f };
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
    m_eulerUpdater->m_globalAcceleration = glm::vec4{ 0.0, -15.0, 0.0, 0.0 };
    m_system->addUpdater (m_eulerUpdater);

    m_floorUpdater = std::make_shared<FloorUpdater> ();
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
    Editor::AddTweakColor4f (tw, "start col min", &m_colGenerator->m_minStartCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "start col max", &m_colGenerator->m_maxStartCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "end col min", &m_colGenerator->m_minEndCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "end col max", &m_colGenerator->m_maxEndCol.x, "group=effect");
    Editor::AddTweak (tw, "gravity", &m_eulerUpdater->m_globalAcceleration.y, "group=effect min=-20 max=0 step=0.05");
    Editor::AddTweak (tw, "bounce", &m_floorUpdater->m_bounceFactor, "group=effect min=0 max=1 step=0.05");
  }

  void FountainEffect::removeUI (TwBar* tw)
  {
    Editor::RemoveVar (tw, "start col min");
    Editor::RemoveVar (tw, "start col max");
    Editor::RemoveVar (tw, "end col min");
    Editor::RemoveVar (tw, "end col max");
    Editor::RemoveVar (tw, "gravity");
    Editor::RemoveVar (tw, "bounce");
  }

  void FountainEffect::update (double dt)
  {
    static double time = 0.0;
    time += dt;

    m_posGenerator->m_pos.x = 0.1f*sin ((float) time*2.5f);
    m_posGenerator->m_pos.z = 0.1f*cos ((float) time*2.5f);
  }

  void FountainEffect::cpuUpdate (double dt)
  {
    m_system->update (dt);
  }

  void FountainEffect::gpuUpdate (double dt)
  {
    m_renderer->update ();
  }

  void FountainEffect::render ()
  {
    glEnable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, texture->Get_ID ());
    shader->uni1f ("size", 15.0f);

    m_renderer->render ();
  }
}
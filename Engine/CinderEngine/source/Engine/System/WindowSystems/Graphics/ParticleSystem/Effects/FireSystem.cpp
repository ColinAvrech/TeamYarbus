/******************************************************************************/
/*!
\file   FireSystem.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>
#include "FireSystem.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "EventSystem.h"
#include "KeyEvent.h"
#include "Thermodynamics.h"
#include "GameEvent.h"
#include "glfw3.h"
#include "Events.h"

namespace Framework
{
  DefineComponentName (FireSystem);

  // Constructor
  FireSystem::FireSystem ()
  {}

  // Destructor
  FireSystem::~FireSystem ()
  {
    if (Physics::ThermodynamicsSystem::FIRE == this)
    {
      Physics::ThermodynamicsSystem::FIRE = nullptr;
    }
  }

  void FireSystem::Serialize (Serializer::DataNode* data)
  {
    Component::Get_Enabled (data);
    Serializer::DataNode* value = data->FindElement (data, "ParticleSize");
    value->GetValue (&size);
  }

  void FireSystem::Initialize ()
  {
    initialize (0);
    initializeRenderer ();

    if (Physics::ThermodynamicsSystem::FIRE == nullptr)
    {
      Physics::ThermodynamicsSystem::FIRE = this;
    }
  }

  void FireSystem::OnKeyPressed (KeyEvent* key)
  {
    if (key->KeyDown)
    {
      switch (key->KeyValue)
      {
      case GLFW_KEY_F:
        AddFire (glm::linearRand (-8.0f, 8.0f), glm::linearRand (-8.0f, 8.0f), 100);
        break;
      default:
        break;
      }
    }
  }

  bool FireSystem::initialize (size_t numParticles)
  {
    EVENTSYSTEM->mConnect<KeyEvent, FireSystem> (Events::KEY_ANY, this, &FireSystem::OnKeyPressed);
    texture = Resources::RS->Get_Texture ("Particle.bmp");
    shader = Resources::RS->Get_Shader ("Particle");

    const size_t NUM_PARTICLES = numParticles == 0 ? 500000 : numParticles;
    m_system = std::make_shared<ParticleSystem> (NUM_PARTICLES);
    m_system->init (NUM_PARTICLES);

    //AddFire (0, 0, 100);

    auto timeUpdater = std::make_shared<BasicTimeUpdater> ();
    m_system->addUpdater (timeUpdater);

    auto colorUpdater = std::make_shared<BasicColorUpdater> ();
    //m_system->ssbo = colorUpdater->Init (NUM_PARTICLES);
    m_system->addUpdater (colorUpdater);

    auto m_eulerUpdater = std::make_shared<EulerUpdater> ();
    m_eulerUpdater->m_globalAcceleration = glm::vec4{ 0.0, 0.0, 0.0, 0.0 };
    m_system->addUpdater (m_eulerUpdater);

    return true;
  }

  void FireSystem::AddFire (float x, float y, float emitRate)
  {
    glm::vec3 minVelocity = glm::linearRand (glm::vec3 (-1.0f, 1.0f, 0), glm::vec3 (1.0f, 2.0f, 0));
    glm::vec3 maxVelocity = glm::linearRand (glm::vec3 (-2.0f, 2.0f, 0), glm::vec3 (2.0f, 6.0f, 0));
    AddFireEmitter (glm::vec3 (x, y, 0), emitRate, minVelocity, maxVelocity);
  }

  bool FireSystem::initializeRenderer ()
  {
    m_renderer = RendererFactory::create ("gl");
    m_renderer->generate (m_system.get (), false);

    return true;
  }

  void FireSystem::reset ()
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void FireSystem::clean ()
  {
    if (m_renderer) m_renderer->destroy ();
  }

  void FireSystem::addUI (TwBar*)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void FireSystem::removeUI (TwBar*)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void FireSystem::update (double dt)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void FireSystem::cpuUpdate (double dt)
  {
    m_system->update (dt);
  }

  void FireSystem::gpuUpdate (double dt)
  {
    m_renderer->update ();
  }

  void FireSystem::Draw ()
  {
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

  void FireSystem::render ()
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

  int FireSystem::numAllParticles ()
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  int FireSystem::numAliveParticles ()
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  //BoundType* FireSystem::ZilchGetDerivedType () const
  //{
  //  throw std::logic_error ("The method or operation is not implemented.");
  //}

  void FireSystem::AddFireEmitter (glm::vec3 position, float emitRate, glm::vec3 minVel, glm::vec3 maxVel)
  {
    auto particleEmitter = std::make_shared<ParticleEmitter> ();
    {
      particleEmitter->m_emitRate = emitRate;
      particleEmitter->position = position;

      // pos:
      auto m_posGenerator = std::make_shared<BoxPosGen> ();
      m_posGenerator->m_pos = glm::vec4{ particleEmitter->position, 0.0 };
      m_posGenerator->m_maxStartPosOffset = glm::vec4{ 0.75f, 1.0, 0.0, 0.0 };
      particleEmitter->addGenerator (m_posGenerator);

      auto m_colGenerator = std::make_shared<BasicColorGen> ();
      m_colGenerator->m_minStartCol = glm::vec4{ 255.0 / 255, 64.0 / 255, 00.0 / 255, 0.0 };
      m_colGenerator->m_maxStartCol = glm::vec4{ 255.0 / 255, 64.0 / 255, 0.0 / 255, 0.2 };
      m_colGenerator->m_minEndCol = glm::vec4{ 0, 0, 0, 0.0 };
      m_colGenerator->m_maxEndCol = glm::vec4{ 0, 0, 0, 0.0 };
      particleEmitter->addGenerator (m_colGenerator);

      auto velGenerator = std::make_shared<BasicVelGen> ();
      velGenerator->m_minStartVel = glm::vec4{ minVel, 0.0f };
      velGenerator->m_maxStartVel = glm::vec4{ maxVel, 0.0f };
      particleEmitter->addGenerator (velGenerator);

      auto timeGenerator = std::make_shared<BasicTimeGen> ();
      timeGenerator->m_minTime = 0.0f;
      timeGenerator->m_maxTime = 1.0f;
      particleEmitter->addGenerator (timeGenerator);
    }

    m_system->addEmitter (particleEmitter);
  }

  void FireSystem::OnApplicationPause (PauseEvent* pause)
  {
    paused = pause->Paused;
  }

  void FireSystem::RemoveFire (FireStarter* param1)
  {
    m_system->reset();
  }
}
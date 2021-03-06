/******************************************************************************/
/*!
\file   AttractorEffect.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>
#include "glew.h"
#include "EditorUI.h"
#include "AttractorEffect.h"
#include "ResourceManager.h"

namespace Framework
{
  bool AttractorEffect::initialize (size_t numParticles)
  {
    texture = Resources::RS->Get_Texture ("Particle1.png");
    shader = Resources::RS->Get_Shader ("Particle");
    // CREATE PARTICLE SYSTEM
    const size_t NUM_PARTICLES = numParticles == 0 ? 25000 : numParticles;
    m_system = std::make_shared<ParticleSystem> (NUM_PARTICLES);

    // ADD COLOR GENERATOR
    m_colGenerator = std::make_shared<BasicColorGen> ();
    m_colGenerator->m_minStartCol = vec4{ 0.99, 0.99, 0.99, 1.0 };
    m_colGenerator->m_maxStartCol = vec4{ 0.99, 0.99, 1.0, 1.0 };
    m_colGenerator->m_minEndCol = vec4{ 0.99, 0.99, 0.99, 0.0 };
    m_colGenerator->m_maxEndCol = vec4{ 0.99, 0.99, 1.0, 0.25 };

    // ADD SPHERE VELOCITY GENERATOR
    auto velGenerator = std::make_shared<SphereVelGen> ();
    velGenerator->m_minVel = 0.1f;
    velGenerator->m_maxVel = 0.1f;

    // ADD TIMER
    auto timeGenerator = std::make_shared<BasicTimeGen> ();
    timeGenerator->m_minTime = 1.6f;
    timeGenerator->m_maxTime = 4.0f;


    // EMITTER 1
    auto particleEmitter = std::make_shared<ParticleEmitter> ();
    {
      particleEmitter->m_emitRate = (float) NUM_PARTICLES*0.1f;

      // POSITION CALCULATION:
      m_posGenerators [0] = std::make_shared<BoxPosGen> ();
      m_posGenerators [0]->m_pos = vec4{ 2.0, 0.0, -0.25, 0.0 };
      m_posGenerators [0]->m_maxStartPosOffset = vec4{ 0.0, 0.0, 0.0, 0.0 };
      particleEmitter->addGenerator (m_posGenerators [0]);

      particleEmitter->addGenerator (m_colGenerator);
      particleEmitter->addGenerator (velGenerator);
      particleEmitter->addGenerator (timeGenerator);
    }
    m_system->addEmitter (particleEmitter);


    // EMITTER 2
    auto particleEmitter2 = std::make_shared<ParticleEmitter> ();
    {
      particleEmitter2->m_emitRate = (float) NUM_PARTICLES*0.1f;

      m_posGenerators [1] = std::make_shared<BoxPosGen> ();
      m_posGenerators [1]->m_pos = vec4{ 0.0, 0.0, 0.25, 0.0 };
      m_posGenerators [1]->m_maxStartPosOffset = vec4{ 0.0, 0.0, 0.0, 0.0 };
      particleEmitter2->addGenerator (m_posGenerators [1]);

      particleEmitter2->addGenerator (m_colGenerator);
      particleEmitter2->addGenerator (velGenerator);
      particleEmitter2->addGenerator (timeGenerator);
    }
    m_system->addEmitter (particleEmitter2);


    // EMITTER 3
    auto particleEmitter3 = std::make_shared<ParticleEmitter> ();
    {
      particleEmitter3->m_emitRate = (float) NUM_PARTICLES*0.1f;

      m_posGenerators [2] = std::make_shared<BoxPosGen> ();
      m_posGenerators [2]->m_pos = vec4{ 0.0, 0.0, 0.25, 0.0 };
      m_posGenerators [2]->m_maxStartPosOffset = vec4{ 0.0, 0.0, 0.0, 0.0 };
      particleEmitter3->addGenerator (m_posGenerators [2]);

      particleEmitter3->addGenerator (m_colGenerator);
      particleEmitter3->addGenerator (velGenerator);
      particleEmitter3->addGenerator (timeGenerator);
    }
    m_system->addEmitter (particleEmitter3);

    //
    // updaters:
    //
    auto timeUpdater = std::make_shared<BasicTimeUpdater> ();
    m_system->addUpdater (timeUpdater);

    auto colorUpdater = std::make_shared<VelColorUpdater> ();
    colorUpdater->m_minVel = vec4{ -0.5f, -0.5f, -0.5f, 0.0f };
    colorUpdater->m_maxVel = vec4{ 4.0f, 2.0f, 2.0f, 2.0f };
    m_system->addUpdater (colorUpdater);

    m_attractors = std::make_shared<AttractorUpdater> ();
    m_attractors->add (vec4{ 0.0, 0.0, 0.75, 1.0 });
    m_attractors->add (vec4{ 0.0, 0.0, -0.75, 1.0 });
    m_attractors->add (vec4{ 0.0, 0.75, 0.0, 1.0 });
    m_attractors->add (vec4{ 0.0, -0.75, 0.0, 1.0 });
    m_system->addUpdater (m_attractors);

    auto eulerUpdater = std::make_shared<EulerUpdater> ();
    eulerUpdater->m_globalAcceleration = vec4{ 0.0, 0.0, 0.0, 0.0 };
    m_system->addUpdater (eulerUpdater);

    m_zScale = 1.0f;

    return true;
  }

  bool AttractorEffect::initializeRenderer ()
  {
    m_renderer = RendererFactory::create ("gl");
    m_renderer->generate (m_system.get (), false);

    return true;
  }

  void AttractorEffect::clean ()
  {
    if (m_renderer) m_renderer->destroy ();
  }


  // ANT-TWEAK BAR TODO
  void AttractorEffect::addUI (TwBar* tw)
  {
    Editor::AddTweakColor4f (tw, "start col min", &m_colGenerator->m_minStartCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "start col max", &m_colGenerator->m_maxStartCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "end col min", &m_colGenerator->m_minEndCol.x, "group=effect");
    Editor::AddTweakColor4f (tw, "end col max", &m_colGenerator->m_maxEndCol.x, "group=effect");
    Editor::AddTweak (tw, "z scale", &m_zScale, "min=0.0 max=1.0 step=0.05 group=effect");
    for (size_t i = 0; i < m_attractors->collectionSize(); ++i)
      Editor::AddTweak (tw, ("attr " + std::to_string (i + 1)).c_str (), &(m_attractors->get (i)).w, "min=-1.0 max=1.0 step=0.05 group=effect");
  }


  // ANT-TWEAK BAR TODO
  void AttractorEffect::removeUI (TwBar* tw)
  {
    Editor::RemoveVar(tw, "start col min");
    Editor::RemoveVar(tw, "start col max");
    Editor::RemoveVar(tw, "end col min");
    Editor::RemoveVar(tw, "end col max");
    Editor::RemoveVar(tw, "z scale");
    for (size_t i = 0; i < m_attractors->collectionSize(); ++i)
    Editor::RemoveVar(tw, ("attr " + std::to_string(i + 1)).c_str());
  }


  // UPDATE THE ENTIRE PARTICLE SYSTEM STEP BY STEP
  // SRP BEST EVER!
  void AttractorEffect::update (double dt)
  {
    static double time = 0.0;
    time += dt;

    // PLAY WITH THE VALUES!!
    m_posGenerators [0]->m_pos.x = 0.15f*cosf ((float) time*2.5f);
    m_posGenerators [0]->m_pos.y = 0.15f*cosf ((float) time*2.5f);
    m_posGenerators [0]->m_pos.z = m_zScale*0.25f*cosf ((float) time*2.5f);

    m_posGenerators [1]->m_pos.x = -0.15f*sinf ((float) time*2.f);
    m_posGenerators [1]->m_pos.y = 0.15f*cosf ((float) time*2.f);
    m_posGenerators [1]->m_pos.z = m_zScale*0.25f*cosf ((float) time*1.5f);

    m_posGenerators [2]->m_pos.x = -0.15f*sinf ((float) time*1.5f);
    m_posGenerators [2]->m_pos.y = 0.15f*cosf ((float) time*2.5f);
    m_posGenerators [2]->m_pos.z = m_zScale*0.25f*cosf ((float) time*1.75f);
  }


  // CALCULATIONS ON THE CPU
  // MATRIX MULTIPLICATIONS
  void AttractorEffect::cpuUpdate (double dt)
  {
    m_system->update (dt);
  }

  // GPU CALCULATIONS
  // GOAL IS TO SHIFT AS MUCH CALCULATIONS TO THIS SIDE WITHOUT TOO MUCH DATA TRANSFER
  void AttractorEffect::gpuUpdate (double dt)
  {
    m_renderer->update ();
  }

  void AttractorEffect::render ()
  {
    glEnable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, texture->Get_ID());
    shader->uni1f ("size", 2.0f);
    m_renderer->render ();
  }

  void AttractorEffect::Initialize ()
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void AttractorEffect::Serialize (Serializer::DataNode* data)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

}
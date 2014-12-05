/******************************************************************************/
/*!
\file   PlayerEffect.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "glew.h"
#include "EditorUI.h"
#include "PlayerEffect.h"
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
  DefineComponentName (PlayerEffect);
  static float maxSize;
  static float minSize;

  PlayerEffect::~PlayerEffect ()
  {

	  //EVENTSYSTEM->mDisconnect<KeyEvent, PlayerEffect>(Events::KEY_ESCAPE, this, &PlayerEffect::OnKeyPressed);
	  //EVENTSYSTEM->mDisconnect<PauseEvent, PlayerEffect>(Events::PAUSE, this, &PlayerEffect::OnApplicationPause);
  }


  void PlayerEffect::Serialize (Serializer::DataNode* data)
  {
    Component::Get_Enabled (data);
    Serializer::DataNode* value = data->FindElement (data, "ParticleSize");
    value->GetValue (&size);
  }

  void PlayerEffect::OnKeyPressed (KeyEvent* key)
  {
	  if (key->KeyDown)
	  {
		  CORE->TogglePaused();
	  }
  }


  void PlayerEffect::Initialize ()
  {
    initialize (0);
    initializeRenderer ();

    //EVENTSYSTEM->mConnect<KeyEvent, PlayerEffect> (Events::KEY_ANY, this, &PlayerEffect::OnKeyPressed);
    //EVENTSYSTEM->mConnect<PauseEvent, PlayerEffect> (Events::PAUSE, this, &PlayerEffect::OnApplicationPause);

  }


  bool PlayerEffect::initialize (size_t numParticles)
  {
    texture = Resources::RS->Get_Texture ("Particle.bmp");
    shader = Resources::RS->Get_Shader ("Particle");
    //
    // particles
    //
    const size_t NUM_PARTICLES = numParticles == 0 ? 20000 : numParticles;
    m_system = std::make_shared<ParticleSystem> (NUM_PARTICLES);
    m_system->init (NUM_PARTICLES);

    // Emitters
    vec3 position = gameObject->Transform->GetPosition ();
    trailGenerator = std::make_shared<BoxPosGen> ();
    trailGenerator1 = std::make_shared<BoxPosGen> ();
    CreateTrailEmitter  (trailGenerator, true, position, { 1.5f, 1.5f, -0.5f }, { 3.0f, 3.0f, 0.5f }, 50);
    CreateTrailEmitter (trailGenerator1, true, position, { 1.5f, -1.5f, -0.5f }, { 3.0f, -3.0f, 0.5f }, 50);
    CreateRingEmitter   (true, position, { -0.5f, 1.5f, -0.5f }, { 0.5f, 3.0f, 0.5f }, 100);

    auto timeUpdater = std::make_shared<BasicTimeUpdater> ();
    m_system->addUpdater (timeUpdater);

    auto colorUpdater = std::make_shared<BasicColorUpdater> ();
    //m_system->ssbo = colorUpdater->Init (NUM_PARTICLES);
    m_system->addUpdater (colorUpdater);

    m_eulerUpdater = std::make_shared<EulerUpdater> ();
    m_eulerUpdater->m_globalAcceleration = vec4{ 0.0, 0.0, 0.0, 0.0 };
    m_system->addUpdater (m_eulerUpdater);

    //m_floorUpdater = std::make_shared<FloorUpdater> ();
    //m_system->addUpdater (m_floorUpdater);

    return true;
  }

  bool PlayerEffect::initializeRenderer ()
  {
    m_renderer = RendererFactory::create ("gl");
    m_renderer->generate (m_system.get (), false);

    return true;
  }

  void PlayerEffect::clean ()
  {
    if (m_renderer) m_renderer->destroy ();
  }

  void PlayerEffect::addUI (TwBar* tw)
  {
    //Editor::AddTweakColor4f (tw, "start col min", &m_colGenerator->m_minStartCol.x, "group=effect");
    //Editor::AddTweakColor4f (tw, "start col max", &m_colGenerator->m_maxStartCol.x, "group=effect");
    //Editor::AddTweakColor4f (tw, "end col min", &m_colGenerator->m_minEndCol.x, "group=effect");
    //Editor::AddTweakColor4f (tw, "end col max", &m_colGenerator->m_maxEndCol.x, "group=effect");
    //Editor::AddTweak (tw, "gravity", &m_eulerUpdater->m_globalAcceleration.y, "group=effect min=-20 max=0 step=0.05");
    //Editor::AddTweak (tw, "bounce", &m_floorUpdater->m_bounceFactor, "group=effect min=0 max=1 step=0.05");
  }

  void PlayerEffect::removeUI (TwBar* tw)
  {
    //Editor::RemoveVar (tw, "start col min");
    //Editor::RemoveVar (tw, "start col max");
    //Editor::RemoveVar (tw, "end col min");
    //Editor::RemoveVar (tw, "end col max");
    //Editor::RemoveVar (tw, "gravity");
    //Editor::RemoveVar (tw, "bounce");
  }

  void PlayerEffect::update (double dt)
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

  void PlayerEffect::cpuUpdate (double dt)
  {
    trailGenerator->m_pos = glm::vec4 (gameObject->Transform->GetPosition (), 1.0f);
    trailGenerator1->m_pos = glm::vec4 (gameObject->Transform->GetPosition (), 1.0f);
    //particleEmitter->position = gameObject->Transform->GetPosition ();
    m_system->update (dt);
  }

  void PlayerEffect::gpuUpdate (double dt)
  {
    m_renderer->update ();
  }

  void PlayerEffect::Draw ()
  {
	  paused = CORE->IsPaused();
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

  void PlayerEffect::render ()
  {
    glEnable (GL_POINT_SPRITE);
    glEnable (GL_PROGRAM_POINT_SIZE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glEnable (GL_TEXTURE_2D);
    texture->Bind();
    shader->Use ();
    shader->uni1i ("tex", 0);
    shader->uni1f ("size", size);

    shader->uniMat4 ("matModel", glm::value_ptr (gameObject->Transform->GetModelMatrix ()));
    shader->uniMat4 ("matProjection", glm::value_ptr (Camera::GetViewToProjectionMatrix()));
    shader->uniMat4 ("matModelview", glm::value_ptr (Camera::GetWorldToViewMatrix()));
    m_renderer->render ();
    shader->Disable ();
    texture->Unbind ();
  }

  void PlayerEffect::CreateTrailEmitter(std::shared_ptr<BoxPosGen> trail, bool active, vec3 position, vec3 minVelocity, vec3 maxVelocity, float emitRate)
{
    trailEmitter = std::make_shared<ParticleEmitter> ();
    trailEmitter->m_emitRate = emitRate;
    trailEmitter->position = position;
    trailEmitter->active = active;

    // pos:
    trail->m_pos = vec4{ trailEmitter->position, 0.0 };
    trail->m_maxStartPosOffset = vec4{ 0.0, 0.0, 0.0, 0.0 };
    trailEmitter->addGenerator (trail);

    auto colGenerator = std::make_shared<BasicColorGen> ();
    colGenerator->m_minStartCol = vec4{ 255.0 / 255, 64.0 / 255, 00.0 / 255, 0.0 };
    colGenerator->m_maxStartCol = vec4{ 255.0 / 255, 64.0 / 255, 0.0 / 255, 0.4 };
    colGenerator->m_minEndCol = vec4{ 0, 0, 0, 0.0 };
    colGenerator->m_maxEndCol = vec4{ 0, 0, 0, 0.0 };
    trailEmitter->addGenerator (colGenerator);

    auto velGenerator = std::make_shared<BasicVelGen> ();
    velGenerator->m_minStartVel = vec4{ minVelocity, 0.0f };
    velGenerator->m_maxStartVel = vec4{ maxVelocity, 0.0f };
    trailEmitter->addGenerator (velGenerator);

    auto timeGenerator = std::make_shared<BasicTimeGen> ();
    timeGenerator->m_minTime = 0.0f;
    timeGenerator->m_maxTime = 1.0f;
    trailEmitter->addGenerator (timeGenerator);

    m_system->addEmitter (trailEmitter);
  }

  void PlayerEffect::CreateRingEmitter (bool active, vec3 position, vec3 minVelocity, vec3 maxVelocity, float emitRate)
  {
    ringEmitter = std::make_shared<ParticleEmitter> ();
    ringEmitter->m_emitRate = emitRate;
    ringEmitter->position = position;
    ringEmitter->active = active;

    // pos:
    //ringGenerator = std::make_shared<RoundPosGen> ();
    //ringGenerator->m_center = vec4{ ringEmitter->position, 0.0 };
    //ringGenerator->m_radX = gameObject->ShapeCollider2D->radius;
    //ringGenerator->m_radY = gameObject->ShapeCollider2D->radius;
    ringEmitter->addGenerator (trailGenerator);

    auto colGenerator = std::make_shared<BasicColorGen> ();
    colGenerator->m_minStartCol = vec4{ 255.0 / 255, 0.0 / 255, 00.0 / 255, 0.0 };
    colGenerator->m_maxStartCol = vec4{ 255.0 / 255, 0.0 / 255, 0.0 / 255, 0.4 };
    colGenerator->m_minEndCol = vec4{ 0, 0, 0, 0.0 };
    colGenerator->m_maxEndCol = vec4{ 0, 0, 0, 0.0 };
    //colGenerator->m_minStartCol = vec4{ 0, 0, 1, 0.0 };
    //colGenerator->m_maxStartCol = vec4{ 0, 0, 1, 0.4 };
    //colGenerator->m_minEndCol = vec4{ 0, 0, 0, 0.0 };
    //colGenerator->m_maxEndCol = vec4{ 0, 0, 0, 0.0 };
    ringEmitter->addGenerator (colGenerator);

    //auto velGenerator = std::make_shared<SphereVelGen> ();
    //velGenerator->m_minVel = -5.f;
    //velGenerator->m_maxVel = 5.f;
    //ringEmitter->addGenerator (velGenerator);

    auto generator = std::make_shared<VelFromPosGen> ();
    generator->m_maxScale = 5.0f;
    generator->m_minScale = -5.0f;
    generator->m_offset = glm::vec4 (0);
    ringEmitter->addGenerator (generator);

    auto timeGenerator = std::make_shared<BasicTimeGen> ();
    timeGenerator->m_minTime = 0.0f;
    timeGenerator->m_maxTime = 1.0f;
    ringEmitter->addGenerator (timeGenerator);

    m_system->addEmitter (ringEmitter);
  }

  void PlayerEffect::OnApplicationPause (PauseEvent* pauseEvent)
  {
    paused = pauseEvent->Paused;
  }
}
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
#include "WindowSystem.h"
#include "Pipeline.h"
#include "EventSystem.h"
#include "KeyEvent.h"

namespace Framework
{
  DefineComponentName (FountainEffect);
  std::shared_ptr<ParticleEmitter> particleEmitter;
  std::shared_ptr<BasicVelGen> velGenerator;
  static float size;
  static float maxSize;
  static float minSize;

  FountainEffect::~FountainEffect ()
  {
    //WINDOWSYSTEM->effectList.remove (this);
  }


  void FountainEffect::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "ParticleSize");
    value->GetValue (&size);

    value = data->FindElement (data, "MaxSize");
    value->GetValue (&maxSize);

    value = data->FindElement (data, "MinSize");
    value->GetValue (&minSize);
  }

  void FountainEffect::OnKeyPressed (KeyEvent* key)
  {
    switch (key->KeyValue)
    {
    case GLFW_KEY_F:
      break;
    default:
      break;
    }
  }


  void FountainEffect::Initialize ()
  {
    initialize (0);
    initializeRenderer ();

    EVENTSYSTEM->mConnect<KeyEvent, FountainEffect> (Events::KEY_ANY, this, &FountainEffect::OnKeyPressed);
  }


  bool FountainEffect::initialize (size_t numParticles)
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
    float x = 0.01f, y = -0.12f;
    AddFireEmitter (true, position, { -0.5f, 1.5f, -0.5f }, { 0.5f, 3.0f, 0.5f }, 100);

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

  static float getmic() { return AUDIOSYSTEM->input.peaklevel[0]; }

  void FountainEffect::update (double dt)
  {
    static double time = 0.0;
    time += dt;
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
    m_posGenerator->m_pos = glm::vec4 (gameObject->Transform->GetPosition (), 1.0f);
    //particleEmitter->position = gameObject->Transform->GetPosition ();
    m_system->update (dt);
  }

  void FountainEffect::gpuUpdate (double dt)
  {
    m_renderer->update ();
  }

  void FountainEffect::Draw ()
  {
    cpuUpdate (0.016);
    gpuUpdate (0.016);
    render ();
  }

  void FountainEffect::render ()
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

    glDisable (GL_POINT_SPRITE);
    glDisable (GL_PROGRAM_POINT_SIZE);
    glDisable (GL_BLEND);
  }

  void FountainEffect::AddFireEmitter(bool active, vec3 position, vec3 minVelocity, vec3 maxVelocity, float emitRate)
{
    particleEmitter = std::make_shared<ParticleEmitter> ();
    {
      particleEmitter->m_emitRate = emitRate;
      particleEmitter->position = position;
      particleEmitter->active = active;

      // pos:
      m_posGenerator = std::make_shared<BoxPosGen> ();
      m_posGenerator->m_pos = vec4{ particleEmitter->position, 0.0 };
      m_posGenerator->m_maxStartPosOffset = vec4{ 0.0, 0.0, 0.0, 0.0 };
      particleEmitter->addGenerator (m_posGenerator);

      m_colGenerator = std::make_shared<BasicColorGen> ();
      m_colGenerator->m_minStartCol = vec4{ 255.0 / 255, 64.0 / 255, 00.0 / 255, 0.0 };
      m_colGenerator->m_maxStartCol = vec4{ 255.0 / 255, 64.0 / 255, 0.0 / 255, 0.4 };
      m_colGenerator->m_minEndCol = vec4{ 0, 0, 0, 0.0 };
      m_colGenerator->m_maxEndCol = vec4{ 0, 0, 0, 0.0 };
      particleEmitter->addGenerator (m_colGenerator);

      velGenerator = std::make_shared<BasicVelGen> ();
      velGenerator->m_minStartVel = vec4{ minVelocity, 0.0f };
      velGenerator->m_maxStartVel = vec4{ maxVelocity, 0.0f };
      particleEmitter->addGenerator (velGenerator);

      auto timeGenerator = std::make_shared<BasicTimeGen> ();
      timeGenerator->m_minTime = 0.0f;
      timeGenerator->m_maxTime = 1.0f;
      particleEmitter->addGenerator (timeGenerator);
    }
    m_system->addEmitter (particleEmitter);
  }

}
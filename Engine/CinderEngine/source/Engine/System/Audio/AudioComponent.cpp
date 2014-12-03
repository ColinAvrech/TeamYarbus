/*****************************************************************************/
/*!
\file   AudioComponent.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Creates a component for audio
*/      
/*****************************************************************************/

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include "AudioComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "AudioSystem.h"
#include "Core.h"

#pragma endregion

namespace Framework
{
  /*---------------------------------------------------------------------------
  // Class Implementation
  ---------------------------------------------------------------------------*/

  /*---------------------------------------------------------------------------
  // Constructors
  ---------------------------------------------------------------------------*/
  #pragma region Constructors

  DefineComponentName(AudioComponent);

  AudioComponent::AudioComponent()
  {
    positional = false;
    playing = false;
    mute = false;
    lowpassed = false;
    lowcutoff = 6000;
    lowresonance = 1;
    highpassed = false;
    highcutoff = 150;
    highresonance = 1;
    micEffect = false;
  }

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Public Variables
  ---------------------------------------------------------------------------*/
  #pragma region Global Variables

  #pragma endregion

  /*-------------------------------------------------------------------------*/
  // Public Structs
  /*-------------------------------------------------------------------------*/
  #pragma region Public Structs

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Public Functions
  ---------------------------------------------------------------------------*/
  #pragma region Public Functions

  void AudioComponent::Serialize(Serializer::DataNode* data)
  {
    string soundName;

    Serializer::DataNode* value = data->FindElement(data, "Positional");
    value->GetValue(&positional);

    value = data->FindElement(data, "MicEffect");
    value->GetValue(&micEffect);

    value = data->FindElement(data, "Play");
    value->GetValue(&playing);

    value = data->FindElement(data, "Mute");
    value->GetValue(&mute);

    value = data->FindElement(data, "LowPassFilter");
    value->GetValue(&lowpassed);

    value = data->FindElement(data, "LPFcutoff");
    value->GetValue(&lowcutoff);

    value = data->FindElement(data, "LPFresonance");
    value->GetValue(&lowresonance);

    value = data->FindElement(data, "HighPassFilter");
    value->GetValue(&highpassed);

    value = data->FindElement(data, "HPFcutoff");
    value->GetValue(&highcutoff);

    value = data->FindElement(data, "HPFresonance");
    value->GetValue(&highresonance);

    value = data->FindElement(data, "SoundID");
    value->GetValue(&soundName);

    

    newSound = AUDIOSYSTEM->LoadSound(soundName.c_str(), "Test", Sound::SOUND_3D, 1.0f);
  }

  void AudioComponent::Initialize()
  {
    LoadSound("FireA.ogg");
    gameObject->AudioComponent = this;
    AUDIOSYSTEM->AddAudioComponent(this);
  }

  void AudioComponent::LoadSound(std::string name)
  {
    newSound = AUDIOSYSTEM->LoadSound(name.c_str(), "Test", Sound::SOUND_3D, 1.0f);
  }

  void AudioComponent::PlaySound()
  {
    playing = true;
    positional = true;
    lowpassed = true;
    lowcutoff = 6000;
    lowresonance = 1;
    highpassed = false;
    highcutoff = 150;
    highresonance = 1;
    micEffect = true;
  }

  void AudioComponent::Update()
  {
    if (positional)
      newSound->SetPosition(gameObject->Transform->GetPosition());

    if (playing)
      newSound->Play();
    else
      newSound->Stop();

    if (lowpassed && playing)
    {
      newSound->LowPassFilter();
      newSound->SetLPF(lowcutoff, lowresonance);

      lowpassed = false;
    }

    if (highpassed && playing)
    {
      newSound->HighPassFilter();
      newSound->SetHPF(highcutoff, highresonance);

      highpassed = false;
    }

    if (micEffect && playing)
      newSound->micEffectUpdate();

    newSound->SetMute(mute);

    if (CORE->IsPaused() == true)
    {
      AUDIOSYSTEM->SetPauseMenuEffect(500.0f, 2.5f, 0.5f);
    }
    else
    {
      AUDIOSYSTEM->SetPauseMenuEffect(22000.0f, 0.0f, 4.0f);
    }
  }

  #pragma endregion


  /*---------------------------------------------------------------------------
  // Static Public Variables
  ---------------------------------------------------------------------------*/
  #pragma region Static Public Variables

  #pragma endregion


  /*---------------------------------------------------------------------------
  // Static Public Functions
  ---------------------------------------------------------------------------*/
  #pragma region Static Public Functions

  #pragma endregion


  /*---------------------------------------------------------------------------
  // Destructor
  ---------------------------------------------------------------------------*/
  #pragma region Destructor

  #pragma endregion


  /*---------------------------------------------------------------------------
  // Private Variables
  ---------------------------------------------------------------------------*/
  #pragma region Private Variables

  #pragma endregion


  /*---------------------------------------------------------------------------
  // Private Structs
  ---------------------------------------------------------------------------*/
  #pragma region Private Structs

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Private Functions
  ---------------------------------------------------------------------------*/
  #pragma region Private Functions

  #pragma endregion


  /*---------------------------------------------------------------------------
  // Static Functions
  ---------------------------------------------------------------------------*/
  #pragma region Static Functions

  AudioComponent::~AudioComponent()
  {
    AUDIOSYSTEM->DeleteAudioComponent(this);
  }

  #pragma endregion

}

//-----------------------------------------------------------------------------

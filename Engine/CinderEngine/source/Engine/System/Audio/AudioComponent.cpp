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
    _positional = false;
    _playing = false;
    _mute = false;
    _lowpassed = false;
    _lowcutoff = 6000;
    _lowresonance = 1;
    _highpassed = false;
    _highcutoff = 150;
    _highresonance = 1;
    _micEffect = false;
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
    Serializer::DataNode* value = data->FindElement(data, "Positional");
    value->GetValue(&_positional);

    value = data->FindElement(data, "MicEffect");
    value->GetValue(&_micEffect);

    value = data->FindElement(data, "Play");
    value->GetValue(&_playing);

    value = data->FindElement(data, "Mute");
    value->GetValue(&_mute);

    value = data->FindElement(data, "LowPassFilter");
    value->GetValue(&_lowpassed);

    value = data->FindElement(data, "LPFcutoff");
    value->GetValue(&_lowcutoff);

    value = data->FindElement(data, "LPFresonance");
    value->GetValue(&_lowresonance);

    value = data->FindElement(data, "HighPassFilter");
    value->GetValue(&_highpassed);

    value = data->FindElement(data, "HPFcutoff");
    value->GetValue(&_highcutoff);

    value = data->FindElement(data, "HPFresonance");
    value->GetValue(&_highresonance);

    value = data->FindElement(data, "FileName");
    value->GetValue(&_fileName);

    value = data->FindElement(data, "SoundID");
    value->GetValue(&_soundID);

    value = data->FindElement(data, "SoundName");
    value->GetValue(&_soundName);

    if (_soundID == "BG")
      _type = Sound::MUSIC;
    else if (_soundID == "2D")
      _type = Sound::SOUND_2D;
    else if (_soundID == "3D")
      _type = Sound::SOUND_3D;    

    _newSound = AUDIOSYSTEM->LoadSound(_fileName.c_str(), const_cast<char*>(_soundName.c_str()), _type, 1.0f);
  }

  void AudioComponent::Initialize()
  {
    //LoadSound("FireA.ogg");
    gameObject->AudioComponent = this;
    AUDIOSYSTEM->AddAudioComponent(this);
  }

  void AudioComponent::LoadSound(std::string name)
  {
    _newSound = AUDIOSYSTEM->LoadSound(name.c_str(), "Test", Sound::SOUND_3D, 1.0f);
  }

  void AudioComponent::PlaySound()
  {
    _playing = true;
    _positional = true;
    _lowpassed = true;
    _lowcutoff = 6000;
    _lowresonance = 1;
    _highpassed = false;
    _highcutoff = 150;
    _highresonance = 1;
    _micEffect = true;
  }

  void AudioComponent::Update()
  {
    if (_positional)
      _newSound->SetPosition(gameObject->Transform->GetPosition());

    if (_playing)
      _newSound->Play();

    if (_lowpassed && _playing)
    {
      _newSound->LowPassFilter();
      _newSound->SetLPF(_lowcutoff, _lowresonance);

      _lowpassed = false;
    }

    if (_highpassed && _playing)
    {
      _newSound->HighPassFilter();
      _newSound->SetHPF(_highcutoff, _highresonance);

      _highpassed = false;
    }

    if (_micEffect && _playing)
      _newSound->micEffectUpdate();

    if (_playing)
      _newSound->SetMute(_mute);
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

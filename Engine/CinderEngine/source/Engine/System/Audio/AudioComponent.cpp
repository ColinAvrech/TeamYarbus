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

    value = data->FindElement(data, "Play");
    value->GetValue(&playing);

    value = data->FindElement(data, "Mute");
    value->GetValue(&mute);

    value = data->FindElement(data, "SoundID");
    value->GetValue(&soundName);

    newSound = Resources::RS->Get_Sound(soundName);
  }

  void AudioComponent::Initialize()
  {
    gameObject->AudioComponent;
    AUDIOSYSTEM->AddAudioComponent(this);
  }

  void AudioComponent::Update()
  {
    if (positional)
    {
      newSound->SetPosition(gameObject->Transform->GetPosition());
    }


    if (playing)
      newSound->Play();
    else
      newSound->Stop();

      newSound->SetMute(mute);
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

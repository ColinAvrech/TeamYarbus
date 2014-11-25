/*****************************************************************************/
/*!
\file   AudioEvents.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Creates the infrastructure for the Audio Events
*/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include "AudioEvents.h"

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

  AudioEvents::AudioEvents()
  {
  
  }
  #pragma endregion

  /*---------------------------------------------------------------------------
  // Public Variables
  ---------------------------------------------------------------------------*/
  #pragma region Public Variables

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

  void AudioEvents::Initialize()
  {
    EVENTSYSTEM->mConnect<KeyEvent, AudioEvents>(Events::KEY_TAB, this, &AudioEvents::AudioEventsUpdate);
  }

  void AudioEvents::AudioEventsUpdate(KeyEvent* e)
  {
    std::cout << "blah" << std::endl;

    if (e->ALTPressed && e->KeyDown)
    {
      std::cout << "blah2.0" << std::endl;
      AUDIOSYSTEM->SetPaused(true, Sound::SFX_ALL);
    }

    if (e->ALTPressed && !e->KeyDown)
    {
      AUDIOSYSTEM->SetPaused(false, Sound::SFX_ALL);
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

  AudioEvents::~AudioEvents()
  {
  
  }

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

  #pragma endregion

} // namespace Framework

//-----------------------------------------------------------------------------
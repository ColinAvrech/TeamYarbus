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
#include "KeyEvent.h"
#include "EventSystem.h"
#include "WindowFocusEvent.h"
#include "UpdateEvent.h"
#include "KeyEvent.h"
#include "WindowSystem.h"
#include "Core.h"


#pragma endregion

namespace Framework
{
  /*---------------------------------------------------------------------------
  // Class Implementation
  ---------------------------------------------------------------------------*/

  AudioEvents *AUDIOEVENTS = nullptr;
  /*---------------------------------------------------------------------------
  // Constructors
  ---------------------------------------------------------------------------*/
  #pragma region Constructors

  Sound *test, *test2;

  AudioEvents::AudioEvents()
  {
    ErrorIf(AUDIOEVENTS != nullptr, "ERROR created AudioEvents twice!");
    AUDIOEVENTS = this;
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
    EVENTSYSTEM->mConnect<WindowFocusEvent, AudioEvents>(Events::WINDOWFOCUSEVENT, this, &AudioEvents::AudioEventsUpdate);
    EVENTSYSTEM->mConnect<KeyEvent, AudioEvents>(Events::KEY_ANY, this, &AudioEvents::OnKeyPressed);
    EVENTSYSTEM->mConnect<UpdateEvent, AudioEvents>(Events::UPDATEEVENT, this, &AudioEvents::OnUpdate);

    //test = AUDIOSYSTEM->LoadSound("CreditsMusic.wav", "NOISE", Sound::MUSIC, 1.0f);
    //test2 = AUDIOSYSTEM->LoadSound("MainMenuScroll.wav", "meh", Sound::SOUND_2D, 1.0f);
    //test->Play();
  }

  void AudioEvents::AudioEventsUpdate(WindowFocusEvent* e)
  {
    if (e == nullptr)
      return;

    if (e->InFocus)
    {      
      AUDIOSYSTEM->SetPaused(false, Sound::SFX_ALL);
      SetPauseUnmanagedSounds(false);
      std::cout << CinderConsole::red << "PAUSE OFF" << std::endl;
    }
    else
    {
      AUDIOSYSTEM->SetPaused(true, Sound::SFX_ALL);
      SetPauseUnmanagedSounds(true);
      std::cout << CinderConsole::green << "PAUSE ON" << std::endl;
    }
  }

  void AudioEvents::OnKeyPressed(KeyEvent* key)
  {
    if (key->KeyDown)
    {
      switch (key->KeyValue)
      {
      case GLFW_KEY_UP:
        //test->Play();
        break;

      case GLFW_KEY_DOWN:
        //test2->Play();
        break;

      default:
        break;
      }
    }    
  }

  void AudioEvents::OnUpdate(UpdateEvent *e)
  {
    for (auto Sound : unmanagedSounds)
    {
      if (!Sound->GetPlaying())
      {
        Sound->Stop();
      }
    }
  }

  void AudioEvents::SetPauseUnmanagedSounds(bool paused)
  {
    for (auto Sound : unmanagedSounds)
    {
      Sound->Get_Channel()->setPaused(paused);
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
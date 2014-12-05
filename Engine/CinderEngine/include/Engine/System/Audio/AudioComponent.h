/*****************************************************************************/
/*!
\file   AudioComponent.h
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Header file for the AudioSystem Class
*/
/*****************************************************************************/

#ifndef _AUDIO_COMPONENT
#define _AUDIO_COMPONENT

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include "Component.h"
#include "JSONSerializer.h"
#include "Sound.h"

#pragma endregion

namespace Framework
{
  class Sound;

  /*---------------------------------------------------------------------------
  // Class
  ---------------------------------------------------------------------------*/
  class AudioComponent : public  Component
  {
    public:

      /*-----------------------------------------------------------------------
      // Constructors
      -----------------------------------------------------------------------*/
      #pragma region Constructors

      AudioComponent();

      #pragma endregion

      /*-----------------------------------------------------------------------
      // Public Variables
      -----------------------------------------------------------------------*/
      #pragma region Public Variables

      static const string Name;
      glm::vec3     position;

      #pragma endregion

      /*---------------------------------------------------------------------*/
      // Public Structs/Enums
      /*---------------------------------------------------------------------*/
      #pragma region Public Structs      

      #pragma endregion

      /*-----------------------------------------------------------------------
      // Public Functions
      -----------------------------------------------------------------------*/
      #pragma region Public Functions

      virtual void Serialize(Serializer::DataNode*);
      virtual void Initialize();
      void LoadSound(std::string name);
      void PlaySound();
      void Update();

      #pragma endregion


      /*-----------------------------------------------------------------------
      // Static Public Variables
      -----------------------------------------------------------------------*/
      #pragma region Static Public Variables

      #pragma endregion


      /*-----------------------------------------------------------------------
      // Static Public Functions
      -----------------------------------------------------------------------*/
      #pragma region Static Public Functions

      #pragma endregion


      /*-----------------------------------------------------------------------
      // Destructor
      -----------------------------------------------------------------------*/
      #pragma region Destructors

      ~AudioComponent();

      #pragma endregion

    private:

      /*-----------------------------------------------------------------------
      // Private Variables
      -----------------------------------------------------------------------*/
      #pragma region Private Variables

      bool           _positional;
      bool           _playing;
      bool           _mute = false;
      bool           _lowpassed;
      float          _lowcutoff;
      float          _lowresonance;
      bool           _highpassed;
      float          _highcutoff;
      float          _highresonance;
      bool           _micEffect;
      Sound          *_newSound;
      string         _fileName;
      string         _soundID;
      string         _soundName;
      Sound::SoundID _type;
      bool           _played = false;

      #pragma endregion


      /*-----------------------------------------------------------------------
      // Private Structs
      -----------------------------------------------------------------------*/
      #pragma region Private Structs    

      #pragma endregion

      /*-----------------------------------------------------------------------
      // Private Functions
      -----------------------------------------------------------------------*/
      #pragma region Private Functions

      #pragma endregion


      /*-----------------------------------------------------------------------
      // Static Functions
      -----------------------------------------------------------------------*/
      #pragma region Static Functions

      #pragma endregion

  };
}

//-----------------------------------------------------------------------------

#endif				// _AUDIO_COMPONENT
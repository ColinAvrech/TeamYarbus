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
      void AudioComponent::Update();

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

      bool          positional;
      bool          playing;
      bool          mute = false;
      bool          lowpassed;
      float         lowcutoff;
      float         lowresonance;
      bool          highpassed;
      float         highcutoff;
      float         highresonance;
      bool          micEffect;
      Sound         *newSound;

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
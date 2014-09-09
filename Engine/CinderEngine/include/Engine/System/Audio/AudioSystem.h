/*****************************************************************************/
/*!
\file   AudioSystem.h
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Header file for the AudioSystem Class
*/
/*****************************************************************************/

#ifndef _AUDIO_H
#define _AUDIO_H

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include "Common.h"
#include "fmod.hpp"
#include "fmod_DSP.h"
#include "fmod_errors.h"
#include "Sound.h"

#pragma endregion

namespace Framework
{
  /*---------------------------------------------------------------------------
  // Class
  ---------------------------------------------------------------------------*/
  class AudioSystem : public BaseSystem
  {
    public:

      /*-----------------------------------------------------------------------
      // Constructors
      -----------------------------------------------------------------------*/
      #pragma region Constructors

      AudioSystem();

      #pragma endregion

      /*-----------------------------------------------------------------------
      // Public Variables
      -----------------------------------------------------------------------*/
      #pragma region Public Variables

      char* AudioAssetsPath = "../../Assets/Audio/";

      #pragma endregion

      /*---------------------------------------------------------------------*/
      // Public Structs
      /*---------------------------------------------------------------------*/
      #pragma region Public Structs

      #pragma endregion

      /*-----------------------------------------------------------------------
      // Public Functions
      -----------------------------------------------------------------------*/
      #pragma region Public Functions
    
      void          ErrCheck(FMOD_RESULT result);
      bool          Initialize();
      Sound*        LoadSound(char* filename,       char* soundName, 
                              Sound::SoundID type,  float volume);
      void          LoadAllSounds();
      void          StopSounds(int id);

      // Called every frame
      virtual void  Update(const double dt);

      // Returns name of System
      virtual const std::string GetName(){ return "AudioSystem";}

      // Getters
      Sound*        GetSound(char* soundName);
      bool          GetMuted(int id);
    
      // Setters
      void          SetPaused(bool paused, int id);    

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
      virtual ~AudioSystem();

      #pragma endregion

    private:

      /*-----------------------------------------------------------------------
      // Private Variables
      -----------------------------------------------------------------------*/
      #pragma region Private Variables

      std::map<std::string, Sound*>     soundMap;
      FMOD::System                      *pFMODAudioSystem = NULL;
      FMOD::ChannelGroup                *GroupMusic;
      FMOD::ChannelGroup                *Group2DSFX;
      FMOD::ChannelGroup                *Group3DSFX;

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

      bool CreateFMODSystem();
      void CreateSoundGroups();

      #pragma endregion


      /*-----------------------------------------------------------------------
      // Static Functions
      -----------------------------------------------------------------------*/
      #pragma region Static Functions

      #pragma endregion

  };

  extern AudioSystem *AUDIOSYSTEM;
}

//-----------------------------------------------------------------------------

#endif				// _AUDIO_H
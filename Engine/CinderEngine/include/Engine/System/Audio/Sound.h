/*****************************************************************************/
/*!
\file   Sound.h
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Header file for the Sound Class
*/
/*****************************************************************************/

#ifndef _SOUND_H
#define _SOUND_H

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include "common.h"
#include "fmod.hpp"
#include "fmod_DSP.h"
#include "fmod_errors.h"

#pragma endregion

namespace Framework
{
  /*---------------------------------------------------------------------------
  // Class
  ---------------------------------------------------------------------------*/
  class Sound
  {
    public:
    
      /*-----------------------------------------------------------------------
      // Constructors
      -----------------------------------------------------------------------*/
      #pragma region Constructors

      Sound();

      #pragma endregion

      /*-----------------------------------------------------------------------
      // Public Variables
      -----------------------------------------------------------------------*/
      #pragma region Public Variables

      static bool system_on_;

      #pragma endregion

      /*---------------------------------------------------------------------*/
      // Public Structs/Enums
      /*---------------------------------------------------------------------*/
      #pragma region Public Structs/Enums

      enum SoundID
      {
        SOUND_2D = 0,
        SOUND_3D,
        SFX_ALL,
        MUSIC
      };

      struct objects_DSP
      {
        FMOD::DSP                     *dsp_lpf = 0;
        FMOD::DSP                     *dsp_hpf = 0;
        FMOD::DSP                     *dsp_reverb = 0;
      }objects_DSP;

      #pragma endregion   

      /*-----------------------------------------------------------------------
      // Public Functions
      -----------------------------------------------------------------------*/
      #pragma region Public Functions

      void            ErrCheck(FMOD_RESULT result);
      void            Load(FMOD::System* pSystem, 
                           char* filename, 
                           char* soundName, 
                           SoundID id);
      void            Play();
      void            PlayNew();
      void            AddSound(char* filename);
      virtual void    Update(const double dt);  

      // DSP
      void            LowPassFilter(float cutoff = 5000.0, float resonance = 1.0);
      void            HighPassFilter(float cutoff = 5000.0, float resonance = 1.0);
      void            Reverb(
                              float Wet = -6,
                              float Dry = 0,
                              float Diffusion = 100,
                              float Density = 100,
                              float Decay = 1500,
                              float EarlyR = 20,
                              float Delay = 40,
                              float HF_ref = 5000,
                              float HF_decay = 50,                              
                              float LowShelf_Hz = 250,
                              float LowShelf_Gain = 0,
                              float HighCut = 20000,
                              float EarlyLateMix = 50                              
                            );

      // Setters
      void            SetChannelGroup(FMOD::ChannelGroup* channelGroup);      
      void            SetVolume(const float volume);
      void            SetLoop(const bool loopState, int index);
      void            SetLoopPoints(const float startPos, 
                                    const float endPos = 0, 
                                    const int index = 0);
      void            SetType(unsigned type, unsigned index);

      // Getters
      float           GetMasterVolume(void);
      bool            GetPlaying();
      bool            GetPaused();      
      float           GetVolume();      
      unsigned        GetID(){return ID;}

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

      ~Sound();

      #pragma endregion   

    private:

      /*-----------------------------------------------------------------------
      // Private Variables
      -----------------------------------------------------------------------*/
      #pragma region Private Variables

      unsigned                      ID;
      float                         _volume;
      float                         _paused;
      float                         _pitch;
      std::vector<FMOD::Sound*>     sound_queue_;
      FMOD::Channel                 *pChannel;
      FMOD::ChannelGroup            *pChannelGroup;
      FMOD::System                  *pFMODAudioSystem;
      std::string                   _soundName;      
      FMOD_VECTOR                   _position;

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

#endif        // _SOUND_H
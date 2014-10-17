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
      FMOD::Sound *pSound;

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

      enum ReverbPresetName
      {
        OFF,              GENERIC,      PADDEDCELL,     ROOM,
        BATHROOM,         LIVINGROOM,   STONEROOM,      AUDITORIUM,
        CONCERTHALL,      CAVE,         ARENA,          HANGAR,
        CARPETTEDHALLWAY, HALLWAY,      STONECORRIDOR,  ALLEY,
        FOREST,           CITY,         MOUNTAINS,      QUARRY,
        PLAIN,            PARKINGLOT,   SEWERPIPE,      UNDERWATER
      };

      struct objects_DSP
      {
        FMOD::DSP                     *dsp_lpf = 0;
        FMOD::DSP                     *dsp_hpf = 0;
        FMOD::DSP                     *dsp_reverb = 0;
        FMOD::DSP                     *dsp_noise = 0;
      }objects_DSP;

      struct ReverbPreset
      {
        float DecayTime;
        float EarlyDelay;
        float LateDelay;
        float HFReference;
        float HFDecayRatio;
        float Diffusion;
        float Density;
        float LowShelfFrequency;
        float LowShelfGain;
        float HighCut;
        float EarlyLateMix;
        float WetLevel;
        float DryLevel;
      };

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
      void            VolumeFade(float volume, float fadeTime);
      void            Stop();
      void            PlayNew();
      void            AddSound(char* filename);
      virtual void    Update(const double dt);
      void            UpdateVolumeFade(const double dt);

      // DSP
      void            LowPassFilter();
      void            HighPassFilter();
      void            GenerateNoise();
      void            Reverb();

      // Setters
      ReverbPreset    SetReverbPreset(ReverbPresetName preset);
      void            SetChannelGroup(FMOD::ChannelGroup* channelGroup);          
      void            SetVolume(const float volume);
      void            SetMute(bool muteState);
      void            SetPause(bool pauseState);
      void            SetPan(char pan);
      void            SetLoop(const bool loopState, int index);
      void            SetLoopPoints(const float startPos, 
                                    const float endPos = 0, 
                                    const int index = 0);
      void            SetType(unsigned type, unsigned index);
      void            SetLPF(float cutoff = 5000.0f, float resonance = 1.0f);
      void            SetHPF(float cutoff = 5000.0f, float resonance = 1.0f);
      void            AddReverbPreset(Sound::ReverbPresetName preset);

      // Getters
      float           GetMasterVolume(void);
      bool            GetPlaying();
      bool            GetPaused();
      float           GetVolume();   
      unsigned        GetTime();
      unsigned        GetID(){return ID;}
      float*          GetVolumePtr();

      inline FMOD::Channel* Get_Channel()
      {
        return pChannel;
      }

      inline FMOD::DSP* Get_LPF ()
      {
        return objects_DSP.dsp_lpf;
      }

      inline FMOD::DSP* Get_HPF()
      {
        return objects_DSP.dsp_hpf;
      }

      inline FMOD::DSP* Get_RVRB()
      {
        return objects_DSP.dsp_reverb;
      }

      inline FMOD::DSP* Get_NOISE()
      {
        return objects_DSP.dsp_noise;
      }

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
      float                         _volValue;
      float                         _fadeValue;
      bool                          test = true;
      bool                          test2 = false;
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
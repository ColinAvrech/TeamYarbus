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

#include "BaseSystem.h"
#include "Sound.h"

#pragma endregion

#define LATENCY_MS (50)

namespace Framework
{

  class Resources;
  class AudioComponent;
  class Transform;

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

      char* AudioAssetsPath = "../../Resources/Audio/";      
      FMOD_DSP_METERING_INFO input; // Microphone input values
      Transform* listener;

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
    
      void          ErrCheck(FMOD_RESULT result);
      bool          Initialize();
      Sound*        LoadSound(const char* filename,  char* soundName, 
                              Sound::SoundID type,  float volume);      
      void          StopSounds(int id);
      void          AddAudioComponent(AudioComponent* component);
      void          DeleteAudioComponent(AudioComponent* component);

      // Pause Menu Effect
      void          InitPauseMenuEffect();
      void          SetPauseMenuEffect(float cutoff, float resonance, float time);
      void          UpdatePauseMenuEffect(const double dt);

      // Microphone Functions
      void          InitMicData();
      void          LoadMicData();
      void          UpdateMicData();
      void          micMeter();
      void          micFrequencyData();
      void          micFilter(float cutoff = 150, float resonance = 1);
      void          meterConsoleOut();
      void          frequencyConsoleOut();
      void          latencyConsoleOut();
      bool          checkMicrophone();

      // Called every frame
      virtual void  Update(const double &dt);
      

      // Returns name of System
      virtual const string GetName(){ return "AudioSystem";}

      // Getters
      Sound*        GetSound(char* soundName);
      bool          GetMuted(int id = Sound::SFX_ALL);
      float         GetMicrophoneValue ();
    
      // Setters
      void          SetPaused(bool paused = true, int id = Sound::SFX_ALL);    

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

      std::map<string, Sound*>          soundMap;
      std::list<AudioComponent*>        audioComponentList;
      FMOD::System                      *pFMODAudioSystem = NULL;
      FMOD::ChannelGroup                *GroupMusic;
      FMOD::ChannelGroup                *Group2DSFX;
      FMOD::ChannelGroup                *Group3DSFX;
      FMOD::DSP                         *fft;
      FMOD::DSP                         *meter;
      FMOD::DSP                         *filter;
      FMOD::DSP                         *lowPassEffect;
      FMOD::Sound                       *micSound = 0;
      FMOD::Channel                     *micChannel = 0;
      Sound                             *_newSound;
      FMOD_CREATESOUNDEXINFO            _exinfo;
      unsigned int                      _soundlength;
      unsigned int                      _recordpos;
      unsigned int                      _recorddelta;
      unsigned int                      _minrecorddelta;
      unsigned int                      _lastrecordpos;
      unsigned int                      _samplesrecorded;
      unsigned int                      _playpos;
      unsigned int                      _adjustedlatency;
      unsigned int                      _driftthreshold;
      int                               _recordrate;
      int                               _recordchannels;
      int                               _recordnumdrivers;
      float                             _smootheddelta;  
      float                             _cutoffVal;
      float                             _resonanceVal;
      float                             _fadeValA;
      float                             _fadeValB;
      bool                              _check;
      bool                              _lpfstate = false;

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
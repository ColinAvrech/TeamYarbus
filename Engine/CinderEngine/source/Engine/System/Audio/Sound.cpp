/*****************************************************************************/
/*!
\file   Sound.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This file has all the method implementations for the Sound class
*/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include "Sound.h"
#include <Windows.h>
#include <vector>

#pragma endregion

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

namespace Framework
{
  /*---------------------------------------------------------------------------
  // Class Implementation
  ---------------------------------------------------------------------------*/
  
  /*---------------------------------------------------------------------------
  // Constructors
  ---------------------------------------------------------------------------*/
  #pragma region Constructors  

  Sound::Sound(void) : ID(0),
    pChannel(NULL), pChannelGroup(NULL), _volume(1.0f),
    pFMODAudioSystem(NULL)

  {
  }

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Public Variables
  ---------------------------------------------------------------------------*/
  #pragma region Public Variables

  bool Sound::system_on_ = true;

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

  /***************************************************************************/
  /*!
  \brief  Sets a channel group to the Sound Object

  \param channelGroup
  Takes in a pointer to an FMOD channel group

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::SetChannelGroup(FMOD::ChannelGroup* channelGroup)
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
      return; 
    }

    pChannelGroup = channelGroup;
  }

  /***************************************************************************/
  /*!
  \brief  Checks for FMOD related errors

  \param  FMOD_RESULT
  Type of error

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::ErrCheck(FMOD_RESULT result)
  {
    if (result != FMOD_OK)
    {
      char buffer[1000];

      sprintf(buffer, "FMOD error! (%d) %s\n",
      result, FMOD_ErrorString(result));

      // Prints the buffer to Visual Studio's output window
      OutputDebugStringA(buffer);
    }
  }

  /***************************************************************************/
  /*!
  \brief  Sets the volume to the Sound Object

  \param  volume
  The float value of the volume

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::SetVolume(const float volume)
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    {
      return; 
    }

    _volume = volume;

    if (pChannel)
      pChannel->setVolume(volume);
  }

  void Sound::SetPause(bool pauseState)
  {
    if (Sound::system_on_ == false)
    { 
      return; 
    }

    if (pChannel)
    {
      if (pauseState == true)
      {
        pChannel->setPaused(true);
        _paused = true;
      }
      else if (pauseState == false)
      {
        pChannel->setPaused(false);
        _paused = false;
      }
      
    }
  }

  void Sound::Stop()
  {
    if (Sound::system_on_ == false)
    { 
      return; 
    }

    if (pChannel)
    {
      bool playing = false;
      pChannel->isPlaying(&playing);
      if (playing)
      {
        ErrCheck(pChannel->stop());
        pChannel = NULL;
      }
    }
  }

  void Sound::SetMute(bool muteState)
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    {
      return;
    }

    if (pChannel)
    {
      if (muteState == true)
      {
        pChannel->setMute(muteState);
      }
      else if (muteState == false)
      {
        pChannel->setMute(muteState);
      }
    }    
  }

  /***************************************************************************/
  /*!
  \brief  Attaches a Low Pass Filter to the Signal Chain

  \param  cutoff
  Lowpass cutoff frequency in hz. 10.0 to 22000.0

  \param  resonance
  Lowpass resonance Q value. 1.0 to 10.0

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::LowPassFilter(float cutoff, float resonance)
  {
    FMOD_RESULT result;
    bool active;

    // Checks if system is not on
    if (Sound::system_on_ == false)
    {
      return;
    }

    result = pFMODAudioSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &objects_DSP.dsp_lpf);
    ErrCheck(result);

    // Lowpass cutoff frequency in hz.   10.0 to 22000.0
    result = objects_DSP.dsp_lpf->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, cutoff);
    ErrCheck(result);

    // Lowpass resonance Q value. 1.0 to 10.0
    result = objects_DSP.dsp_lpf->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, resonance);
    ErrCheck(result);

    result = objects_DSP.dsp_lpf->getActive(&active);
    ErrCheck(result);

    if (active)
    {
      result = pChannel->removeDSP(objects_DSP.dsp_lpf);
      ErrCheck(result);
    }
    else
    {
      result = pChannel->addDSP(0, objects_DSP.dsp_lpf, 0);
      ErrCheck(result);
    }
  }

  /***************************************************************************/
  /*!
  \brief  Attaches a High Pass Filter to the Signal Chain

  \param  cutoff
  Highpass cutoff frequency in hz. 
  Ranges from 1.0 to 22000.0

  \param  resonance
  Highpass resonance Q value. 
  Ranges from 1.0 to 10.0

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::HighPassFilter(float cutoff, float resonance)
  {
    FMOD_RESULT result;
    bool active;

    // Checks if system is not on
    if (Sound::system_on_ == false)
    {
      return;
    }

    result = pFMODAudioSystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &objects_DSP.dsp_hpf);
    ErrCheck(result);

    // Highpass cutoff frequency in hz.  1.0 to output 22000.0
    result = objects_DSP.dsp_hpf->setParameterFloat(FMOD_DSP_HIGHPASS_CUTOFF, cutoff);
    ErrCheck(result);

    // Highpass resonance Q value.  1.0 to 10.0
    result = objects_DSP.dsp_hpf->setParameterFloat(FMOD_DSP_HIGHPASS_RESONANCE, resonance);
    ErrCheck(result);

    result = objects_DSP.dsp_hpf->getActive(&active);
    ErrCheck(result);

    if (active)
    {
      result = pChannel->removeDSP(objects_DSP.dsp_hpf);
      ErrCheck(result);
    }
    else
    {
      result = pChannel->addDSP(0, objects_DSP.dsp_hpf, 0);
      ErrCheck(result);
    }
  }

  void Sound::GenerateNoise()
  {
    FMOD_RESULT result;
    bool active;

    // Checks if system is not on
    if (Sound::system_on_ == false)
    {
      return;
    }

    result = pFMODAudioSystem->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &objects_DSP.dsp_noise);
    ErrCheck(result);

    result = pFMODAudioSystem->playDSP(objects_DSP.dsp_noise, 0, false, &pChannel);
    ErrCheck(result);

    result = objects_DSP.dsp_noise->setParameterInt(FMOD_DSP_OSCILLATOR_TYPE, 5);
    ErrCheck(result);

    result = objects_DSP.dsp_noise->getActive(&active);
    ErrCheck(result);

    if (active)
    {
      result = pChannel->removeDSP(objects_DSP.dsp_noise);
      ErrCheck(result);
    }
    else
    {
      result = pChannel->addDSP(0, objects_DSP.dsp_noise, 0);
      ErrCheck(result);
    }
  }

  /***************************************************************************/
  /*!
  \brief  Emulates artificial reverb to the overall mix

  \param  Wet
  Reverb signal level in dB.  
  Ranges from -80.0 to 20.0

  \param  Dry
  Dry signal level in dB.  
  Ranges from -80.0 to 20.0

  \param  Diffusion
  Reverberation diffusion (echo density) in percent.  
  Ranges from 0.0 to 100.0

  \param  Density
  Reverberation density (modal density) in percent.  
  Ranges from 0.0 to 100.0

  \param  Decay
  Reverberation decay time at low-frequencies in milliseconds. 
  Ranges from 100.0 to 20000.0

  \param  EarlyR
  Delay time of first reflection in milliseconds.  
  Ranges from 0.0 to 300.0

  \param  Delay
  Late reverberation delay time relative to first reflection in milliseconds. 
  Ranges from 0.0 to 100.0

  \param  HF_ref
  eference frequency for high-frequency decay in Hz.  
  Ranges from 20.0 to 20000.0

  \param  HF_decay
  High-frequency decay time relative to decay time in percent.  
  Ranges from 10.0 to 100.0

  \param  LowShelf_Hz
  Transition frequency of low-shelf filter in Hz.  
  Ranges from 20.0 to 1000.0

  \param  LowShelf_Gain
  Gain of low-shelf filter in dB.  
  Ranges from -36.0 to 12.0

  \param  HighCut
  Cutoff frequency of low-pass filter in Hz.  
  Ranges from 20.0 to 20000.0

  \param  EarlyLateMix
  Blend ratio of late reverb to early reflections in percent.  
  Ranges from 0.0 to 100.0

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::Reverb(
                      float Wet,
                      float Dry,
                      float Diffusion,
                      float Density,
                      float Decay,
                      float EarlyR,
                      float Delay,
                      float HF_ref,
                      float HF_decay,
                      float LowShelf_Hz,
                      float LowShelf_Gain,
                      float HighCut,
                      float EarlyLateMix
                    )
  {
    FMOD_RESULT result;
    bool active;

    // Checks if system is not on
    if (Sound::system_on_ == false)
    {
      return;
    }

    result = pFMODAudioSystem->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &objects_DSP.dsp_reverb);
    ErrCheck(result);

    // Reverb signal level in dB.  Ranges from -80.0 to 20.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_WETLEVEL, Wet);
    ErrCheck(result);
    // Dry signal level in dB.  Ranges from -80.0 to 20.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_DRYLEVEL, Dry);
    ErrCheck(result);

    // Reverberation diffusion (echo density) in percent.  Ranges from 0.0 to 100.0.
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_DIFFUSION, Diffusion);
    ErrCheck(result);

    // Reverberation density (modal density) in percent.  Ranges from 0.0 to 100.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_DENSITY, Density);
    ErrCheck(result);

    // Reverberation decay time at low-frequencies in milliseconds.  Ranges from 100.0 to 20000.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_DECAYTIME, Decay);
    ErrCheck(result);

    // Delay time of first reflection in milliseconds.  Ranges from 0.0 to 300.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYDELAY, EarlyR);
    ErrCheck(result);

    // Late reverberation delay time relative to first reflection in milliseconds.  Ranges from 0.0 to 100.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_LATEDELAY, Delay);
    ErrCheck(result);

    // Reference frequency for high-frequency decay in Hz.  Ranges from 20.0 to 20000.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_HFREFERENCE, HF_ref);
    ErrCheck(result);

    // High-frequency decay time relative to decay time in percent.  Ranges from 10.0 to 100.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_HFDECAYRATIO, HF_decay);
    ErrCheck(result);    

    // Transition frequency of low-shelf filter in Hz.  Ranges from 20.0 to 1000.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_LOWSHELFFREQUENCY, LowShelf_Hz);
    ErrCheck(result);

    // Gain of low-shelf filter in dB.  Ranges from -36.0 to 12.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_LOWSHELFGAIN, LowShelf_Gain);
    ErrCheck(result);

    // Cutoff frequency of low-pass filter in Hz.  Ranges from 20.0 to 20000.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_HIGHCUT, HighCut);
    ErrCheck(result);

    // Blend ratio of late reverb to early reflections in percent.  Ranges from 0.0 to 100.0
    result = objects_DSP.dsp_reverb->setParameterFloat(FMOD_DSP_SFXREVERB_EARLYLATEMIX, EarlyLateMix);
    ErrCheck(result);

    result = objects_DSP.dsp_reverb->getActive(&active);
    ErrCheck(result);

    if (active)
    {
      result = pChannel->removeDSP(objects_DSP.dsp_reverb);
      ErrCheck(result);
    }
    else
    {
      result = pChannel->addDSP(0, objects_DSP.dsp_reverb, 0);
      ErrCheck(result);
    }
  }

  /***************************************************************************/
  /*!
  \brief  Sets the loop state of the Sound Object

  \param  loopState
  Bool to state if we want to have looping

  \param index
  The index number of the sound we want to set to loop

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::SetLoop(const bool loopState, int index)
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
      return; 
    }

    // Adds the sound object to the sound queue
    FMOD::Sound* pSound = sound_queue_[index];

    if (loopState == true)
    {
      pSound->setMode(FMOD_LOOP_NORMAL); // Sets Loop On
    }
    else
    {
      pSound->setMode(FMOD_LOOP_OFF);    // Sets Loop Off
    }
  }

  /***************************************************************************/
  /*!
  \brief  Sets the loop points of the sound object

  \param  startPos
  Start of the loop

  \param endPos
  End of the loop

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::SetLoopPoints(const float startPos, 
                            const float endPos, 
                            const int index)
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
      return; 
    }

    FMOD::Sound* pSound = sound_queue_[index];

    unsigned fmStartPos = static_cast<unsigned>(startPos * 1000);
    unsigned fmEndPos = static_cast<unsigned>(endPos * 1000);

    if (!fmEndPos)
    { 
      pSound->getLength(&fmEndPos, FMOD_TIMEUNIT_MS); 
    }

    pSound->setLoopPoints(fmStartPos, FMOD_TIMEUNIT_MS, fmEndPos,
      FMOD_TIMEUNIT_MS);
  }
  /***************************************************************************/
  /*!
  \brief  Sets the mode of the Sound Object

  \param  type
  Type of Sound

  \param index
  The index number of the sound we want to assign the type to

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::SetType(unsigned type, unsigned index)
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
      return; 
    }

    // Checks if the index is larger than the size of the container
    if (sound_queue_.size() < index + 1)
      return;

    // Assigns the sound object to the sound queue
    FMOD::Sound* pSound = sound_queue_[index];

    // Sets mode of the system if it has to be 2D or 3D
    switch (type)
    {
    case SOUND_2D:
      pSound->setMode(FMOD_2D);
      SetLoop(false, index);
      break;
    case SOUND_3D:
      pSound->setMode(FMOD_3D);
      SetLoop(false, index);
      break;
    case MUSIC:
      pSound->setMode(FMOD_2D);
      SetLoop(true, index);
      break;
    }
  }

  /***************************************************************************/
  /*!
  \brief  Adds the sound to the FMOD Audio System

  \param  filename
  Name of the audio file

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::AddSound(char* filename)
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    {
      return; 
    }

    FMOD::Sound* pSound;

    // FMOD error check to check if sounds are being created in the system
    ErrCheck(pFMODAudioSystem->createSound(filename, FMOD_SOFTWARE, 0, &pSound));

    // Piles up the sound on the stack
    sound_queue_.push_back(pSound);

    unsigned index = sound_queue_.size() - 1;

    // Sets the sound object ID and index
    SetType(ID, index);
  }

  /***************************************************************************/
  /*!
  \brief  Gets the playing state of a sound

  \return Returns a bool stating if the song is playing or not
  */
  /***************************************************************************/
  bool Sound::GetPlaying()
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
      return false; 
    }

    bool playing = false; // If not playing

    // If channel exists
    if (pChannel)
    {
      pChannel->isPlaying(&playing); // Check if channel is playing
    }

    return playing;
  }

  /***************************************************************************/
  /*!
  \brief  Gets the paused state of the Sound Object

  \return Returns the paused state 
  */
  /***************************************************************************/
  bool Sound::GetPaused()
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
      return false; 
    }

    bool paused = false; // If the channel is paused

    // If the channel exists
    if (pChannel)
    {
      pChannel->getPaused(&paused); // Checks the paused state
    }

    return paused;
  }

  /***************************************************************************/
  /*!
  \brief  Plays the loaded sound

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::Play()
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
      return; 
    }

    // If the sound is already playing then return
    if (GetPlaying())
    {
      return;
    }

    // If the sound is paused
    if (GetPaused())
    {
      if (pChannel) // If the channel exists
      {
        pChannel->setPaused(false); // Unpause
      }
      return;
    }

    // Play new sound
    PlayNew();
  }

  /***************************************************************************/
  /*!
  \brief  Plays the sound Object that is loaded

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::PlayNew()
  {
    if (Sound::system_on_ == false)
    {
      return; 
    }

    FMOD::Sound* pSound = sound_queue_[0]; // Adds the new sound object

    // Calls the FMOD playSound function
    pFMODAudioSystem->playSound(pSound, 0, false, &pChannel);

    // If the channel group exists
    if (pChannelGroup)
    { 
      pChannel->setChannelGroup(pChannelGroup); // Add channel to the group
    }
    
    _paused = false; // Set paused state to false
  }

  /***************************************************************************/
  /*!
  \brief  Gets the master volume

  \return Returns a float with the volume value
  */
  /***************************************************************************/
  float Sound::GetMasterVolume(void)
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
      return 0; 
    }

    float volume = false;

    if (pChannelGroup)
    {
      pChannelGroup->getVolume(&volume); // Gets the master volume
    }
    return volume;
  }

  /***************************************************************************/
  /*!
  \brief  Gets the volume level

  \return Returns a float with the volume level
  */
  /***************************************************************************/
  float Sound::GetVolume()
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
      return 0; 
    }

    return _volume;
  }

  /***************************************************************************/
  /*!
  \brief  Loads the sound

  \param  pSystem
  The FMOD audio system

  \param filename
  Name of the file

  \param soundName
  Name of the sound

  \param id
  Id attached to the sound object

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::Load(FMOD::System* pSystem, char* filename, char* soundName, SoundID id)
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
      return; 
    }

    pFMODAudioSystem = pSystem;
    _soundName = soundName;
    ID = id;

    AddSound(filename);
  }

  /***************************************************************************/
  /*!
  \brief  Updates Instances

  \param  dt
  Frametiem of the engine

  \return Returns nothing
  */
  /***************************************************************************/
  void Sound::Update(const double dt)
  {
    // Checks if system is not on
    if (Sound::system_on_ == false)
    { 
      return; 
    }

    SetVolume(GetVolume());
  }
 
  /*---------------------------------------------------------------------------
  // Destructor
  ---------------------------------------------------------------------------*/
  Sound::~Sound()
  {
    std::vector<FMOD::Sound*>::iterator it;
    for (it = sound_queue_.begin(); it != sound_queue_.end(); ++it)
    {
      ErrCheck((*it)->release());
    }
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

}
//-----------------------------------------------------------------------------
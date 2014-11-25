/*****************************************************************************/
/*!
\file   MicrophoneSystem.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains various functions for getting microphone input
*/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#include "AudioSystem.h"

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

namespace Framework
{
  /***************************************************************************/
  /*!
  \brief  Initializes all the microphone data

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::InitMicData()
  {
    FMOD_RESULT result;

    // Initializing all the values
    _soundlength = 0;
    _recordpos = 0;
    _recorddelta = 0;
    _minrecorddelta = (unsigned int)-1;
    _lastrecordpos = 0;
    _samplesrecorded = 0;
    _playpos = 0;
    _adjustedlatency = 0;
    _driftthreshold = 0;
    _smootheddelta = 0;
    _recordrate = 0;
    _recordchannels = 1;
    _recordnumdrivers = 0;
    _check = true;
    input = {};

    // Gets information regarding the total number of recording devices in the computer
    result = pFMODAudioSystem->getRecordNumDrivers(&_recordnumdrivers);
    ErrCheck(result);

    // If no microphone is detected
    if (_recordnumdrivers <= 0)
    {
      std::cout << CinderConsole::red << "No recording devices found/plugged in!" << std::endl;
    }

    // Get microphone drivcer information
    result = pFMODAudioSystem->getRecordDriverInfo(0, NULL, 0, 0,
                                        &_recordrate, 0, &_recordchannels);
    ErrCheck(result);

    // Calculates the samplerate after applying possible latency
    _adjustedlatency = (_recordrate * LATENCY_MS) / 1000;
    _driftthreshold = _adjustedlatency / 2;
  }

  /***************************************************************************/
  /*!
  \brief  Loads the microphone and makes it usable

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::LoadMicData()
  {  
    FMOD_RESULT result;

    // Allocates a block of memory to _exinfo
    memset(&_exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    _exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    _exinfo.numchannels = 1;
    _exinfo.format = FMOD_SOUND_FORMAT_PCM16;
    _exinfo.defaultfrequency = _recordrate;
    _exinfo.length = _exinfo.defaultfrequency * sizeof(short) * _exinfo.numchannels * 5;    
    
    // Creates a new sound object dedicated to the microphone
    result = pFMODAudioSystem->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &_exinfo, &micSound);
    ErrCheck(result);

    // Starts recording the microphone input
    result = pFMODAudioSystem->recordStart(0, micSound, true);
    ErrCheck(result);

    // Gets length in PCM samples
    result = micSound->getLength(&_soundlength, FMOD_TIMEUNIT_PCM);
    ErrCheck(result);
  }

  /***************************************************************************/
  /*!
  \brief  Constantly updates the microphone

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::UpdateMicData()
  {
    FMOD_RESULT result;

    // Gets the current position of the sample
    result = pFMODAudioSystem->getRecordPosition(0, &_recordpos);
    ErrCheck(result);

    // Get the delta value for the current samples
    if (_recordpos >= _lastrecordpos)
      _recorddelta = _recordpos - _lastrecordpos;
    else
      _recorddelta = _recordpos + _soundlength - _lastrecordpos;

    // Get the last recorded sample's position and the total samples recorded
    _lastrecordpos = _recordpos;
    _samplesrecorded += _recorddelta;

    // Start playing back the recording once latency is adjusted
    // and if micChannel doesn't exist
    if (_samplesrecorded >= _adjustedlatency && !micChannel)
    {
      result = pFMODAudioSystem->playSound(micSound, 0, false, &micChannel);
      ErrCheck(result);
    }

    if (micChannel && _recorddelta)
    {
      if (_recorddelta < _minrecorddelta)
      {
        _minrecorddelta = _recorddelta; // Update the lowest possible delta value

        if (_adjustedlatency < _recorddelta)
        {
          _adjustedlatency = _recorddelta; // Update the latecy
        }
      }

      // Get current position of the PCM samples
      result = micChannel->getPosition(&_playpos, FMOD_TIMEUNIT_PCM);
      ErrCheck(result);

      // Empty Scope
      {
        const float dampratio = 0.97f; // Constant Damping Ratio
        static float total = 0;

        total *= dampratio;

        // Adjusting total samples
        if (_recordpos >= _playpos)
          total += _recordpos - _playpos;
        else
          total += _recordpos + _soundlength - _playpos;

        _smootheddelta = total * (1.0f - dampratio);
      }

      // Checks to adjust the pitch of the samples
      // and to check for sudden changes in the samples
      // that would cause stretching of the recording
      if (_smootheddelta < (_adjustedlatency - _driftthreshold) || 
          _smootheddelta > _soundlength / 2)
      {
        micChannel->setFrequency((float)_recordrate - (_recordrate / 50));
      }
      else if (_smootheddelta > (_adjustedlatency + _driftthreshold))
      {
        micChannel->setFrequency((float)_recordrate + (_recordrate / 50));
      }
      else
      {
        micChannel->setFrequency((float)_recordrate);
      }      

      if (_check == true)
      {
        micChannel->setMute(true); // Avoid acoustic feedback
        micFilter();               // Apply microphone filter
        micMeter();                // Get the RMS peaks from the mic
        micFrequencyData();        //Get dominant frequency ranges       

        _check = false;        
      }      
      meter->getMeteringInfo(&input, 0);

      // FOR TESTING
      //frequencyConsoleOut();   
      //meterConsoleOut();
      //latencyConsoleOut();
    }   
  }

  /***************************************************************************/
  /*!
  \brief  Starts metering the input data

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::micMeter()
  {
    FMOD_RESULT result;

    result = micChannel->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &meter);
    ErrCheck(result);

    // Starts metering the input values
    result = meter->setMeteringEnabled(true, false);
    ErrCheck(result);
  }

  /***************************************************************************/
  /*!
  \brief  Gets the dominant frequencies from the input data

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::micFrequencyData()
  {
    FMOD_RESULT result;
    bool active;

    // Creates a new Fast Fourier Transform DSP
    result = pFMODAudioSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &fft);
    ErrCheck(result);        

    // Checks the DSP's active state
    result = fft->getActive(&active);
    ErrCheck(result);

    if (active)
    {      
      result = micChannel->removeDSP(fft);
      ErrCheck(result);
    }
    else
    {      
      result = micChannel->addDSP(FMOD_CHANNELCONTROL_DSP_TAIL, fft);
      ErrCheck(result);
    }
  }

  /***************************************************************************/
  /*!
  \brief  A filter to block out certain frequency ranges

  \param  cutoff
  The cutoff frequency

  \param resonance
  The bandwidth of the filter

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::micFilter(float cutoff, float resonance)
  {
    FMOD_RESULT result;
    bool active;

    // Creates a lowpass filter to get rid of all the high frequencies
    // from the microphone. Thus taking only blowing into consideration. 
    // (20 - 150 hertz)
    result = pFMODAudioSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &filter);
    ErrCheck(result);

    // Sets the passed in values
    result = filter->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, cutoff);
    ErrCheck(result);

    result = filter->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, resonance);
    ErrCheck(result);

    result = filter->getActive(&active);
    ErrCheck(result);

    if (active)
    {
      result = micChannel->removeDSP(filter);
      ErrCheck(result);
    }
    else
    {
      result = micChannel->addDSP(FMOD_CHANNELCONTROL_DSP_TAIL, filter);
      ErrCheck(result);
    }  
  }

  /***************************************************************************/
  /*!
  \brief  Value of Microphone Input

  \return Returns value of Microphone Input between 0.0 and 1.0
  */
  /***************************************************************************/
  float AudioSystem::GetMicrophoneValue()
  {
    return input.peaklevel[0];
  }

  /***************************************************************************/
  /*!
  \brief  Prints out the color coded values of the RMS peaks from the mic

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::meterConsoleOut()
  {
    if (input.peaklevel[0] < 0.2)
      std::cout << CinderConsole::cyan 
                << "RMS Peak : " 
                << CinderConsole::darkgreen 
                << input.peaklevel[0] 
                << std::endl;

    else if (input.peaklevel[0] < 0.4)
      std::cout << CinderConsole::cyan 
                << "RMS Peak : " 
                << CinderConsole::green 
                << input.peaklevel[0] 
                << std::endl;

    else if (input.peaklevel[0] < 0.6)
      std::cout << CinderConsole::cyan 
                << "RMS Peak : " 
                << CinderConsole::yellow 
                << input.peaklevel[0] 
                << std::endl;

    else if (input.peaklevel[0] < 0.9)
      std::cout << CinderConsole::cyan 
                << "RMS Peak : " 
                << CinderConsole::red 
                << input.peaklevel[0] 
                << std::endl;
    else
      std::cout << CinderConsole::cyan 
                << "RMS Peak : " 
                << CinderConsole::darkred 
                << input.peaklevel[0] 
                << std::endl;    
  } 

  /***************************************************************************/
  /*!
  \brief  Prints out color coded values of the dominant frequencies being
          picked up from the microphone.

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::frequencyConsoleOut()
  {
    int sampleSize = 64;
    char *specLeft;
    specLeft = new char[sampleSize];
    float val;
    
    // Get the frequency values
    fft->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &val, specLeft, 20000);
    
    if (val < 80)
      std::cout << CinderConsole::red 
                << "Dominant Frequency = " 
                << CinderConsole::white
                << val
                << " (SUPER LOW)"
                << std::endl; 

    else if (val < 1500)
      std::cout << CinderConsole::red
                << "Dominant Frequency : "
                << CinderConsole::gray
                << val
                << " (LOW)"
                << std::endl;

    else if (val < 3000)
      std::cout << CinderConsole::red
                << "Dominant Frequency : "
                << CinderConsole::yellow
                << val
                << " (LOW MIDS)"
                << std::endl;

    else if (val < 4500)
      std::cout << CinderConsole::red
                << "Dominant Frequency : "
                << CinderConsole::darkyellow
                << val
                << " (UPPER MIDS)"
                << std::endl;

    else if (val < 6000)
      std::cout << CinderConsole::red
                << "Dominant Frequency : "
                << CinderConsole::cyan
                << val
                << " (HIGH)"
                << std::endl;
    else if (val < 15000)
      std::cout << CinderConsole::red
                << "Dominant Frequency : "
                << CinderConsole::darkcyan
                << val
                << " (UPPER HIGH)"
                << std::endl;
    else
      std::cout << CinderConsole::red
                << "Dominant Frequency : "
                << CinderConsole::blue
                << val
                << " (UPPER HIGH)"
                << std::endl;
    
    // Delete allocated memory
    delete[] specLeft;
  }  

  /***************************************************************************/
  /*!
  \brief  Prints information regarding samples recorded and the latency
          caused from the user's audio hardware

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::latencyConsoleOut()
  {
    std::cout << CinderConsole::red 
              << "Samples: " 
              << CinderConsole::yellow
              << (int)_smootheddelta 
              << CinderConsole::green
              << " Playback Latency: "
              << CinderConsole::cyan
              << (int)_smootheddelta * 1000 / _recordrate 
              << " milliseconds" 
              << std::endl;
  }

  /***************************************************************************/
  /*!
  \brief  Checks if the microphone is present in the system

  \return Returns a bool stating the presence of the microphone
  */
  /***************************************************************************/
  bool AudioSystem::checkMicrophone()
  {
    FMOD_RESULT result;
    bool micPresent;

    result = pFMODAudioSystem->getRecordNumDrivers(&_recordnumdrivers);
    ErrCheck(result);

    // If no microphone is detected
    if (_recordnumdrivers <= 0)
    {
      micPresent = false;
      return micPresent;
    }
    else
    {
      micPresent = true;
      return micPresent;
    }
  }
}

//-----------------------------------------------------------------------------
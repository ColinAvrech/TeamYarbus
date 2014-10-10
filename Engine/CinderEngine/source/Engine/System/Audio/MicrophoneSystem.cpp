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
  void AudioSystem::InitMicData()
  {
    FMOD_RESULT result;

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

    result = pFMODAudioSystem->getRecordNumDrivers(&_recordnumdrivers);
    ErrCheck(result);

    if (_recordnumdrivers <= 0)
    {
      std::cout << Console::red << "No recording devices found/plugged in!";
    }

    result = pFMODAudioSystem->getRecordDriverInfo(0, NULL, NULL, 0, 0, 
                                        &_recordrate, 0, &_recordchannels);
    ErrCheck(result);

    _adjustedlatency = (_recordrate * LATENCY_MS) / 1000;
    _driftthreshold = _adjustedlatency / 2;
  }

  void AudioSystem::LoadMicData()
  {  
    FMOD_RESULT result;

    memset(&_exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    _exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    _exinfo.numchannels = 1;
    _exinfo.format = FMOD_SOUND_FORMAT_PCM16;
    _exinfo.defaultfrequency = _recordrate;
    _exinfo.length = _exinfo.defaultfrequency * sizeof(short) * _exinfo.numchannels * 5;    
    
    result = pFMODAudioSystem->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &_exinfo, &micSound);
    ErrCheck(result);

    result = pFMODAudioSystem->recordStart(0, micSound, true);
    ErrCheck(result);

    result = micSound->getLength(&_soundlength, FMOD_TIMEUNIT_PCM);
  }

  void AudioSystem::UpdateMicData()
  {
    FMOD_RESULT result;

    result = pFMODAudioSystem->getRecordPosition(0, &_recordpos);
    ErrCheck(result);

    _recorddelta = (_recordpos >= _lastrecordpos) ? 
                    (_recordpos - _lastrecordpos) : 
                    (_recordpos + _soundlength - _lastrecordpos);
    _lastrecordpos = _recordpos;

    _samplesrecorded += _recorddelta;

    if (_samplesrecorded >= _adjustedlatency && !micChannel)
    {
      result = pFMODAudioSystem->playSound(micSound, 0, false, &micChannel);
      ErrCheck(result);
    }

    if (micChannel && _recorddelta)
    {
      if (_recorddelta < _minrecorddelta)
      {
        _minrecorddelta = _recorddelta;
        if (_adjustedlatency < _recorddelta)
        {
          _adjustedlatency = _recorddelta;
        }
      }

      result = micChannel->getPosition(&_playpos, FMOD_TIMEUNIT_PCM);
      ErrCheck(result);

      {
        const float dampratio = 0.97f;
        static float total = 0;

        total *= dampratio;
        total += (_recordpos >= _playpos) ? 
                  (_recordpos - _playpos) : 
                  (_recordpos + _soundlength - _playpos);
        _smootheddelta = total * (1.0f - dampratio);
      }

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
        micFilter();
        micMeter();
        //micFrequencyData();        

        _check = false;        
      }      

      //frequencyConsoleOut();   
      meterConsoleOut();
      //latencyConsoleOut();
    }   
  }

  void AudioSystem::micMeter()
  {
    FMOD_RESULT result;
    bool active;

    result = micChannel->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &meter);
    ErrCheck(result);

    result = meter->setMeteringEnabled(true, false);
    ErrCheck(result);

    result = meter->getActive(&active);
    ErrCheck(result);

    if (active)
    {
      result = micChannel->removeDSP(meter);
      ErrCheck(result);
    }
    else
    {
      result = micChannel->addDSP(0, meter, 0);
      ErrCheck(result);
    }  
  }

  void AudioSystem::micFrequencyData()
  {
    FMOD_RESULT result;
    bool active;

    result = pFMODAudioSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &fft);
    ErrCheck(result);        

    result = fft->getActive(&active);
    ErrCheck(result);

    if (active)
    {
      
      result = micChannel->removeDSP(fft);
      ErrCheck(result);
    }
    else
    {      
      result = micChannel->addDSP(0, fft, 0);
      ErrCheck(result);
    }
  }

  void AudioSystem::micFilter(float cutoff, float resonance)
  {
    FMOD_RESULT result;
    bool active;

    result = pFMODAudioSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &filter);
    ErrCheck(result);

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
      result = micChannel->addDSP(0, filter, 0);
      ErrCheck(result);
    }  
  }

  void AudioSystem::meterConsoleOut()
  {
    meter->getMeteringInfo(&input, 0);

    if (input.peaklevel[0] < 0.2)
      std::cout << Console::cyan 
                << "RMS Peak : " 
                << Console::darkgreen 
                << input.peaklevel[0] 
                << std::endl;

    else if (input.peaklevel[0] < 0.4)
      std::cout << Console::cyan 
                << "RMS Peak : " 
                << Console::green 
                << input.peaklevel[0] 
                << std::endl;

    else if (input.peaklevel[0] < 0.6)
      std::cout << Console::cyan 
                << "RMS Peak : " 
                << Console::yellow 
                << input.peaklevel[0] 
                << std::endl;

    else if (input.peaklevel[0] < 0.9)
      std::cout << Console::cyan 
                << "RMS Peak : " 
                << Console::red 
                << input.peaklevel[0] 
                << std::endl;
    else
      std::cout << Console::cyan 
                << "RMS Peak : " 
                << Console::darkred 
                << input.peaklevel[0] 
                << std::endl;    
  } 

  void AudioSystem::frequencyConsoleOut()
  {
    int sampleSize = 64;
    char *specLeft;
    specLeft = new char[sampleSize];
    float val;
    
    fft->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &val, specLeft, 20000);
    
    if (val < 80)
      std::cout << Console::red 
                << "Dominant Frequency = " 
                << Console::white
                << val
                << " (SUPER LOW)"
                << std::endl; 

    else if (val < 1500)
      std::cout << Console::red
                << "Dominant Frequency : "
                << Console::gray
                << val
                << " (LOW)"
                << std::endl;

    else if (val < 3000)
      std::cout << Console::red
                << "Dominant Frequency : "
                << Console::yellow
                << val
                << " (LOW MIDS)"
                << std::endl;

    else if (val < 4500)
      std::cout << Console::red
                << "Dominant Frequency : "
                << Console::darkyellow
                << val
                << " (UPPER MIDS)"
                << std::endl;

    else if (val < 6000)
      std::cout << Console::red
                << "Dominant Frequency : "
                << Console::cyan
                << val
                << " (HIGH)"
                << std::endl;
    else if (val < 15000)
      std::cout << Console::red
                << "Dominant Frequency : "
                << Console::darkcyan
                << val
                << " (UPPER HIGH)"
                << std::endl;
    else
      std::cout << Console::red
                << "Dominant Frequency : "
                << Console::blue
                << val
                << " (UPPER HIGH)"
                << std::endl;
    
    delete[] specLeft;
  }  

  void AudioSystem::latencyConsoleOut()
  {
    std::cout << Console::red 
              << "Samples: " 
              << Console::yellow
              << (int)_smootheddelta 
              << Console::green
              << " Playback Latency: "
              << Console::cyan
              << (int)_smootheddelta * 1000 / _recordrate 
              << " milliseconds" 
              << std::endl;
  }
}

//-----------------------------------------------------------------------------
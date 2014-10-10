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

  FMOD::Sound *test = 0;
  FMOD::Channel *pChannel = 0;
  bool fly = true;

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
    //_newSound = new Sound();    
    FMOD_RESULT result;

    memset(&_exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    _exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    _exinfo.numchannels = 1;
    _exinfo.format = FMOD_SOUND_FORMAT_PCM16;
    _exinfo.defaultfrequency = _recordrate;
    _exinfo.length = _exinfo.defaultfrequency * sizeof(short) * _exinfo.numchannels * 5;

    //test = _newSound->LoadMic(pFMODAudioSystem, soundName, type, _exinfo);
    
    result = pFMODAudioSystem->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &_exinfo, &test);
    ErrCheck(result);

    result = pFMODAudioSystem->recordStart(0, test, true);
    ErrCheck(result);

    result = test->getLength(&_soundlength, FMOD_TIMEUNIT_PCM);

    

    //return _newSound;
  }

  FMOD::Sound* Sound::LoadMic(FMOD::System* pSystem, char* soundName, SoundID id, FMOD_CREATESOUNDEXINFO exinfo)
  {
    FMOD_RESULT result;

    pFMODAudioSystem = pSystem;
    _soundName = soundName;
    ID = id;

    result = pSystem->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &pSound);
    ErrCheck(result);

    result = pSystem->recordStart(0, pSound, true);
    ErrCheck(result);

    sound_queue_.push_back(pSound);

    unsigned index = sound_queue_.size() - 1;

    SetType(ID, index);

    return pSound;
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

    if (_samplesrecorded >= _adjustedlatency && !pChannel)
    {
      //pChannel = _newSound->Get_Channel();

      result = pFMODAudioSystem->playSound(test, 0, false, &pChannel);
      ErrCheck(result);
    }

    if (pChannel && _recorddelta)
    {
      if (_recorddelta < _minrecorddelta)
      {
        _minrecorddelta = _recorddelta;
        if (_adjustedlatency < _recorddelta)
        {
          _adjustedlatency = _recorddelta;
        }
      }

      result = pChannel->getPosition(&_playpos, FMOD_TIMEUNIT_PCM);
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
        pChannel->setFrequency((float)_recordrate - (_recordrate / 50));
      }
      else if (_smootheddelta > (_adjustedlatency + _driftthreshold))
      {
        pChannel->setFrequency((float)_recordrate + (_recordrate / 50));
      }
      else
      {
        pChannel->setFrequency((float)_recordrate);
      }

      FMOD::DSP *reverb;
      FMOD::DSP *lpf;
      
      bool active;

      if (fly == true)
      {
        pChannel->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &meter);

        meter->setMeteringEnabled(true, false);


        //result = pFMODAudioSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &fft);
        //ErrCheck(result);

        result = pFMODAudioSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &lpf);
        ErrCheck(result);

        lpf->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 200);
        lpf->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, 1);

        //FMOD_REVERB_PROPERTIES propon = FMOD_PRESET_BATHROOM;
        //result = pFMODAudioSystem->setReverbProperties(0, &propon);

        result = meter->getActive(&active);
        //result = reverb->getActive(&active);
        result = lpf->getActive(&active);
        //result = fft->getActive(&active);
        ErrCheck(result);

        if (active)
        {
          result = pChannel->removeDSP(meter);
          //result = pChannel->removeDSP(reverb);
          result = pChannel->removeDSP(lpf);
          //result = pChannel->removeDSP(fft);
          ErrCheck(result);
        }
        else
        {
          result = pChannel->addDSP(0, meter, 0);
          //result = pChannel->addDSP(0, reverb, 0);
          result = pChannel->addDSP(0, lpf, 0);
          //result = pChannel->addDSP(0, fft, 0);
          ErrCheck(result);
        }

        fly = false;        
      } 
      
      

      meter->getMeteringInfo(&input, 0);
      //if (input.peaklevel[0] < 0.2)
      //  std::cout << Console::cyan << "RMS Peak : " << Console::darkgreen << input.peaklevel[0] << std::endl;
      //else if (input.peaklevel[0] < 0.4)
      //  std::cout << Console::cyan << "RMS Peak : " << Console::green << input.peaklevel[0] << std::endl;
      //else if (input.peaklevel[0] < 0.6)
      //  std::cout << Console::cyan << "RMS Peak : " << Console::yellow << input.peaklevel[0] << std::endl;
      //else if (input.peaklevel[0] < 0.9)
      //  std::cout << Console::cyan << "RMS Peak : " << Console::red << input.peaklevel[0] << std::endl;
      //else
      //  std::cout << Console::cyan << "RMS Peak : " << Console::darkred << input.peaklevel[0] << std::endl;
     //int sampleSize = 64;
     //char *specLeft;
     //specLeft = new char[sampleSize];
     //float val;
     //
     //fft->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &val, specLeft, 1000);
     //       
     //std::cout << Console::red << "playback latency : " << specLeft << std::endl; 
     //
     //delete[] specLeft;
      
    }
    //std::cout << Console::red << "playback latency : " << (int)_smootheddelta << " samples" << "," << (int)_smootheddelta * 1000 / _recordrate << " MS" << std::endl;
    

    
  }  
}

//-----------------------------------------------------------------------------
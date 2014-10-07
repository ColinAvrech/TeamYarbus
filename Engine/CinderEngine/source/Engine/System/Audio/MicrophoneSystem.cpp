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
    _playpos = 0;
    _smootheddelta = 0;

    result = pFMODAudioSystem->getRecordDriverInfo(0, NULL, NULL, 0, 0, 
                                        &_recordrate, 0, &_recordchannels);
    ErrCheck(result);

    _adjustedlatency = (_recordrate * LATENCY_MS) / 1000;
    _driftthreshold = _adjustedlatency / 2;
  }

  Sound* AudioSystem::LoadMicData(char* soundName, Sound::SoundID type, float volume)
  {
    _newSound = new Sound();    
    FMOD_RESULT result;

    memset(&_exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    _exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    _exinfo.numchannels = _recordchannels;
    _exinfo.format = FMOD_SOUND_FORMAT_PCM16;
    _exinfo.defaultfrequency = _recordrate;
    _exinfo.length = _exinfo.defaultfrequency * sizeof(short) * _exinfo.numchannels * 5;

    _newSound->pSound = _newSound->LoadMic(pFMODAudioSystem, soundName, type, _exinfo);
    
    result = _newSound->pSound->getLength(&_soundlength, FMOD_TIMEUNIT_PCM);

    return _newSound;
  }

  void AudioSystem::UpdateMicData()
  {
    FMOD_RESULT result;
    FMOD::Channel *pChannel;

    result = pFMODAudioSystem->getRecordPosition(0, &_recordpos);
    ErrCheck(result);

    _recorddelta = (_recordpos >= _lastrecordpos) ? 
                    (_recordpos - _lastrecordpos) : 
                    (_recordpos + _soundlength - _lastrecordpos);
    _lastrecordpos = _recordpos;

    _samplesrecorded += _recorddelta;

    if (_samplesrecorded >= _adjustedlatency && !(_newSound->Get_Channel()))
    {
      pChannel = _newSound->Get_Channel();

      result = pFMODAudioSystem->playSound(_newSound->pSound, 0, false, &pChannel);
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
        pChannel->setFrequency(_recordrate - (_recordrate / 50));
      }
      else if (_smootheddelta > (_adjustedlatency + _driftthreshold))
      {
        pChannel->setFrequency(_recordrate + (_recordrate / 50));
      }
      else
      {
        pChannel->setFrequency(_recordrate);
      }
    }
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



  
}

//-----------------------------------------------------------------------------
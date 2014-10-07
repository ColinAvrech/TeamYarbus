/*****************************************************************************/
/*!
\file   DSP_System.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains various functions meant for Digital Signal Processing
*/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#include "Sound.h"

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

namespace Framework
{
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
}

//-----------------------------------------------------------------------------
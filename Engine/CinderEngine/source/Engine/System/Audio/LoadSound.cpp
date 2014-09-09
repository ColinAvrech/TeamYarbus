/*****************************************************************************/
/*!
\file   LoadSound.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Loads all the audio assets
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
  \brief  Loads all the audio assets

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::LoadAllSounds()
  {
    LoadSound("music.mp3", "Test", Sound::SOUND_2D, 0.7f)->Play();

    std::cout << "ALL SOUNDS LOADED" << std::endl;
  }
}
//-----------------------------------------------------------------------------
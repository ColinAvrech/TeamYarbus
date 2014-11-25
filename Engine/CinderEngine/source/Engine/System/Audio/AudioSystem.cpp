/*****************************************************************************/
/*!
\file   AudioSystem.cpp
\author Deepak Chennakkadan
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Creates the infrastructure for the Audio System
*/      
/*****************************************************************************/

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes

#include "AudioSystem.h"

#pragma endregion

namespace Framework
{
  /*---------------------------------------------------------------------------
  // Class Implementation
  ---------------------------------------------------------------------------*/
  
  /*---------------------------------------------------------------------------
  // Constructors
  ---------------------------------------------------------------------------*/
  #pragma region Constructors

  AudioSystem::AudioSystem() : pFMODAudioSystem(NULL), 
                               GroupMusic(NULL), 
                               Group2DSFX(NULL), 
                               Group3DSFX(NULL)
  {
    ErrorIf(AUDIOSYSTEM != NULL, "AudioSystem Audio System already created");
    AUDIOSYSTEM = this;
    //ErrorIf(CreateFMODSystem() == false, "AudioSystem not created");
    CreateFMODSystem();
  }

  #pragma endregion

  /*---------------------------------------------------------------------------
  // Public Variables
  ---------------------------------------------------------------------------*/
  #pragma region Public Variables

  AudioSystem *AUDIOSYSTEM;

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
  \brief  Checks for FMOD related errors

  \param  FMOD_RESULT
  Type of error

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::ErrCheck(FMOD_RESULT result)
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
  \brief  Initialize Function

  \return Returns nothing
  */
  /***************************************************************************/
  bool AudioSystem::Initialize()
  {
    // Calls function the creates FMOD groups
    CreateSoundGroups();
    InitMicData();

    std::cout << "Audio System Initialized" << std::endl;
	  return true;
  }

  /***************************************************************************/
  /*!
  \brief  This fucntion loads a sound into the Engine

  \param  filename
  The name of the audio file

  \param  soundName
  The name assigned to the file we are loading

  \param type
  Assigning the type of sound we are loading

  \param volume
  Assign volume at which the sound should play

  \return Returns a Sound class Object
  */
  /***************************************************************************/
  Sound* AudioSystem::LoadSound(const char* filename, 
                                char* soundName, 
                                Sound::SoundID type, 
                                float volume)
  {    
    Sound* newSound = new Sound(); // Dynamically creates a new 'Sound' object
    char Path[250];

    sprintf(Path, "%s%s", AudioAssetsPath, filename);

    // Calls the load method from the Sound Class
    newSound->Load(pFMODAudioSystem, Path, soundName, type);

    // Statements to check which group to assign the loaded sound to
    if (type == Sound::MUSIC)
    {
      newSound->SetChannelGroup(GroupMusic);
    }
    else if (type == Sound::SOUND_2D)
    {
      newSound->SetChannelGroup(Group2DSFX);
    }
    else
    {
      newSound->SetChannelGroup(Group3DSFX);
    }

    // Assigns volume to the sound object
    newSound->SetVolume(volume); 

    // Adds the sound object to the soundMap container
    soundMap[soundName] = (newSound); 

    // Returns the newly created sound object
    return newSound;
  }

  /***************************************************************************/
  /*!
  \brief  Stops the specific sounds we want to stop playing

  \param  id
  Specific sound type that we intend to stop playing

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::StopSounds(int id)
  {
    // Checks if audio system is not on
    if (Sound::system_on_ == false)
    {
      return;
    }

    // Stops all Music sounds
    if (id == Sound::MUSIC || id == -1)
    {
      GroupMusic->stop();
    }

    // Stops all 2D sound effects
    if (id == Sound::SOUND_2D
      || id == Sound::SFX_ALL || id == -1)
    {
      Group2DSFX->stop();
    }

    // Stops all 3D sound effects
    if (id == Sound::SOUND_3D
      || id == Sound::SFX_ALL || id == -1)
    {
      Group3DSFX->stop();
    }
  }

  /***************************************************************************/
  /*!
  \brief  Update Function

  \param  dt
  Takes in the Engine's frametime

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::Update(const double &dt)
  {
    // Checks if audio system is not on
    if (Sound::system_on_ == false)
    {
      return;
    }

    // Updates the audio system
    pFMODAudioSystem->update();
    UpdateMicData();

    // Updates all the Sound Object Instances
    std::map<string, Sound*>::iterator it;
    for (it = soundMap.begin(); it != soundMap.end(); ++it)
    {
      it->second->Update(dt);
    }
  }

  /***************************************************************************/
  /*!
  \brief  Get method to get the sound object

  \param  soundName
  Name of the sound

  \return Returns the element stored in the soundMap
  */
  /***************************************************************************/
  Sound* AudioSystem::GetSound(char* soundName)
  {
    return soundMap[soundName];
  }  

  /***************************************************************************/
  /*!
  \brief  Checks if sound groups are muted

  \param  id
  Specific sound type that we intend to check

  \return Returns a bool stating which group is muted
  */
  /***************************************************************************/
  bool AudioSystem::GetMuted(int id)
  {
    // Checks if audio system is not on
    if (Sound::system_on_ == false)
    {
      return true;
    }

    if (soundMap.size() == 0)
    {
      return 0.0f;
    }

    bool sfx2DMuted = 0;
    bool sfx3DMuted = 0;
    bool musicMuted = 0;

    // FMOD getters for checking for Mute
    GroupMusic->getMute(&sfx2DMuted);
    Group2DSFX->getMute(&sfx3DMuted);
    Group3DSFX->getMute(&musicMuted);

    // Statements to check which groups have been muted
    if (id == -1)
    {
      return musicMuted && sfx2DMuted && sfx3DMuted;
    }
    if (id == Sound::MUSIC)
    {
      return musicMuted;
    }
    if (id == Sound::SOUND_3D)
    {
      return sfx3DMuted;
    }
    if (id == Sound::SOUND_2D)
    {
      return sfx2DMuted;
    }
    else
    {
      return sfx2DMuted && sfx3DMuted;
    }
  }

  /***************************************************************************/
  /*!
  \brief  Pauses the sounds at its current state of playing

  \param  paused
  Bool to state if paused or not

  \param id
  Specific sound type that we intend to pause

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::SetPaused(bool paused, int id)
  {
    // Checks if audio system is not on
    if (Sound::system_on_ == false)
    { 
      return;
    }

    // Pauses all Music sounds
    if (id == Sound::MUSIC
      || id == Sound::SFX_ALL || id == -1)
    {
      GroupMusic->setPaused(paused);
    }

    // Pauses all 2D sound effects
    if (id == Sound::SOUND_2D
      || id == Sound::SFX_ALL || id == -1)
    {
      Group2DSFX->setPaused(paused);
    }

    // Pauses all 3D sound effects
    if (id == Sound::SOUND_3D
      || id == Sound::SFX_ALL || id == -1)
    {
      Group3DSFX->setPaused(paused);
    }
  }

  #pragma endregion


  /*---------------------------------------------------------------------------
  // Static Public Variables
  ---------------------------------------------------------------------------*/
  #pragma region Static Public Variables

  #pragma endregion


  /*---------------------------------------------------------------------------
  // Static Public Functions
  ---------------------------------------------------------------------------*/
  #pragma region Static Public Functions

  #pragma endregion


  /*---------------------------------------------------------------------------
  // Destructor
  ---------------------------------------------------------------------------*/
  #pragma region Destructor

  AudioSystem :: ~AudioSystem ()
  {
    FMOD_RESULT result;

    // Deletes all the sounds objects
    std::map<string, Sound*>::iterator it;
    for (it = soundMap.begin(); it != soundMap.end(); ++it)
    {
      delete it->second;
    }

    // Prints out when audio system gets destroyed
	  std::cout << GetName() + " destroyed" << std::endl;

    // Closes down the FMOD system
    result = pFMODAudioSystem->close();
    ErrCheck(result);


    // Releases the FMOD system
    result = pFMODAudioSystem->release();
    ErrCheck(result);

    AUDIOSYSTEM = NULL;
    pFMODAudioSystem = NULL;
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

  /***************************************************************************/
  /*!
  \brief  Creates Sound Groups to organize sounds of different types

  \return Returns nothing
  */
  /***************************************************************************/
  void AudioSystem::CreateSoundGroups()
  {
    FMOD_RESULT result;

    // Creates group that will be holding all elements of Music
    result = pFMODAudioSystem->createChannelGroup("Music", &GroupMusic);
    ErrCheck(result);

    // Creates group that will be holding elements of non-spatial sounds
    result = pFMODAudioSystem->createChannelGroup("2DSFX", &Group2DSFX);
    ErrCheck(result);

    // Creates group that will be holding lements of spatial sounds
    result = pFMODAudioSystem->createChannelGroup("3DSFX", &Group3DSFX);
    ErrCheck(result);
  }

  /***************************************************************************/
  /*!
  \brief  Creates the FMOD system

  \return Returns a bool stating if the Audio System was created or not
  */
  /***************************************************************************/
  bool AudioSystem::CreateFMODSystem()
  {
    FMOD_RESULT result;

    // Creates the FMOD audio system
    result = FMOD::System_Create(&pFMODAudioSystem);
    if (result != FMOD_OK)
    {
      printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
      return false;
    }

    // FMOD's init functions. Sets channels and default modes
    result = pFMODAudioSystem->init(512, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
      printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
      return false;
    }

    // If system was sucessfully created
    return true;
  }

  #pragma endregion


  /*---------------------------------------------------------------------------
  // Static Functions
  ---------------------------------------------------------------------------*/
  #pragma region Static Functions

  #pragma endregion

}
//-----------------------------------------------------------------------------

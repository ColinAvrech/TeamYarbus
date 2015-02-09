/******************************************************************************/
/*!
\file   PlayerStats.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include <Precompiled.h>

namespace Framework
{
	DefineComponentName(PlayerStats);

  // Constructor
  PlayerStats::PlayerStats ()
  {
  }
  
  // Destructor
  PlayerStats::~PlayerStats ()
  {}

  void PlayerStats::Initialize ()
  {
    EVENTSYSTEM->mConnect<AllTreesBurnedEvent, PlayerStats> (Events::ALLLTREESBURNED, this, &PlayerStats::AllTreesBurned);
    EVENTSYSTEM->mConnect<UpdateEvent, PlayerStats> (Events::UPDATEEVENT, this, &PlayerStats::Update);
    OPENGL->Change_Shader("FadeIn", (int)SS_FADE_IN);
  }

  void PlayerStats::Serialize (Serializer::DataNode* data)
  {
    data->FindElement (data, "NextLevel")->GetValue (&NextLevel);
  }

  void PlayerStats::OnApplicationPause (PauseEvent* pause)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void PlayerStats::AllTreesBurned (AllTreesBurnedEvent* atb)
  {
    OPENGL->Change_Shader("FadeIn", (int)SS_FADE_OUT);
    levelComplete = true;
  }

  void PlayerStats::Update (UpdateEvent* update)
  {
    if (levelComplete)
    {
      timer += 0.016f;
      if (timer > 2.24f)
      {
        OBJECTSYSTEM->NextLevel();
        timer = 0.0f;
      }
    }
  }

}
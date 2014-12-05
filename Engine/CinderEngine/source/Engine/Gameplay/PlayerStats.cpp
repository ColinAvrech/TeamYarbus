/******************************************************************************/
/*!
\file   PlayerStats.cpp
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "PlayerStats.h"
#include "EventSystem.h"
#include "ObjectSystem.h"
#include "GameEvent.h"

namespace Framework
{


  // Constructor
  PlayerStats::PlayerStats ()
  {}
  
  // Destructor
  PlayerStats::~PlayerStats ()
  {}

  void PlayerStats::Initialize ()
  {
    EVENTSYSTEM->mConnect<AllTreesBurnedEvent, PlayerStats> (Events::ALLLTREESBURNED, this, &PlayerStats::AllTreesBurned);
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
    OBJECTSYSTEM->LoadLevel (NextLevel.c_str ());
  }

}
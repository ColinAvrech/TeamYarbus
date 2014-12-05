/******************************************************************************/
/*!
\file   LevelTimer.cpp
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "LevelTimer.h"
#include "EventSystem.h"
#include "UpdateEvent.h"
#include "ObjectSystem.h"
#include "Zilch.hpp"

namespace Framework
{
  DefineComponentName (LevelTimer);

  // Constructor
  LevelTimer::LevelTimer ()
  {}
  
  // Destructor
  LevelTimer::~LevelTimer ()
  {
    EVENTSYSTEM->mDisconnect<UpdateEvent, LevelTimer> (Events::UPDATEEVENT, this, &LevelTimer::Update);
  }

  void LevelTimer::Initialize ()
  {
    EVENTSYSTEM->mConnect<UpdateEvent, LevelTimer> (Events::UPDATEEVENT, this, &LevelTimer::Update);
  }

  void LevelTimer::Serialize (Serializer::DataNode* data)
  {
    Serializer::DataNode* value = data->FindElement (data, "NextLevel");
    value->GetValue (&nextLevel);

    value = data->FindElement (data, "EndTime");
    value->GetValue (&endTime);
  }

  void LevelTimer::Update (UpdateEvent* update)
  {
    time += 0.016f;

    if (time >= endTime)
    {
      OBJECTSYSTEM->LoadLevel (nextLevel.c_str());
    }
  }

}
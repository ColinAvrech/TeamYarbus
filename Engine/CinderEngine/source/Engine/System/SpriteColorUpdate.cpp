/******************************************************************************/
/*!
\file   SpriteColorUpdate.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "SpriteColorUpdate.h"
#include "EventSystem.h"
#include "UpdateEvent.h"
#include "GameObject.h"
#include "Sprite.h"

namespace Framework
{
  DefineComponentName (SpriteColorUpdate);

  // Constructor
  SpriteColorUpdate::SpriteColorUpdate ()
  {}
  
  // Destructor
  SpriteColorUpdate::~SpriteColorUpdate ()
  {}

  void SpriteColorUpdate::Serialize (Serializer::DataNode* data)
  {
    data->FindElement (data, "MinColor")->GetValue (&minColor);
    data->FindElement (data, "MaxColor")->GetValue (&maxColor);
    data->FindElement (data, "Multiplier")->GetValue (&multiplier);
  }

  void SpriteColorUpdate::Initialize ()
  {
    EVENTSYSTEM->mConnect<UpdateEvent, SpriteColorUpdate> (Events::UPDATEEVENT, this, &SpriteColorUpdate::Update);
    gameObject->Sprite->color = minColor;
  }

  void SpriteColorUpdate::Update (UpdateEvent* update)
  {
    timer += 0.016f;
    if (gameObject->Sprite->color.a < 1.0f)
    {
      gameObject->Sprite->color = glm::mix (minColor, maxColor, timer * multiplier);
    }
  }

  void SpriteColorUpdate::OnApplicationPause (PauseEvent* pause)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

}
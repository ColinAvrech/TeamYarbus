/******************************************************************************/
/*!
\file   Health.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The players health
*/
/******************************************************************************/
#include "Health.h"
#include "EventSystem.h"
#include "UpdateEvent.h"
#include "CollisionEvent.h"
#include "Core.h"
#include "ObjectSystem.h"
#include "GUIText.h"
#include "Thermodynamics.h"
#include "PlayerEffect.h"
#include "Pipeline.h"

namespace Framework
{
	Health::~Health()
	{
		EVENTSYSTEM->mDisconnect<UpdateEvent, Health>(Events::UPDATEEVENT, this, &Health::Update);
		//EVENTSYSTEM->mDisconnect<CollisionEvent, Health>("CollisionEvent", this, &Health::OnCollisionEnter);
	}

	void Health::Serialize(Serializer::DataNode* data)
	{
		Serializer::DataNode* value = data->FindElement(data, "alive");
		value->GetValue(&alive);

		value = data->FindElement(data, "maxRadius");
		value->GetValue(&maxRadius);

		value = data->FindElement(data, "minRadius");
		value->GetValue(&minRadius);

		value = data->FindElement(data, "reFueling");
		value->GetValue(&reFueling);
	}

	void Health::Initialize()
	{
		EVENTSYSTEM->mConnect<UpdateEvent, Health>(Events::UPDATEEVENT, this, &Health::Update);
		gameObject->Health = this;

//    currentDeathRate = startDeathRate;
		currentRadius = maxRadius;
    invincible = false;
    originalPosition = gameObject->Transform->GetPosition ();
    playerEffect = reinterpret_cast<PlayerEffect*>(gameObject->GetComponent ("PlayerEffect"));
	}

#include "PlayerStats.h"
	void Health::Update(UpdateEvent* e)
	{
    if (levelFailed)
    {
      timer += 0.016f;
      if (timer > 1.0f)
      {
        OBJECTSYSTEM->LoadLevel (OBJECTSYSTEM->currentLevelName.c_str ());
      }
      return;
    }

		//check if player is colliding with node on fire -- for refuel
		glm::ivec2 currPos = gameObject->Transform->GetGridPosition();
		int material = Physics::THERMODYNAMICS->GetCellMaterial(currPos.x, currPos.y);
		float temp = Physics::THERMODYNAMICS->GetCellTemperature(currPos.x, currPos.y);

    playerEffect->size = currentRadius * 100.0f;

		if (temp >= Physics::Constant::BT_Organics && material == GRASS)
			currentRadius = maxRadius;
    
    if (invincible || CORE->IsPaused())
    {
      return;
    }
	
    //if (deathRate != .1f)
	  //	deathRate = .1f;

    currentRadius -= currentDeathRate * .016;
		gameObject->Transform->Scale(currentRadius / maxRadius);

    if (currentRadius <= minRadius)
    {
      levelFailed = true;
      //OPENGL->Change_Shader ("FadeIn", (int) SS_FADE_OUT);
      //printf("dead");
      GUIText* guiText = reinterpret_cast<GUIText*>(gameObject->GetComponent("GUIText"));
      //PlayerStats* stats = reinterpret_cast<PlayerStats*>(gameObject->GetComponent("PlayerStats"));
      if (guiText)// && stats)
      {
        guiText->text = "You ran out of fuel :(. Restarting Level: ";// +stats->NextLevel.c_str();
        //TODO_AUDIO: Play Death Sound/Music
      }
    }
	}

	void Health::reFuel()
	{
		reFueling = true;
	}

  void Health::ToggleInvulnerability()
  {
    invincible = !invincible;
  }

	DefineComponentName(Health);
}
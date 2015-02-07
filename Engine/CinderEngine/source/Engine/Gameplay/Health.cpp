/******************************************************************************/
/*!
\file   Health.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The players health
*/
/******************************************************************************/

#include <Precompiled.h>

namespace Framework
{
	Health::~Health()
	{
		EVENTSYSTEM->mDisconnect<UpdateEvent, Health>(Events::UPDATEEVENT, this, &Health::Update);
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

		//currentDeathRate = startDeathRate;
		currentRadius = maxRadius;
		invincible = false;
    originalPosition = static_cast<Transform*>(gameObject->GetComponent("Transform"))->GetPosition();
		playerEffect = static_cast<PlayerEffect*>(gameObject->GetComponent("PlayerEffect"));
	}

#include "PlayerStats.h"
	void Health::Update(UpdateEvent* e)
	{
		//if (e->TimePassed > 3.5f)
		//{
		//  invincible = false;
		//}

		if (levelFailed)
		{
			timer += e->Dt;
			if (timer > 2.24f)
			{
				OBJECTSYSTEM->RestartLevel();
				timer = 0.0f;
			}
			return;
		}


    Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
		//check if player is colliding with node on fire -- for refuel
		glm::ivec2 currPos = tform->GetGridPosition();
		int material = Physics::THERMODYNAMICS->GetCellMaterial(currPos.x, currPos.y);
		float temp = Physics::THERMODYNAMICS->GetCellTemperature(currPos.x, currPos.y);

		playerEffect->size = currentRadius * 100.0f;

		if (temp >= Physics::Constant::BT_Organics && material == GRASS)
			currentRadius = maxRadius;

		if (invincible || CORE->IsPaused())
		{
			return;
		}

		currentRadius -= currentDeathRate * e->Dt;
		tform->Scale(currentRadius / maxRadius);

		if (currentRadius <= minRadius)
		{
			//printf("dead");
			//PlayerStats* stats = reinterpret_cast<PlayerStats*>(gameObject->GetComponent("PlayerStats"));
			GUIText* guiText = reinterpret_cast<GUIText*>(gameObject->GetComponent("GUIText"));
			if (guiText)// && stats)
			{
				guiText->text = "You ran out of fuel :(. Restarting Level: ";// +stats->NextLevel.c_str();
				//TODO_AUDIO: Play Hud update sound
			}
			//TODO_AUDIO: Play Death Sound/Music
			Sound* loseFX = Resources::RS->Get_Sound("fx_lose.ogg");
			AUDIOEVENTS->unmanagedSounds.push_back(loseFX);
			loseFX->Play();
			OPENGL->Change_Shader("FadeIn", (int)SS_FADE_OUT);
			levelFailed = true;
			alive = false;
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
/******************************************************************************/
/*!
\file   Splitscreen.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#include <Precompiled.h>
#include "Splitscreen.h"
#include "UpdateEvent.h"
#include "DataNode.h"
#include "WindowSystem.h"
#include "KeyEvent.h"
#include "EventSystem.h"
#include "Component.h"

#define SWITCH_TIME 5.0 //the amount of alotted time in the light World

namespace Framework
{
	WindowSystem* WINDOWSYSTEM;
	Splitscreen::~Splitscreen()
	{
		EVENTSYSTEM->mDisconnect<KeyEvent, Splitscreen>(Events::KEY_ANY, this, &Splitscreen::KeyPressed);
	}
	void Splitscreen::Initialize()
	{
		t_maxtime = 0.0;
		lworld_maxtime = 0.0;
		currentState = DARK_WORLD;
		lastState = DARK_WORLD;
		EVENTSYSTEM->mConnect<KeyEvent, Splitscreen>(Events::KEY_ANY, this, &Splitscreen::KeyPressed);
	}
	void Splitscreen::Serialize(Serializer::DataNode* data)
	{
		//SPECIFY SHADERS HERE FOR LIGHT WORLD___
		data->FindElement(data, "lightRadius")->GetValue(&lightRadius);
		data->FindElement(data, "lightTheta")->GetValue(&lightTheta);

	}
	void Splitscreen::CalculateFlashlight()
	{
		if (flashlight == true)
		{
			//flaslight follows the mouse
			origin.x = gameObject->Transform->GetPosition().x - WINDOWSYSTEM->Get_Mouse_Position().x;
			origin.y = gameObject->Transform->GetPosition().y - WINDOWSYSTEM->Get_Mouse_Position().y;
			leftBounds.x = lightRadius*cos(lightTheta / 2);
			leftBounds.y = lightRadius*sin(lightTheta / 2);
			rightBounds.x = lightRadius*cos(-lightTheta / 2);
			rightBounds.y = lightRadius*sin(-lightTheta / 2);
		}
	}
	void Splitscreen::Update(UpdateEvent* e)
	{
		//transitional period
		if (currentState == TRANS_TOLIGHT ||
			currentState == TRANS_TODARK &&
			t_timer <= t_maxtime)
		{
			t_timer += e->Dt;
			if (currentState == TRANS_TOLIGHT)
			{
				currentState = LIGHT_WORLD;
				lastState = TRANS_TOLIGHT;
			}
			else if (currentState == TRANS_TODARK)
			{
				currentState = DARK_WORLD;
				lastState = TRANS_TODARK;
			}
			lworld_timer += e->Dt;
		}
		if (lworld_timer <= lworld_maxtime)
		{
			EnableInteractions();
		}
		else
			currentState = TRANS_TODARK;
		if (currentState == DARK_WORLD)
		{
			CalculateFlashlight();
		}
	}
	void Splitscreen::KeyPressed(KeyEvent* e)
	{
		if (e->KeyDown)
		{
			if (currentState == DARK_WORLD)
			{
				currentState = TRANS_TODARK;
				lastState = DARK_WORLD;
			}
		}
	}
	void Splitscreen::EnableInteractions(/*parameters of filters*/)
	{
		//turn on colliders on objects in other world
		//draw said objects

	}
	DefineComponentName(Splitscreen);
}

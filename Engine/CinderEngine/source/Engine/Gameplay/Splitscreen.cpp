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
	Splitscreen::~Splitscreen()
	{
		EVENTSYSTEM->mDisconnect<KeyEvent, Splitscreen>(Events::KEY_ANY, this, &Splitscreen::KeyPressed);
	}
	void Splitscreen::Initialize()
	{

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
      Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));
			origin.x = tform->GetPosition().x - WINDOWSYSTEM->Get_Mouse_Position().x;
			origin.y = tform->GetPosition().y - WINDOWSYSTEM->Get_Mouse_Position().y;
			leftBounds.x = lightRadius*cos(lightTheta / 2);
			leftBounds.y = lightRadius*sin(lightTheta / 2);
			rightBounds.x = lightRadius*cos(-lightTheta / 2);
			rightBounds.y = lightRadius*sin(-lightTheta / 2);
		}
	}
	void Splitscreen::Update(UpdateEvent* e)
	{

	}
	void Splitscreen::KeyPressed(KeyEvent* e)
	{
		if (e->KeyDown)
		{
			//WORLDSTA
		}
	}
	void Splitscreen::EnableInteractions(/*parameters of filters*/)
	{
		//turn on colliders on objects in other world
		//draw said objects

	}
	DefineComponentName(Splitscreen);
}

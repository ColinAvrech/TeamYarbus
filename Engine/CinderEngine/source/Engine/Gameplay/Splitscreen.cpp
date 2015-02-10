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

	}
	void Splitscreen::Update(UpdateEvent* e)
	{

	}
	void Splitscreen::KeyPressed(KeyEvent* e)
	{
		if (e->KeyDown)
			WORLDSTATE->ToggleState();
	}
	void Splitscreen::EnableInteractions(/*parameters of filters*/)
	{
		//turn on colliders on objects in other world
		//draw said objects

	}
	DefineComponentName(Splitscreen);
}

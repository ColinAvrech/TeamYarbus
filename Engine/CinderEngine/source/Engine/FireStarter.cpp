/******************************************************************************/
/*!
\file   GameLogic.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The logic for burning gameObjects
*/
/******************************************************************************/
#include "FireStarter.h"
#include "GameObject.h"
#include "ComponentInclude.h"

namespace Framework
{ 
	void FireStarter::Serialize(Serializer::DataNode* data)
	{
		
	}

	void FireStarter::Initialize()
	{
		gameObject->FireStarter = this;
	}

	void FireStarter::lightOnFire(void)
	{
		onFire = true;
	}

	DefineComponentName(FireStarter);
}
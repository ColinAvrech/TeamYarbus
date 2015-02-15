/******************************************************************************/
/*!
\file   FpsCounter.cpp
\author Damien Low
\par    Course: GAM250
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief	Component for displaying FPS
*/
/******************************************************************************/
#include <Precompiled.h>
#include "FpsCounter.h"

namespace Framework
{
	DefineComponentName(FpsCounter);

	FpsCounter::FpsCounter()
	{

	}

	FpsCounter::~FpsCounter()
	{
		EVENTSYSTEM->mDisconnect<UpdateEvent, FpsCounter>(Events::UPDATEEVENT, this, &FpsCounter::Update);
	}


	void FpsCounter::Initialize()
	{
		EVENTSYSTEM->mConnect<UpdateEvent, FpsCounter>(Events::UPDATEEVENT, this, &FpsCounter::Update);
//		guiText_->position = { -0.2f, -1.9f };
//		guiText_->Initialize();
	}

	void FpsCounter::Serialize(Serializer::DataNode* data)
	{

	}

	void FpsCounter::Update(UpdateEvent* e)
	{
		Transform* tform = static_cast<Transform*>(gameObject->GetComponent("Transform"));

		guiText_ = reinterpret_cast<GUIText*>(gameObject->GetComponent("GUIText"));
		if (guiText_)
		{
//			guiText_->text = "FPS: ";
//			guiText_->Draw();
		}
	}
}
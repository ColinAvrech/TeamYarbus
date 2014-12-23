/******************************************************************************/
/*!
\file   Microphone.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Microphone active
*/
/******************************************************************************/

#include <Precompiled.h>

namespace Framework
{
	void Microphone::Serialize(Serializer::DataNode* data)
	{
		bool Active;
		Serializer::DataNode* temp = data->FindElement(data, "Active");
		temp->GetValue(&Active);
	}

	void Microphone::Initialize()
	{
		gameObject->Microphone = this;
	}

	DefineComponentName(Microphone);
}
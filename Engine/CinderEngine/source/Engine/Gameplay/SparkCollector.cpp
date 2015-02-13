/******************************************************************************/
/*!
\file   SparkCollector.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "SparkCollector.h"

META_DEFINE(Framework::SparkCollector, SparkCollector)
{

}

namespace Framework
{
	GUIText* SparkCollector::guiText = nullptr;
	SparkCollector::~SparkCollector()
	{
		EVENTSYSTEM->mDisconnect<UpdateEvent, SparkCollector>(Events::UPDATEEVENT, this, &SparkCollector::Update);
		EVENTSYSTEM->mDisconnect<CollisionEvent, SparkCollector>(Events::COLLISION, this, &SparkCollector::CollideSpark);
	}

	void SparkCollector::CollideSpark(CollisionEvent* c)
	{
		if (c->thisObject && c->OtherObject)
		{
			if (c->thisObject->Name == "Spark"
				&& c->OtherObject->Name == "Player")
			{
				sparkCounter++;
				//delete spark
				//c->thisObject->ShapeCollider2D->~ShapeCollider2D();
				//c->thisObject->RigidBody2D->~RigidBody2D();
				//c->thisObject = nullptr;
			}
		}	
	}

	void SparkCollector::Initialize()
	{
		EVENTSYSTEM->mConnect<CollisionEvent, SparkCollector>(Events::COLLISION, this, &SparkCollector::CollideSpark);
		EVENTSYSTEM->mConnect<UpdateEvent, SparkCollector>(Events::UPDATEEVENT, this, &SparkCollector::Update);
		guiText = reinterpret_cast<GUIText*> (gameObject->AddComponent("GUIText"));
		guiText->position = { -0.2f, -1.9f };
		guiText->Initialize();
		sparkCounter = 0;
	}

	void SparkCollector::Serialize(Serializer::DataNode* data)
	{
		//TODO->Serailize
	}

	void SparkCollector::Update(UpdateEvent *u)
	{
		//GUIText* guiText;
		string txt = "Sparks Collected: " + std::to_string(sparkCounter);
		guiText->text = String(txt.c_str());
	}

	DefineComponentName(SparkCollector);
}
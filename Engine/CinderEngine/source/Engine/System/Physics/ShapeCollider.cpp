/******************************************************************************/
/*!
\file   ShapeCollider.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#include "ComponentInclude.h"
#include "IncludeForAllCollision.h"
#include "EventSystem.h"
#include "JSONSerializer.h"
#include "DebugRenderer.h"
#include "WindowSystem.h"

#pragma once
namespace Framework
{
	void ShapeCollider::SerializeMaterial(const char* name)
	{
		//opens file
		Serializer::DataNode* temp;
		Serializer::ZeroSerializer materialtype;
		name = Serializer::SkipHash(name);
		materialtype.open(name);
		materialtype.CreateArchive();
		temp = materialtype.GetTrunk()->branch;

		Serializer::DataNode* temp2;
		temp2 = temp->FindElement(temp, "Restitution");
		temp2->GetValue(&material.restitution);

		temp2 = temp->FindElement(temp, "StaticFriction");
		temp2->GetValue(&material.staticfriction);

		temp2 = temp->FindElement(temp, "DynamicFriction");
		temp2->GetValue(&material.dynamicfriction);

		temp2 = temp->FindElement(temp, "Density");
		temp2->GetValue(&material.density);

		material.name = std::string(name);

		materialtype.close();
	}

  //////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////
  // Debug Draw
  DebugRenderer* ShapeCollider::dr;

  ShapeCollider::ShapeCollider ()
  {
    if (dr == nullptr)
    {
      dr = new DebugRenderer ();
      dr->Initialize ();
    }

    WindowSystem::debugColliders.push_back (this);
  }

  ShapeCollider::~ShapeCollider ()
  {
    if (dr != nullptr)
    {
      delete dr;
      dr = nullptr;
    }

    WindowSystem::debugColliders.remove (this);
  }

  void ShapeCollider::Draw ()
  {}
  //////////////////////////////////////////////////////////////////////////

  DefineComponentName (ShapeCollider);
}
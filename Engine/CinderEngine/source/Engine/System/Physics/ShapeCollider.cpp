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
#include "ShapeCollider.h"

#pragma once
namespace Framework
{

	CircleCollider::~CircleCollider()
	{
	}

	PointCollider::PointCollider(GameObject *obj)
	{
		gameObject = obj;
	}

	PointCollider::~PointCollider()
	{
	}

	LineCollider::LineCollider(GameObject* obj)
	{
		gameObject = obj;
	}

	LineCollider::~LineCollider()
	{
	}

    DefineComponentName(ShapeCollider);
	DefineComponentName(CircleCollider);
	DefineComponentName(PointCollider);
	DefineComponentName(LineCollider);
}
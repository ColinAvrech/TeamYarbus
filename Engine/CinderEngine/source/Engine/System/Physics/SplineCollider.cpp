/******************************************************************************/
/*!
\file   SplineCollider.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ComponentInclude.h"
#include "IncludeForAllCollision.h"
#include "EventSystem.h"
#include "CollisionRepeats.h"
#include "Resolution.h"

namespace Framework
{
	void SplineCollider::Initialize()
	{
	};
	void SplineCollider::Serialize(Serializer::DataNode* data)
	{
	};
	void SplineCollider::Update()
	{
	};

	// Getters
	//glm::vec2 GetNormal(void) { return normalVec; }
	//float GetP1DotNormal(void) { return p1dotNormal; }

	// Private member functions
	void SplineCollider::DetectCircle(CircleCollider* c)
	{

	}  //if player is a circle
	void SplineCollider::DetectPoint(PointCollider* p)
	{
	}	  //if player is a point

	DefineComponentName(SplineCollider);
}
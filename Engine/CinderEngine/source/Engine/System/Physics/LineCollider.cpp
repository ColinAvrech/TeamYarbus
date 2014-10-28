/******************************************************************************/
/*!
\file   LineCollider.cpp
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
	void LineCollider::Initialize()
	{
		gameObject->LineCollider = this;
		normalVec = Physics::getNormal(p1, p2);
		p1dotNormal = Physics::DotProduct(p1, normalVec);
	}
	void LineCollider::Serialize(Serializer::DataNode* data)
	{
		Serializer::DataNode* temp;
		
		glm::vec3 objPosition = gameObject->Transform->GetPosition();
		glm::vec3 boxSize;
		glm::vec3 objScale = gameObject->Transform->GetScale();
		temp = data->FindElement(data, "Size");
		temp->GetValue(&boxSize);

		p1.x = objPosition.x - (boxSize.x * objScale.x);
		p1.y = objPosition.y + (boxSize.y * objScale.y);
		p2.x = objPosition.x + (boxSize.x * objScale.x);
		p2.y = objPosition.y + (boxSize.y * objScale.y);

	}
	void LineCollider::Update()
	{
	}

	// Getters
	//glm::vec2 GetNormal(void) { return normalVec; }
	//float GetP1DotNormal(void) { return p1dotNormal; }

	// Private member functions
	void LineCollider::DetectCircle(CircleCollider* c)
	{

	}  //if player is a circle
	void LineCollider::DetectPoint(PointCollider* p)
	{
	}	  //if player is a point

	DefineComponentName(LineCollider);
}
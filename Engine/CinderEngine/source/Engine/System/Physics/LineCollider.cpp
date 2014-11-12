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
  LineCollider::LineCollider (glm::vec3 _p1, glm::vec3 _p2)
  {
    Initialize (_p1, _p2);
  }

	void LineCollider::Initialize()
	{
		gameObject->ShapeCollider = this;
		normalVec = Physics::getNormal(p1, p2);
		p1dotNormal = Physics::DotProduct(p1, normalVec);
	}

  void LineCollider::Initialize (glm::vec3 _p1, glm::vec3 _p2)
  {
    p1 = _p1;
    p2 = _p2;
    normalVec = Physics::getNormal (p1, p2);
    p1dotNormal = Physics::DotProduct (p1, normalVec);
  }

  void LineCollider::Serialize (Serializer::DataNode* data)
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
	//glm::vec3 GetNormal(void) { return normalVec; }
	//float GetP1DotNormal(void) { return p1dotNormal; }

	// Private member functions
	void LineCollider::DetectCollision(CircleCollider* c)
	{

	}  //if player is a circle
	void LineCollider::DetectCollision(PointCollider* p)
	{
	}	  //if player is a point
	void LineCollider::DetectCollision(LineCollider *l)
	{
	}

	DefineComponentName(LineCollider);
}
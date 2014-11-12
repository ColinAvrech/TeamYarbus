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

  void SplineCollider::DetectCollision (CircleCollider * circle)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void SplineCollider::DetectCollision (LineCollider * line)
  {
    throw std::logic_error ("The method or operation is not implemented.");
  }

  void SplineCollider::AddLineCollider (glm::vec2 _p1, glm::vec2 _p2)
  {
	  glm::vec3 p1(_p1.x, _p1.y, 0.0f);
	  glm::vec3 p2(_p2.x, _p2.y, 0.0f);
    colliders.push_back (new LineCollider (p1, p2));
  }

  void SplineCollider::AddLineCollider (std::vector <std::pair <glm::vec2, glm::vec2>>& lines)
  {
    for (auto& i : lines)
    {
      AddLineCollider (i.first, i.second);
    }
  }

	DefineComponentName(SplineCollider);
}
/******************************************************************************/
/*!
\file   PointCollider.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ShapeCollider.h"

#pragma once
namespace Framework
{
	class LineCollider;
	class CircleCollider;

	class PointCollider : public ShapeCollider
	{
	public:
		const static std::string Name;
    PointCollider () {}
		~PointCollider(){}

		/*!Telegraph that the component is active*/
		void Initialize();
		void Serialize(Serializer::DataNode*);
		void Update();

		//collision
		void DetectCircle(CircleCollider* c);
		void DetectLine(LineCollider* l);
	};
}
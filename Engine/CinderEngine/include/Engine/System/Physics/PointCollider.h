/******************************************************************************/
/*!
\file   PointCollider.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ShapeCollider.h"


namespace Framework
{
	class LineCollider;
	class CircleCollider;

	class PointCollider : public ShapeCollider
	{
	public:
		//ZilchDeclareDerivedType(PointCollider, ShapeCollider);
		const static string Name;
    PointCollider () {}
		~PointCollider(){}

		/*!Telegraph that the component is active*/
		void Initialize();
		void Serialize(Serializer::DataNode*);
		void Update();

		//collision
		void DetectCollision(CircleCollider* c);
		void DetectCollision(LineCollider* l);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // Debug Draw
    virtual void Draw ();
    //////////////////////////////////////////////////////////////////////////
	};
}
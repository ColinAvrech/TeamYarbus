/******************************************************************************/
/*!
\file   CircleCollider.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ShapeCollider.h"


namespace Framework
{
	//Forward Declaration
	class PointCollider;
	class LineCollider;
	class SplineCollider;

	class CircleCollider : public ShapeCollider
	{
	public:
		//ZilchDeclareDerivedType(CircleCollider, ShapeCollider);
		const static string Name;
		float radius;
		CircleCollider(){}

		virtual ~CircleCollider(){};

		/*!Telegraph that the component is active*/
		void Initialize();
		void Serialize(Serializer::DataNode* data);
		void Update();

		// Getters
		float GetRadius(void){
			return radius;
		};

		void DetectCollision(CircleCollider* c);
		void DetectCollision(PointCollider* p);
		void DetectCollision(LineCollider* l);
		void DetectCollision(SplineCollider* s);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    // Debug Draw
    virtual void Draw ();
    //////////////////////////////////////////////////////////////////////////

	private:
		// Private data
		
	};
}
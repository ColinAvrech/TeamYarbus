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

	class CircleCollider : public ShapeCollider
	{
	public:
		const static std::string Name;
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

		void DetectCircle(CircleCollider* c);
		void DetectPoint(PointCollider* p);
		void DetectLine(LineCollider* l);

	private:
		// Private data
		
	};
}
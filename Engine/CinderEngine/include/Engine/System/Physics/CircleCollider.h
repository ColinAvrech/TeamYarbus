#include "ShapeCollider.h"

#pragma once
namespace Framework
{
	//Forward Declaration
	class PointCollider;
	class LineCollider;

	class CircleCollider : public ShapeCollider
	{
	public:
		const static std::string Name;
		CircleCollider(){}
		CircleCollider(GameObject* obj)
		{
			gameObject = obj;
		}

		~CircleCollider(){};

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
		float radius;
	};
}
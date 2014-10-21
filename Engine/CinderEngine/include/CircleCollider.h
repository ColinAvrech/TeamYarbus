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
		CircleCollider(){}
		CircleCollider(GameObject* obj) : radius(0.5f)
		{
			gameObject = obj;
		}

		~CircleCollider(){};

		/*!Telegraph that the component is active*/
		void Initialize();
		void Serialize();
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
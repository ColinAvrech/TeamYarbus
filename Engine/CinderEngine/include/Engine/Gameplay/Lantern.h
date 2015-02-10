/******************************************************************************/
/*!
\file   Lantern.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The players Lantern
*/
/******************************************************************************/
#pragma once

namespace Framework
{
	class UpdateEvent;

	class Lantern : public Component
	{
	public:
		const static std::string Name;
		Lantern(GameObject * obj)
		{
			gameObject = obj;
		}
		Lantern(){}

		~Lantern(){};
		void reFuel();
		void Update(UpdateEvent* e);
		virtual void Serialize(Serializer::DataNode* data);
		virtual void Initialize();
		void CalculateBounds();
		void CheckCollision();

	private:
		//flashlight
		float lightRadius;
		double lightTheta;
		glm::vec2 origin;
		glm::vec2 leftBounds;
		glm::vec2 rightBounds;
	};
}
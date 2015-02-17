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

	class Lantern : public IGraphicsObject
	{
	public:
		const static std::string Name;
		Lantern(GameObject * obj)
		{
			gameObject = obj;
		}
		Lantern(){}

		~Lantern();
		void reFuel();
		void Update(UpdateEvent* e);
		virtual void Serialize(Serializer::DataNode* data);
		virtual void Initialize();
		void CalculateBounds();
		void CheckCollision();
		void Draw();
		virtual bool Draw_Last() { return true; }
		virtual bool InViewport() { return true; }

		float getLightTheta(){ return lightTheta; }
		float getAngleFromGround(){ return angleFromGround; }
		bool getRainedOn(){ return rainedOn; }
		void setRainedOn(bool check){ rainedOn = check; }


	private:
		//basic shape
		float lightRadius;
		float lightTheta;
		float lightThetaRad; //in radians
		float angleFromGround;
		glm::vec2 origin;
		glm::vec2 leftBounds;
		glm::vec2 rightBounds;
		float decayRate;

		//interaction with rain
		bool rainedOn;
		float rainDecayRate;
	
	};
}
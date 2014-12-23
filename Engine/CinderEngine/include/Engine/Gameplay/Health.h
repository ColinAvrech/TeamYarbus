/******************************************************************************/
/*!
\file   Health.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The players health
*/
/******************************************************************************/
#pragma once


namespace Framework
{
	class UpdateEvent;
	class PlayerEffect;

	class Health : public Component
	{
	public:
		const static std::string Name;
		Health(GameObject * obj)
		{
			gameObject = obj;
		}
		Health(){}

		~Health();
		void reFuel();
		void Update(UpdateEvent* e);
		virtual void Serialize(Serializer::DataNode* data);
		virtual void Initialize();
		void ToggleInvulnerability();

		float timer = 0.0f;
		bool levelFailed = false;
		float startingDeathRate = 0.2f;
		float currentDeathRate;
		float maxRadius;
		float minRadius;
		float currentRadius;
		bool alive;
		bool reFueling;
		bool invincible;
		glm::vec3 originalPosition;
		PlayerEffect* playerEffect;
	};
}
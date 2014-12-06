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
#include "Component.h"

namespace Framework
{
	class UpdateEvent;
	class CollisionEvent;
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
		void OnCollisionEnter(CollisionEvent* c);
		virtual void Serialize(Serializer::DataNode* data);
    virtual void Initialize(); 
    void ToggleInvulnerability();

    float timer = 0.0f;
    bool levelFailed = false;
<<<<<<< HEAD
    float deathRate = 0.2f;
=======
    float currentDeathRate;
    float startDeathRate = 0.02f;
>>>>>>> be0571b82dddc73c07ab5f9dc35d2ad7aa73454b
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
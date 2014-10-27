#pragma once
#include "Component.h"

namespace Framework
{
	class KeyEvent;
	class CollisionEvent;
	class UpdateEvent;

	class CharacterController : public Component
	{
	public:
		const static std::string Name;
		float accel;
		float drag;
		float maxXVel;
		float jumpVel;

		CharacterController(GameObject * obj)
		{
			gameObject = obj;
		}

		CharacterController(){}

		~CharacterController();

		/*!Telegraph that the component is active*/
		virtual void Initialize();
		virtual void Serialize(Serializer::DataNode* data);

		//Registered to events
		void Update(UpdateEvent* e);
		void OnKeyPressed(KeyEvent* key);
		void OnCollisionEnter(CollisionEvent* collision);
	};
}
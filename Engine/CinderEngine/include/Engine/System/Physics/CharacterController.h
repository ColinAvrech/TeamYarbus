#pragma once
#include "Component.h"


namespace Framework
{
	class CharacterController : public Component
	{
	public:
		const static std::string Name;
		CharacterController(GameObject * obj)
		{
			gameObject = obj;
		}

		CharacterController(){}

		~CharacterController();

		/*!Telegraph that the component is active*/
		virtual void Initialize();
		virtual void Serialize(Serializer::DataNode* data);
		void Update();

	};
}
/******************************************************************************/
/*!
\file   Splitscreen.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains logic for how the player will switch between worlds
*/
/******************************************************************************/
#pragma once
#include "Component.h"

namespace Framework
{
	class KeyEvent;
	class UpdateEvent;
	class DataNode;

	class Splitscreen : public Component
	{
	public:
		const static string Name;
		enum state
		{
			DARK_WORLD = 0,
			LIGHT_WORLD,
			TRANS_TOLIGHT,
			TRANS_TODARK
		};
		state currentState;
		state lastState;
		Splitscreen(GameObject* obj) 
		{
			gameObject = obj;
		}
		Splitscreen(){};
		~Splitscreen();

		virtual void Initialize();
		void Serialize(Serializer::DataNode* data);
		void CalculateFlashlight();
		void EnableInteractions();
		void Update(UpdateEvent* e);
		void KeyPressed(KeyEvent* e);

	private:
		//flashlight
		float lightRadius;
		double lightTheta;
		glm::vec2 origin;
		glm::vec2 leftBounds;
		glm::vec2 rightBounds;
		bool flashlight = true;

		//transitioning
		double t_timer;
		double t_maxtime;

		//light world
		double lworld_timer;
		double lworld_maxtime;
	};
}
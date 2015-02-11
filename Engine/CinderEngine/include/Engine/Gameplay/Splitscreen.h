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

		Splitscreen(GameObject* obj) 
		{
			gameObject = obj;
		}
		Splitscreen(){};
		~Splitscreen();

		virtual void Initialize();
		void Serialize(Serializer::DataNode* data);
		void EnableInteractions();
		void Update(UpdateEvent* e);
		void KeyPressed(KeyEvent* e);

	private:
		//transitioning
		//double t_timer;
		//double t_maxtime;

		////light world
		//double lworld_timer;
		//double lworld_maxtime;
	};
}

/******************************************************************************/
/*!
\file   Rain.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Rain behavior - randomized and interacting with wind
*/
/******************************************************************************/
#pragma once

namespace Framework
{
	class UpdateEvent;

	class Rain : public Component
	{
	public:

		const static std::string Name;
		Rain(GameObject * obj)
		{
			gameObject = obj;
		}
		Rain(){}

		~Rain();
		void Update(UpdateEvent* e);
		virtual void Serialize(Serializer::DataNode* data);
		virtual void Initialize();

	private: 
		float rainAngle;
	};
}
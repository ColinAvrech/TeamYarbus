/******************************************************************************/
/*!
\file   FpsCounter.h
\author Damien Low
\par    Course: GAM250
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief	Component for displaying FPS
*/
/******************************************************************************/
#pragma once

#include "Component.h"

namespace Framework
{
	class FpsCounter : public Component
	{
	public:
		FpsCounter(void);
		~FpsCounter(void);

		virtual void Initialize();
		virtual void Serialize(Serializer::DataNode* data);

		void Update(UpdateEvent* e);

		// The non-base component uses DefineComponentName macro to name component
		const static std::string Name;

	private:
		GUIText *guiText_;
	};
}
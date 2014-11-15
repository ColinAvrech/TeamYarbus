/******************************************************************************/
/*!
\file   GameLogic.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The logic for burning gameObjects
*/
/******************************************************************************/
#include "Component.h"

namespace Framework
{
	class GameLogic : public Component
	{
	public:
		void lightOnFire();
		bool onFire;		
	};
}
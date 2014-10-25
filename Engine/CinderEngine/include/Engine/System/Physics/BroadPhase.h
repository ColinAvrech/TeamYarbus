/******************************************************************************/
/*!
\file   BroadPhase.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This contains the data for doing a broad phase collision check
		Collects possible collisions and stores them in ObjectPair
		struct
*/
/******************************************************************************/
#include "GameObject.h"
#pragma once

namespace Framework
{
	class BroadPhase
	{
		struct ObjectPair
		{
			GameObject * A;
			GameObject * B;
		};

		void GeneratePairs(void);
		bool SortPairs(void);
		ObjectPair pairs;
	};

}
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
#include "DataNode.h"

namespace Framework
{
	class FireStarter : public Component
	{
	public:
		FireStarter(){}
		~FireStarter(){}
		void lightOnFire();
		void Serialize(Serializer::DataNode* data);
		void Initialize();

		const static std::string Name;
		bool onFire;		
	};
}
/******************************************************************************/
/*!
\file   Microphone.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Microphone active
*/
/******************************************************************************/
#include "Component.h"
#include "DataNode.h"

namespace Framework
{
	class Microphone : public Component
	{
	public:
		Microphone(){}
		~Microphone(){}
		void Serialize(Serializer::DataNode* data);
		void Initialize();

		const static std::string Name;
		bool Active;
	};
}
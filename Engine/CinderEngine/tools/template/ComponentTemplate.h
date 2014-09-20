/******************************************************************************/
/*!
\file   ComponentSystem.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#pragma once


namespace Framework
{
	class Template_Component : public GameComponent
	{
		public:
			Template_Component(GameObject* ob);
			~Template_Component();



		// The non-base component uses DefineComponentName macro to name component
		const static std::string Name;
		
		private:


	}
}
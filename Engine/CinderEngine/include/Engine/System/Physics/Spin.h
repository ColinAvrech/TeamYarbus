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
	class Spin : public Component
	{
		public:
			Spin();
			~Spin();
			
				
			virtual void Initialize ();
			virtual void Serialize (Serializer::DataNode* data);
      void OnUpdate(UpdateEvent *e);



		// The non-base component uses DefineComponentName macro to name component
		const static std::string Name;
		
		private:
      


  };
}
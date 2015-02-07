/******************************************************************************/
/*!
\file   SparkCollector.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once


namespace Framework
{

	class SparkCollector : public Component
	{
	public:
		META_DECLARE( SparkCollector );

		static GUIText* guiText;
		SparkCollector(GameObject* obj)
		{
			gameObject = obj;
		}
		SparkCollector(){}
		~SparkCollector();


		virtual void Initialize();
		virtual void Serialize(Serializer::DataNode* data);
		virtual void Update(UpdateEvent* u);
		void CollideSpark(CollisionEvent* c);


		// The non-base component uses DefineComponentName macro to name component
		const static std::string Name;

	private:
		int sparkCounter = 0;
	};
}

META_DEFINE( Framework::SparkCollector, SparkCollector )
{

}
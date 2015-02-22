#ifndef OBJECTPROPERTIESPANEL_H_
#define OBJECTPROPERTIESPANEL_H_

#include "GameObject.h"

#include "EditorBase.h"
#include "PanelManager.h"

namespace Editor
{

	class ObjectPropertiesPanel : public EditorBase
	{
	public:

		ObjectPropertiesPanel( );
		~ObjectPropertiesPanel( );

		void SetVisible( bool state );
		void Refresh( );
		void Reset( );

		void Focus( Framework::GameObject * obj );

		//////////////////////////////////////////////////
		// Buttons
		//////////////////////////////////////////////////
		static void TW_CALL OpenComponentList( void * );
		static void TW_CALL CloseComponentList( void * );

		static void TW_CALL AddComponent( void * type );
		static void TW_CALL RemoveComponent( void * type );

	private:
		//////////////////////////////////////////////////
		// Properties Panel
		//////////////////////////////////////////////////
		void OpenObjectProperties( );
		void CloseObjectProperties( );

		void SetupObjectPropertiesPanelProperties( );
		void PopulateObjectProperties( );

		void ReadType( const Reflection::MetaType & metatype, void * component, const std::string & groupName );
		void ReadMember( const Reflection::MetaType & metatype, const Reflection::MetaMember & member, void * component, const std::string & groupName );
		void AddMember( const Reflection::MetaMember & member, const Reflection::MetaType & value, void * component, const std::string & groupName );

		//////////////////////////////////////////////////
		// Component List Panel
		//////////////////////////////////////////////////
		void OpenComponentList( );
		void CloseComponentList( );

		void SetupComponentListPanelProperties( );
		void PopulateComponentList( );

		Framework::GameObject * mFocusObject = nullptr;
		std::string mFocusObjectName;

		Panel::PanelManager::PanelHandle mObjectPropertiesPanel;
		Panel::PanelManager::PanelHandle mComponentListPanel;
		

	};

}

#endif
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

	private:
		void SetupObjectPropertiesPanelProperties( );

		void Open( );
		void Close( );

		void PopulateObjectProperties( );
		void ReadComponent( const Reflection::MetaType & metatype, void * component );
		void ReadMember( const Reflection::MetaType & metatype, const Reflection::MetaMember & member, void * component );
			
		Framework::GameObject * mFocusObject = nullptr;

		Panel::PanelManager::PanelHandle mObjectPropertiesPanel;
		

	};

}

#endif
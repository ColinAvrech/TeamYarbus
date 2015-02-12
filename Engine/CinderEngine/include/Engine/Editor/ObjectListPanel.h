#ifndef OBJECTLISTPANEL_H_
#define OBJECTLISTPANEL_H_

#include "EditorBase.h"
#include "PanelManager.h"

namespace Editor
{

	class ObjectListPanel : public EditorBase
	{

	public:
		ObjectListPanel( );
		~ObjectListPanel( );

		void SetVisible( bool state );
		void Refresh( );
		void Reset( );

		//////////////////////////////////////////////////
		// Buttons
		//////////////////////////////////////////////////
		static void TW_CALL SelectObject( void * obj );
		static void TW_CALL CreateObject( void * );

	private:
		void Open( );
		void Close( );

		void SetupObjectListPanelProperties( );
		void PopulateObjectList( );

	private:
		unsigned int mObjectCount = 0;

		Panel::PanelManager::PanelHandle mObjectListPanel;

		std::string mNewObjectName;
	};

}

#endif
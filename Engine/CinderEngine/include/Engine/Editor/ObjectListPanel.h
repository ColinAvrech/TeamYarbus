#ifndef OBJECTLISTPANEL_H_
#define OBJECTLISTPANEL_H_

#include "EditorBase.h"
#include "PanelManager.h"

namespace Editor
{

	class ObjectListPanel : public EditorBase
	{
	public:
		static void TW_CALL SelectObject( void * obj );

	public:
		ObjectListPanel( );
		~ObjectListPanel( );

		void SetVisible( bool state );
		void Refresh( );
		void Reset( );

	private:
		void Open( );
		void Close( );

		void SetupObjectListPanelProperties( );
		void PopulateObjectList( );

	private:
		unsigned int mObjectCount = 0;
		// std::vector< Framework::GameObject * > mObjects;

		Panel::PanelManager::PanelHandle mObjectListPanel;
	};

}

#endif
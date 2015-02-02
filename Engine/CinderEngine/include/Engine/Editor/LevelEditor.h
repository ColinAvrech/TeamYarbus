#ifndef LEVELEDITOR_H_
#define LEVELEDITOR_H_

#include "EditorBase.h"
#include "ObjectListPanel.h"
#include "ObjectPropertiesPanel.h"

#include <vector>

namespace Editor
{

	class LevelEditor
	{
	public:
		LevelEditor( );
		~LevelEditor( );

		void InitializeEditor( );
		void ShutdownEditor( );

		void SetVisible( bool state );
		void Refresh( );

		void Reset( );

		void HandleInput( Framework::KeyEvent * e );

		ObjectListPanel & GetObjectListPanel( );
		ObjectPropertiesPanel & GetObjectPropertiesPanel( );

	private:

		bool mIsVisible = false;
		std::vector<EditorBase *> mEditorPanels;

	};

}

namespace Framework
{
	extern Editor::LevelEditor * LEVELEDITOR;
}

#endif
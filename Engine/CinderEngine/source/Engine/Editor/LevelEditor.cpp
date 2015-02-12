#include "Precompiled.h"

#include "LevelEditor.h"
#include "ObjectListPanel.h"
#include "ObjectPropertiesPanel.h"

namespace Framework
{
	Editor::LevelEditor * LEVELEDITOR = nullptr;
}

namespace Editor
{


	LevelEditor::LevelEditor( )
	{
		Framework::LEVELEDITOR = this;
	}

	LevelEditor::~LevelEditor( )
	{

	}

	void LevelEditor::InitializeEditor( )
	{
		// connect input handler
		Framework::EVENTSYSTEM->mConnect<Framework::KeyEvent, LevelEditor>( Framework::Events::KEY_GRAVEACCENT,
																			this,
																			&LevelEditor::HandleInput );

		mEditorPanels.push_back( new ObjectListPanel( ) );
		mEditorPanels.push_back( new ObjectPropertiesPanel( ) );

		SetVisible( false );
		
		Framework::PANELMANAGER->Define( "buttonalign", "center" );
		Framework::PANELMANAGER->Define( "contained", "true" );
		Framework::PANELMANAGER->Define( "fontsize", "1" );
		Framework::PANELMANAGER->Define( "fontresizable", "false" );
		Framework::PANELMANAGER->Define( "help", "'I am Melvin the Great! Hear me rawr!'" );
	}

	void LevelEditor::ShutdownEditor( )
	{
		// disconnect input handler
		Framework::EVENTSYSTEM->mDisconnect<Framework::KeyEvent, LevelEditor>( Framework::Events::KEY_GRAVEACCENT,
																			   this,
																			   &LevelEditor::HandleInput );

		for ( auto panel : mEditorPanels )
			delete panel;
	}

	ObjectListPanel & LevelEditor::GetObjectListPanel( )
	{
		return *reinterpret_cast< ObjectListPanel * >( mEditorPanels[ 0 ] );
	}

	ObjectPropertiesPanel & LevelEditor::GetObjectPropertiesPanel( )
	{
		return *reinterpret_cast< ObjectPropertiesPanel * >( mEditorPanels[ 1 ] );
	}

	void LevelEditor::SetVisible( bool state )
	{
		for ( auto panel : mEditorPanels )
			panel->SetVisible( state );
	}

	void LevelEditor::Refresh( )
	{
		for ( auto panel : mEditorPanels )
			panel->Refresh( );
	}

	void LevelEditor::Reset( )
	{
		for ( auto panel : mEditorPanels )
			panel->Reset( );
	}

	void LevelEditor::HandleInput( Framework::KeyEvent * e )
	{
		if ( e->KeyDown == true )
		{
			mIsVisible = !mIsVisible;
			SetVisible( mIsVisible );
		}
	}

}
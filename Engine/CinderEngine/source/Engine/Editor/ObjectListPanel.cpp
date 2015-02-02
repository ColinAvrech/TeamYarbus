#include "Precompiled.h"

#include "LevelEditor.h"
#include "ObjectListPanel.h"

namespace Editor
{

	void TW_CALL ObjectListPanel::SelectObject( void * obj )
	{
		Framework::LEVELEDITOR->GetObjectPropertiesPanel( ).Focus( reinterpret_cast< Framework::GameObject* >( obj ) );
	}

	ObjectListPanel::ObjectListPanel( ) :
		mObjectListPanel( Framework::PANELMANAGER->AddPanel( "Object List" ) )
	{
		SetupObjectListPanelProperties( );
	}

	ObjectListPanel::~ObjectListPanel( )
	{
		Framework::PANELMANAGER->RemovePanel( mObjectListPanel );
	}

	void ObjectListPanel::SetVisible( bool state )
	{
		if ( state == true )
			Open( );
		else
			Close( );
	}

	void ObjectListPanel::Refresh( )
	{

	}

	void ObjectListPanel::Reset( )
	{
		mObjectCount = 0;
		// mObjects.clear( );
	}

	void ObjectListPanel::Open( )
	{
		Close( );

		mObjectListPanel->SetVisible( true );
		PopulateObjectList( );
	}

	void ObjectListPanel::Close( )
	{
		mObjectListPanel->SetVisible( false );
		mObjectListPanel->ClearPanel( );

		Reset( );
	}

	void ObjectListPanel::SetupObjectListPanelProperties( )
	{
		Real windowX = static_cast< Real >( Framework::WINDOWSYSTEM->Get_Width( ) );
		Real sizeX = static_cast< Real >( windowX * 0.15f );
		Real windowY = static_cast< Real >( Framework::WINDOWSYSTEM->Get_Height( ) );
		Real sizeY = static_cast< Real >( windowY );

		mObjectListPanel->SetSize( sizeX, sizeY );
		mObjectListPanel->SetPosition( 0.0f, 0.0f );
		mObjectListPanel->SetIconifiable( false );
		mObjectListPanel->SetResizable( false );
		mObjectListPanel->SetMoveable( false );
	}

	void ObjectListPanel::PopulateObjectList( )
	{
		auto objectsFirst = Framework::OBJECTSYSTEM->GameObjects.begin( );
		auto objectsEnd = Framework::OBJECTSYSTEM->GameObjects.end( );

		// mObjects.reserve( std::distance( objectsFirst, objectsEnd ) );

		while ( objectsFirst != objectsEnd )
		{
			std::string name = std::to_string( mObjectCount ) + ". " + objectsFirst->second->GetName( ).c_str( );
			// mObjects.push_back( objectsFirst->second );

			mObjectListPanel->AddButton( name, &ObjectListPanel::SelectObject, objectsFirst->second );

			++mObjectCount;
			++objectsFirst;
		}
	}

}
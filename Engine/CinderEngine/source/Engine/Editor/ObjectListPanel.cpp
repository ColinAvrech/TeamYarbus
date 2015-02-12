#include "Precompiled.h"

#include "LevelEditor.h"
#include "ObjectListPanel.h"

namespace Editor
{

	ObjectListPanel::ObjectListPanel( ) :
		mObjectListPanel( Framework::PANELMANAGER->AddPanel( "Object List" ) )
	{

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
		SetVisible( false );
		SetVisible( true );
	}

	void ObjectListPanel::Reset( )
	{
		mObjectCount = 0;
	}

	//////////////////////////////////////////////////
	// Buttons
	//////////////////////////////////////////////////
	void TW_CALL ObjectListPanel::SelectObject( void * obj )
	{
		Framework::LEVELEDITOR->GetObjectPropertiesPanel( ).Focus( reinterpret_cast< Framework::GameObject* >( obj ) );
	}

	void TW_CALL ObjectListPanel::CreateObject( void * )
	{
		Framework::GameObject * newobj = Framework::OBJECTSYSTEM->CreateObject( );
		newobj->Name = Framework::LEVELEDITOR->GetObjectListPanel( ).mNewObjectName.c_str( );
		Framework::LEVELEDITOR->GetObjectListPanel( ).Refresh( );
	}


	void ObjectListPanel::Open( )
	{
		SetupObjectListPanelProperties( );
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
		// new object creation
		mObjectListPanel->AddField( "Name", &mNewObjectName );
		mObjectListPanel->AddButton( "Create", &ObjectListPanel::CreateObject, nullptr );
		mObjectListPanel->AddSeperator( );

		// objects
		auto objectsFirst = Framework::OBJECTSYSTEM->GameObjects.begin( );
		auto objectsEnd = Framework::OBJECTSYSTEM->GameObjects.end( );

		while ( objectsFirst != objectsEnd )
		{
			std::string name = std::to_string( mObjectCount ) + ". " + objectsFirst->second->GetName( ).c_str( );

			mObjectListPanel->AddButton( name, &ObjectListPanel::SelectObject, objectsFirst->second );

			++mObjectCount;
			++objectsFirst;
		}
	}

}
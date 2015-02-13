#include "Precompiled.h"

#include "LevelEditor.h"
#include "ObjectPropertiesPanel.h"

#include "MetaTypeDB.h"

namespace Editor
{


	ObjectPropertiesPanel::ObjectPropertiesPanel( ) :
		mObjectPropertiesPanel( Framework::PANELMANAGER->AddPanel( "Object Properties" ) ),
		mComponentListPanel( Framework::PANELMANAGER->AddPanel( "Component List" ) )
	{
		SetupObjectPropertiesPanelProperties( );
		SetupComponentListPanelProperties( );
	}

	ObjectPropertiesPanel::~ObjectPropertiesPanel( )
	{
		Framework::PANELMANAGER->RemovePanel( mObjectPropertiesPanel );
		Framework::PANELMANAGER->RemovePanel( mComponentListPanel );
	}

	void ObjectPropertiesPanel::SetVisible( bool state )
	{
		if ( state == true )
			OpenObjectProperties( );
		else
			CloseObjectProperties( );
	}

	void ObjectPropertiesPanel::Refresh( )
	{
		SetVisible( false );
		SetVisible( true );
	}

	void ObjectPropertiesPanel::Reset( )
	{
		mFocusObject = nullptr;
	}

	void ObjectPropertiesPanel::Focus( Framework::GameObject * obj )
	{
		Reset( );
		mFocusObject = obj;

		OpenObjectProperties( );
		CloseComponentList( );
	}

	//////////////////////////////////////////////////
	// Buttons
	//////////////////////////////////////////////////
	void TW_CALL ObjectPropertiesPanel::OpenComponentList( void * )
	{
		Framework::LEVELEDITOR->GetObjectPropertiesPanel( ).OpenComponentList( );
	}

	void TW_CALL ObjectPropertiesPanel::CloseComponentList( void * )
	{
		Framework::LEVELEDITOR->GetObjectPropertiesPanel( ).CloseComponentList( );
	}

	void TW_CALL ObjectPropertiesPanel::AddComponent( void * type )
	{
		const Reflection::MetaType * mt = reinterpret_cast< const Reflection::MetaType * >( type );
		Framework::LEVELEDITOR->GetObjectPropertiesPanel( ).mFocusObject->AddComponent( mt->GetName( ) );

		Framework::LEVELEDITOR->GetObjectPropertiesPanel( ).CloseComponentList( );
		Framework::LEVELEDITOR->GetObjectPropertiesPanel( ).Refresh( );
	}

	void TW_CALL ObjectPropertiesPanel::RemoveComponent( void * type )
	{
		const Reflection::MetaType * mt = reinterpret_cast< const Reflection::MetaType * >( type );
		Framework::LEVELEDITOR->GetObjectPropertiesPanel( ).mFocusObject->RemoveComponent( mt->GetName( ) );
		Framework::LEVELEDITOR->GetObjectPropertiesPanel( ).Refresh( );
	}

	//////////////////////////////////////////////////
	// Properties Panel
	//////////////////////////////////////////////////

	void ObjectPropertiesPanel::OpenObjectProperties( )
	{
		SetupObjectPropertiesPanelProperties( );
		CloseObjectProperties( );

		mObjectPropertiesPanel->SetVisible( true );
		PopulateObjectProperties( );
	}

	void ObjectPropertiesPanel::CloseObjectProperties( )
	{
		mObjectPropertiesPanel->SetVisible( false );
		mObjectPropertiesPanel->ClearPanel( );
	}

	void ObjectPropertiesPanel::SetupObjectPropertiesPanelProperties( )
	{
		Real windowX = static_cast< Real >( Framework::WINDOWSYSTEM->Get_Width( ) );
		Real windowY = static_cast< Real >( Framework::WINDOWSYSTEM->Get_Height( ) );
		Real sizeX = static_cast< Real >( windowX * 0.2f );
		Real sizeY = static_cast< Real >( windowY );

		mObjectPropertiesPanel->SetSize( sizeX, sizeY );
		mObjectPropertiesPanel->SetPosition( windowX - sizeX, 0.0f );
		mObjectPropertiesPanel->SetIconifiable( false );
		mObjectPropertiesPanel->SetResizable( false );
		mObjectPropertiesPanel->SetMoveable( false );
		mObjectPropertiesPanel->SetVisible( false );
	}

	void ObjectPropertiesPanel::PopulateObjectProperties( )
	{
		if ( mFocusObject == nullptr )
		{
			mObjectPropertiesPanel->AddLabel( "--No object selected--" );
			return;
		}

		mFocusObjectName = mFocusObject->GetName( ).c_str( );

		mObjectPropertiesPanel->AddField( "Object Name", &mFocusObjectName, "", mFocusObject == nullptr );
		mObjectPropertiesPanel->AddSeperator( );

		mObjectPropertiesPanel->AddButton( "Add Component", &ObjectPropertiesPanel::OpenComponentList, nullptr );
		mObjectPropertiesPanel->AddSeperator( );

		auto componentFirst = mFocusObject->Components.begin( );
		auto componentLast = mFocusObject->Components.end( );

		// loop through all the component in the type
		while ( componentFirst != componentLast )
		{
			const Reflection::MetaType & mt = META_HASH( componentFirst->second->mComponentType );
			ReadComponent( mt, componentFirst->second );
			mObjectPropertiesPanel->AddSeperator( "", mt.GetName( ) );
			mObjectPropertiesPanel->AddButton( "Remove Component", &ObjectPropertiesPanel::RemoveComponent,
											   const_cast< Reflection::MetaType * > ( &mt ), mt.GetName( ) );

			++componentFirst;
		}

	}

	void ObjectPropertiesPanel::ReadComponent( const Reflection::MetaType & metatype, void * component )
	{
		auto memberFirst = metatype.Begin( );
		auto memberLast = metatype.End( );

		// loop through all the members in the type
		while ( memberFirst != memberLast )
		{
			ReadMember( metatype, *memberFirst, component );
			++memberFirst;
		}
	}

	void ObjectPropertiesPanel::ReadMember( const Reflection::MetaType & metatype,
											const Reflection::MetaMember & member,
											void * component )
	{
		switch ( HASH_TO_PANEL_TYPE( member.GetType( ).GetHashedName( ) ) )
		{
		case TW_TYPE_BOOLCPP:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<bool>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_INT8:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<char>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_INT16:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<short>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_INT32:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<int>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_UINT8:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<unsigned char>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_UINT16:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<unsigned short>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_UINT32:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<unsigned >( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_FLOAT:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<float>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_DOUBLE:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<double>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_STDSTRING:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<std::string>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_DIR2F:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<glm::vec2>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_DIR3F:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<glm::vec3>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_DIR4F:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<glm::vec4>( component ), metatype.GetName( ) );
			break;

		case TW_TYPE_QUAT4F:
			mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<glm::quat>( component ), metatype.GetName( ) );
			break;
		}

		return;
	}

	//////////////////////////////////////////////////
	// Component List Panel
	//////////////////////////////////////////////////
	void ObjectPropertiesPanel::OpenComponentList( )
	{
		SetupComponentListPanelProperties( );
		CloseComponentList( );

		mComponentListPanel->SetVisible( true );
		PopulateComponentList( );
	}

	void ObjectPropertiesPanel::CloseComponentList( )
	{
		mComponentListPanel->SetVisible( false );
		mComponentListPanel->ClearPanel( );
	}

	void ObjectPropertiesPanel::SetupComponentListPanelProperties( )
	{
		Real windowX = static_cast< Real >( Framework::WINDOWSYSTEM->Get_Width( ) );
		Real windowY = static_cast< Real >( Framework::WINDOWSYSTEM->Get_Height( ) );
		Real sizeX = static_cast< Real >( windowX * 0.2f );
		Real sizeY = static_cast< Real >( windowY );

		mComponentListPanel->SetSize( sizeX, sizeY );
		mComponentListPanel->SetPosition( windowX - sizeX * 2.0f, 0.0f );
		mComponentListPanel->SetIconifiable( false );
		mComponentListPanel->SetResizable( false );
		mComponentListPanel->SetMoveable( false );
		mComponentListPanel->SetVisible( false );
	}

	void ObjectPropertiesPanel::PopulateComponentList( )
	{
		mComponentListPanel->AddButton( "Close", &ObjectPropertiesPanel::CloseComponentList, nullptr );
		mComponentListPanel->AddSeperator( );

		auto types = META_TAG( "Component" );

		for ( auto & type : types )
		{
			mComponentListPanel->AddButton( type->GetName( ), &ObjectPropertiesPanel::AddComponent,
											const_cast< Reflection::MetaType * >( type ) );
		}
	}

}
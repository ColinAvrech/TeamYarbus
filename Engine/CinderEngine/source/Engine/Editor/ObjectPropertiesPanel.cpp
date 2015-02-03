#include "Precompiled.h"
#include "ObjectPropertiesPanel.h"

#include "MetaTypeDB.h"

namespace Editor
{

	ObjectPropertiesPanel::ObjectPropertiesPanel( ) :
		mObjectPropertiesPanel( Framework::PANELMANAGER->AddPanel( "Object Properties" ) )
	{
		SetupObjectPropertiesPanelProperties( );
	}

	ObjectPropertiesPanel::~ObjectPropertiesPanel( )
	{
		Framework::PANELMANAGER->RemovePanel( mObjectPropertiesPanel );
	}

	void ObjectPropertiesPanel::SetVisible( bool state )
	{
		if ( state == true )
			Open( );
		else
			Close( );
	}

	void ObjectPropertiesPanel::Refresh( )
	{

	}

	void ObjectPropertiesPanel::Reset( )
	{
		mObjectPropertiesPanel->ClearPanel( );
		mFocusObject = nullptr;
	}

	void ObjectPropertiesPanel::Focus( Framework::GameObject * obj )
	{
		Reset( );
		mFocusObject = obj;
		PopulateObjectProperties( );
	}

	void ObjectPropertiesPanel::SetupObjectPropertiesPanelProperties( )
	{
		Real windowX = static_cast< Real >( Framework::WINDOWSYSTEM->Get_Width( ) );
		Real sizeX = static_cast< Real >( windowX * 0.2f );
		Real sizeY = static_cast< Real >( Framework::WINDOWSYSTEM->Get_Height( ) );

		mObjectPropertiesPanel->SetSize( sizeX, sizeY );
		mObjectPropertiesPanel->SetPosition( windowX - sizeX, 0.0f );
		mObjectPropertiesPanel->SetIconifiable( false );
		mObjectPropertiesPanel->SetResizable( false );
		mObjectPropertiesPanel->SetMoveable( false );
	}

	void ObjectPropertiesPanel::Open( )
	{
		Close( );

		mObjectPropertiesPanel->SetVisible( true );
		PopulateObjectProperties( );
	}

	void ObjectPropertiesPanel::Close( )
	{
		mObjectPropertiesPanel->SetVisible( false );
		Reset( );
	}

	void ObjectPropertiesPanel::PopulateObjectProperties( )
	{
		if ( mFocusObject == nullptr )
			return;

		auto componentFirst = mFocusObject->Components.begin( );
		auto componentLast = mFocusObject->Components.end( );

		// loop through all the component in the type
		while ( componentFirst != componentLast )
		{
			const Reflection::MetaType & mt = META_HASH( componentFirst->second->mComponentType );
			ReadComponent( mt, componentFirst->second );

			++componentFirst;
		}
		std::cout << std::endl;
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
		// member.GetType( ).GetName( );
		// mObjectPropertiesPanel->AddField( member.GetName( ), member.GetPtr<float>( component ), metatype.GetName( ) );

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

}
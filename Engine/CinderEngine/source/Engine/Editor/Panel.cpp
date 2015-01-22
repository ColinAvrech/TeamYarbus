#include "Precompiled.h"

#include "Panel.h"
#include "PanelManager.h"

#include <string>

namespace Panel
{

	//------------------------------------------------------------------------------------------
	// constructors / destructor
	//------------------------------------------------------------------------------------------

	Panel::Panel( const std::string & name ) :
		mName( name ), mPtr( TwNewBar( name.c_str( ) ) )
	{
		Initialize( );
	}

	Panel::~Panel( )
	{
		Shutdown( );
		ErrorCheck( TwDeleteBar( mPtr ) );
	}

	//------------------------------------------------------------------------------------------
	// systems
	//------------------------------------------------------------------------------------------

	void Panel::Initialize( )
	{
		SetName( mName );
	}

	void Panel::Shutdown( )
	{

	}

	//------------------------------------------------------------------------------------------
	// names
	//------------------------------------------------------------------------------------------

	void Panel::SetName( const std::string & name )
	{
		ErrorCheck( TwSetParam( mPtr, nullptr, "label", TW_PARAM_CSTRING, 1, name.c_str( ) ) );
	}

	const std::string & Panel::GetName( ) const
	{
		return mName;
	}

	//------------------------------------------------------------------------------------------
	// panel properties
	//------------------------------------------------------------------------------------------

	void Panel::SetLock( bool state )
	{
		SetMoveable( !state );
		SetResizable( !state );
	}

	void Panel::SetVisible( bool state )
	{
		int value = state;
		ErrorCheck( TwSetParam( mPtr, nullptr, "visible", TW_PARAM_INT32, 1, &value ) );
	}

	void Panel::SetMinimize( bool state )
	{
		int value = state;
		ErrorCheck( TwSetParam( mPtr, nullptr, "iconified", TW_PARAM_INT32, 1, &value ) );
	}

	void Panel::SetIconifiable( bool state )
	{
		int value = state;
		ErrorCheck( TwSetParam( mPtr, nullptr, "iconifiable", TW_PARAM_INT32, 1, &value ) );
	}

	void Panel::SetMoveable( bool state )
	{
		int value = state;
		ErrorCheck( TwSetParam( mPtr, nullptr, "movable", TW_PARAM_INT32, 1, &value ) );
	}

	void Panel::SetResizable( bool state )
	{
		int value = state;
		ErrorCheck( TwSetParam( mPtr, nullptr, "resizable", TW_PARAM_INT32, 1, &value ) );
	}

	void Panel::SetPosition( float x, float y )
	{
		std::ostringstream oss;
		int xx = static_cast< int >( x );
		int yy = static_cast< int >( y );
		oss << xx << ' ' << yy;
		ErrorCheck( TwSetParam( mPtr, nullptr, "position", TW_PARAM_CSTRING, 1, oss.str( ).c_str( ) ) );
	}

	void Panel::SetSize( float x, float y )
	{
		std::ostringstream oss;
		int xx = static_cast< int >( x );
		int yy = static_cast< int >( y );
		oss << xx << ' ' << yy;
		ErrorCheck( TwSetParam( mPtr, nullptr, "size", TW_PARAM_CSTRING, 1, oss.str( ).c_str( ) ) );
	}

	void Panel::SetPanelColor( unsigned char r, unsigned char g, unsigned char b )
	{
		static int color[ 3 ] = { 0 };
		color[ 0 ] = r;
		color[ 1 ] = g;
		color[ 2 ] = b;
		ErrorCheck( TwSetParam( mPtr, nullptr, "color", TW_PARAM_INT32, 3, color ) );
	}

	void Panel::SetAlpha( unsigned int a )
	{
		ErrorCheck( TwSetParam( mPtr, nullptr, "alpha", TW_PARAM_INT32, 1, &a ) );
	}

	//------------------------------------------------------------------------------------------
	// clear panel fiels
	//------------------------------------------------------------------------------------------

	void Panel::ClearPanel( )
	{
		ErrorCheck( TwRemoveAllVars( mPtr ) );
	}

	void Panel::ClearField( const std::string & fieldName )
	{
		ErrorCheck( TwRemoveVar( mPtr, fieldName.c_str( ) ) );
	}

	//------------------------------------------------------------------------------------------
	// add panel fields
	//------------------------------------------------------------------------------------------

	void Panel::AddEnumField( const std::string & label,
							  void * value,
							  const std::string & enums,
							  const std::string & group,
							  bool readOnly )
	{
		std::ostringstream oss;
		if ( group.empty( ) == false )
			oss << " group='" << group << "' ";
		oss << " enum='" << enums << "' ";

		TwType & type = mEnumTypes[ std::hash<std::string >( )( label.c_str( ) ) ];
		type = TwDefineEnum( label.c_str( ), nullptr, 0 );

		if ( readOnly == false )
			ErrorCheck( TwAddVarRW( mPtr, label.c_str( ), type, value, oss.str( ).c_str( ) ) );
		else
			ErrorCheck( TwAddVarRO( mPtr, label.c_str( ), type, value, oss.str( ).c_str( ) ) );

	}

	void Panel::AddEnumFieldCB( const std::string & label,
								void * obj,
								const std::string & enums,
								GetCBType getCB,
								SetCBType setCB,
								const std::string & group )
	{
		std::ostringstream oss;
		if ( group.empty( ) )
			oss << " group='" << group << "' ";
		oss << " enum='" << enums << "' ";

		TwType & type = mEnumTypes[ std::hash<std::string >( )( label.c_str( ) ) ];
		type = TwDefineEnum( label.c_str( ), nullptr, 0 );

		ErrorCheck( TwAddVarCB( mPtr, label.c_str( ), type, setCB, getCB, obj, oss.str( ).c_str( ) ) );
	}

	void Panel::AddButton( const std::string & name,
						   ButtonCBType callback,
						   void * additionalData,
						   const std::string & group )
	{
		std::ostringstream oss;
		if ( group.empty( ) == false )
			oss << " group='" << group << "' ";
		ErrorCheck( TwAddButton( mPtr, name.c_str( ), callback, additionalData, oss.str( ).c_str( ) ) );
	}

	void Panel::AddLabel( const std::string & labelName,
						  const std::string & group )
	{
		std::ostringstream oss;
		if ( group.empty( ) == false )
			oss << " group='" << group << "' ";
		ErrorCheck( TwAddButton( mPtr, labelName.c_str( ), NULL, NULL, oss.str( ).c_str( ) ) );
	}

	void Panel::AddSeperator( const std::string & name,
							  const std::string & group )
	{
		if ( group.empty( ) )
		{
			std::ostringstream oss;
			oss << " group='" << group << "' ";
			ErrorCheck( TwAddSeparator( mPtr, name.c_str( ), oss.str( ).c_str( ) ) );
		}
		else
			ErrorCheck( TwAddSeparator( mPtr, name.c_str( ), nullptr ) );


	}


}
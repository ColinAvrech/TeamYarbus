#ifndef PANEL_H_
#define PANEL_H_

// AntTweakBar header //
#include "AntTweakBar.h"

#include "EnumList.h"
#include "Types.h"

#include <list>
#include <map>
#include <string>
#include <sstream>
#include <hash_set>
#include <hash_map>

#include <iostream>

namespace Panel
{
	class Panel
	{
	private:
		typedef int         Result;
		typedef TwBar*      PanelPtr;

	public:
		typedef void ( TW_CALL * GetCBType )( void *, void * );
		typedef void ( TW_CALL * SetCBType )( const void *, void * );
		typedef TwButtonCallback ButtonCBType;

	public:
		Panel( const std::string & name );
		~Panel( );

		void Initialize( );
		void Shutdown( );

		// names
		void SetName( const std::string & name );
		const std::string & GetName( ) const;

		// modify panel properties
		void SetLock( bool state );
		void SetVisible( bool state );
		void SetMinimize( bool state );
		void SetIconifiable( bool state );
		void SetMoveable( bool state );
		void SetResizable( bool state );
		void SetPosition( float x, float y );
		void SetSize( float x, float y );
		void SetPanelColor( unsigned char r, unsigned char g, unsigned char b );
		void SetAlpha( unsigned int a );

		// modify panel field
		void ClearPanel( );
		void ClearField( const std::string & fieldName );

		// add panel fields
		template <typename T>
		void AddField( const std::string & label,
					   T * value,
					   const std::string & group = std::string( ),
					   bool readOnly = false );

		template <>
		void AddField( const std::string & label,
					   Vec2 * value,
					   const std::string & group,
					   bool readOnly );

		template <>
		void AddField( const std::string & label,
					   Vec4 * value,
					   const std::string & group,
					   bool readOnly );

		template <typename IteratorType>
		void AddContainer( const std::string & label,
						   IteratorType first,
						   IteratorType last,
						   const std::string & group = std::string( ),
						   bool readOnly = false );

		template <typename T>
		void AddFieldCB( const std::string & label,
						 void * obj,
						 GetCBType getCB,
						 SetCBType setCB,
						 const std::string & group = std::string( ) );

		void AddEnumField( const std::string & label,
						   void * value,
						   const std::string & enums,
						   const std::string & group = std::string( ),
						   bool readOnly = false );

		void AddEnumFieldCB( const std::string & label,
							 void * obj,
							 const std::string & enums,
							 GetCBType getCB,
							 SetCBType setCB,
							 const std::string & group = std::string( ) );

		void AddButton( const std::string & name,
						ButtonCBType callback,
						void * additionalData,
						const std::string & group = std::string( ) );

		void AddLabel( const std::string & labelName,
					   const std::string & group = std::string( ) );

		void AddSeperator( const std::string & name = std::string( ),
						   const std::string & group = std::string( ) );

	private:
		std::string mName;
		std::map<std::hash<std::string>::result_type, TwType> mEnumTypes;
		PanelPtr mPtr = nullptr;
	};

#define CBSTRCPY(className, name, value, data)\
  TwCopyStdStringToLibrary( *reinterpret_cast< std::string * >( value ), \
  reinterpret_cast< className * >( data )->name );

}

#include "Panel.inl"

#endif
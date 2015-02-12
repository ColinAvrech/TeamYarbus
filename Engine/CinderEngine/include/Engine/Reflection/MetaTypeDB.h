#ifndef METATYPEDB_H_
#define METATYPEDB_H_

#include "MetaCommon.h"
#include "MetaType.h"
#include "RemQual.h"

#include <unordered_map>

namespace Reflection
{

	class MetaTypeDB
	{
	private:

		typedef std::unordered_map< HashedNameType, MetaType > MetaTypeContainer;
		typedef std::vector<const MetaType *> MetaTagResult;

	public:

		MetaTypeDB( );
		~MetaTypeDB( );

		template <typename T>
		MetaType &			CreateType( );

		template <typename T>
		MetaType &			CreateBaseType( );

		template <typename T>
		const MetaType &	GetType( ) const;

		const MetaType &	GetType( const NameType & typeName ) const;
		const MetaType &	GetType( HashedNameType typeName ) const;

		MetaTagResult		GetTypeByTag( const NameType & tag ) const;

		static MetaTypeDB &	GetDB( );

	private:
		
		inline bool IsTypeExist( HashedNameType name ) const;

		MetaTypeContainer	mTypes;

	};


}

#define META_TYPE( type ) \
	Reflection::MetaTypeDB::GetDB( ).GetType< Reflection::RemQual< type >::Type >( )

#define META_OBJ( obj ) \
	Reflection::MetaTypeDB::GetDB( ).GetType< Reflection::RemQual< decltype( obj ) >::Type >( ) 

#define META_STR( str ) \
	Reflection::MetaTypeDB::GetDB( ).GetType( str )

#define META_HASH( hash ) \
	Reflection::MetaTypeDB::GetDB( ).GetType( hash )

#define META_TAG( tag ) \
	Reflection::MetaTypeDB::GetDB( ).GetTypeByTag( tag )

#include "MetaTypeDB.inl"

#endif
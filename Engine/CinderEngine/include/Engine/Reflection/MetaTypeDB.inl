#include "MetaCommon.h"
#include "MetaTypeRegistar.h"
#include "MetaTypeDB.h"
#include "MetaType.h"

namespace Reflection
{
	template <typename T>
	MetaType & MetaTypeDB::CreateType( )
	{
		HashedNameType hash = MetaTypeToHash<T>( );
		MetaAssert( IsTypeExist( hash ) == false );
		mTypes.insert( { hash, MetaType( MetaTypeToName<T>( ), sizeof( T ) ) } );
		mTypes.find( hash )->second.mIsBase = false;
		return mTypes.find( hash )->second;
	}

	template <typename T>
	MetaType & MetaTypeDB::CreateBaseType( )
	{
		HashedNameType hash = MetaTypeToHash<T>( );
		MetaAssert( IsTypeExist( hash ) == false );
		mTypes.insert( { hash, MetaType( MetaTypeToName<T>( ), sizeof( T ) ) } );
		return mTypes.find( hash )->second;
	}

	template <typename T>
	const MetaType & MetaTypeDB::GetType( ) const
	{
		HashedNameType hash = MetaTypeToHash<T>( );
		MetaAssert( IsTypeExist( hash ) == true );
		return mTypes.find( hash )->second;
	}

	inline bool MetaTypeDB::IsTypeExist( HashedNameType name ) const
	{
		return mTypes.find( name ) != mTypes.end( );
	}
}
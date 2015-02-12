#include "Precompiled.h"

#include "MetaCommon.h"
#include "MetaTypeRegistar.h"
#include "MetaTypeDB.h"
#include "MetaType.h"

namespace Reflection
{

	MetaTypeDB::MetaTypeDB( )
	{

	}

	MetaTypeDB::~MetaTypeDB( )
	{

	}



	const MetaType & MetaTypeDB::GetType( const NameType & typeName ) const
	{
		return GetType( GenerateHash( typeName ) );
	}
	
	const MetaType & MetaTypeDB::GetType( HashedNameType typeName ) const
	{
		MetaAssert( IsTypeExist( typeName ) == true );
		return mTypes.find( typeName )->second;
	}

	MetaTypeDB::MetaTagResult MetaTypeDB::GetTypeByTag( const NameType & tag ) const
	{
		MetaTagResult list;
		
		for ( const auto & type : mTypes )
		{
			if ( type.second.TagExist( tag ) == true )
				list.emplace_back( &type.second );
		}
		
		return list;
	}

	MetaTypeDB & MetaTypeDB::GetDB( )
	{
		static MetaTypeDB instance;
		return instance;
	}

}
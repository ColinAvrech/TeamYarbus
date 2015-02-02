#ifndef METACONFIG_H_
#define METACONFIG_H_

#include "Hash.h"
#include <string>
#include <assert.h>

namespace Reflection
{
	// name types
	typedef std::string			NameType;
	typedef Hash::HashValue		HashedNameType;

	// size types
	typedef size_t				SizeType;
	typedef size_t				OffsetType;
	
	// hash generators
	inline HashedNameType		GenerateHash( const std::string & name )	{ return Hash::Hash::Generate( name ); }
	inline HashedNameType		GenerateHash( const char * name )			{ return GenerateHash( std::string( name ) ); }

	// asserts
	inline void					MetaAssert( bool result, const char * msg = nullptr )	{ Assert( result ); }
	inline void					MetaAssert( bool result, const std::string & msg )		{ Assert( result ); }
		
}

#endif
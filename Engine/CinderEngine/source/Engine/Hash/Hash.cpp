// Precompiled Headers
#include "Precompiled.h"

#include "Hash.h"
#include <string>

namespace Hash
{
	Hash::~Hash( )
	{
	}

	Hash& Hash::operator = ( const char * name )
	{
		mText = name ? name : "";
		mHash = name ? Generate( mText ) : 0;
		return *this;
	}

	Hash & Hash::operator = ( const std::string & name )
	{
		mText = name;
		mHash = Generate( mText );
		return *this;
	}
}
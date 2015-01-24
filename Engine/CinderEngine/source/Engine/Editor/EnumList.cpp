#include "Precompiled.h"

#include "EnumList.h"

#include <string>

namespace Panel
{

	std::string EnumList::GetList( )
	{
		std::string s;

		// Sort( );

		for ( const auto & it : mEnumList )
		{
			s += it.mValue + " {" + it.mName + "}, ";
		}

		if ( s.empty( ) != true )
		{
			s.pop_back( );
			s.pop_back( );
		}

		return s;
	}

	void EnumList::PushBack( const EnumEntry & s )
	{
		mEnumList.emplace_back( s );
	}

	void EnumList::Clear( )
	{
		mEnumList.clear( );
	}

	bool EnumList::IsEmpty( ) const
	{
		return mEnumList.empty( );
	}

	EnumList::EnumEntry::EnumEntry( const std::string & name, unsigned int value )
	{
		mName = name;
		mValue = std::to_string( value );
	}

}
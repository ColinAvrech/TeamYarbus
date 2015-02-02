#include "Precompiled.h"

#include "MetaType.h"

namespace Reflection
{

	MetaType::MetaType( const std::string & typeName, size_t typeSize ) :
		mTypeName( typeName ), 
		mTypeHash( GenerateHash( typeName ) ),
		mTypeSize( typeSize )
	{

	}

	MetaType::~MetaType( )
	{

	}

	const std::string & MetaType::GetName( ) const
	{
		return mTypeName;
	}

	HashedNameType MetaType::GetHashedName( ) const
	{
		return mTypeHash;
	}

	size_t MetaType::GetSize() const
	{
		return mTypeSize;
	}

	bool MetaType::IsBase( ) const
	{
		return mIsBase;
	}

	const MetaMember & MetaType::GetMember( const NameType & name ) const
	{
		return GetMember( GenerateHash( name ) );
	}

	const MetaMember & MetaType::GetMember( HashedNameType name ) const
	{
		const MetaMember * mem = IsMemberExist( name );
		MetaAssert( mem != nullptr, "Member does not exist" );
		return *mem;
	}

	const MetaType::MetaMemberContainer & MetaType::GetMembers( ) const
	{
		return mMembers;
	}

	MetaType::MetaMemberIterator MetaType::Begin( ) const
	{
		return mMembers.cbegin( );
	}

	MetaType::MetaMemberIterator MetaType::End( ) const
	{
		return mMembers.cend( );
	}

	const MetaMember * MetaType::IsMemberExist( const NameType & name ) const
	{
		return IsMemberExist( GenerateHash( name ) );
	}

	const MetaMember * MetaType::IsMemberExist( HashedNameType name ) const
	{
		for ( const auto & mem : mMembers )
		{
			if ( mem.GetHashedName( ) == name )
				return & mem;
		}

		return nullptr;
	}

}
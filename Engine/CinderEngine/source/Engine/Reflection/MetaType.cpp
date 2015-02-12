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

	bool MetaType::TagExist( const NameType & tag ) const
	{
		return std::find( mTags.begin( ), mTags.end( ), tag ) != mTags.end( );
	}

	void MetaType::AddTag( const NameType & tag )
	{
		mTags.emplace_back( tag );
	}

	void MetaType::RemoveTag( const NameType & tag )
	{
		auto it = std::find( mTags.begin( ), mTags.end( ), tag );
		MetaAssert( it != mTags.end( ) );
		mTags.erase( it );
	}

	void MetaType::ReplaceTag( const NameType & prev, const NameType & curr )
	{
		auto it = std::find( mTags.begin( ), mTags.end( ), prev );
		MetaAssert( it != mTags.end( ) );
		*it = curr;
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
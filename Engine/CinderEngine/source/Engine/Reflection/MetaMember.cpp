#include "Precompiled.h"

#include "MetaMember.h"
#include "MetaTypeDB.h"

namespace Reflection
{
	MetaMember::~MetaMember( )
	{

	}

	const NameType & MetaMember::GetName( ) const
	{
		return mName;
	}

	HashedNameType MetaMember::GetHashedName( ) const
	{
		return mType;
	}

	const MetaType & MetaMember::GetType( ) const
	{
		return MetaTypeDB::GetDB().GetType( mType );
	}

	OffsetType MetaMember::GetOffset( ) const
	{
		return mOffset;
	}

	const std::string & MetaMember::GetDescription( ) const
	{
		return mDescription;
	}

	bool MetaMember::IsPointer( ) const
	{
		return mIsPointer;
	}

	bool MetaMember::IsContainer( ) const
	{
		return mIsContainer;
	}

}

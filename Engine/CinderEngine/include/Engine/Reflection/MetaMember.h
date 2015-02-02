#ifndef METAMEMBER_H_
#define METAMEMBER_H_

#include "MetaCommon.h"

#include <string>

namespace Reflection
{

	// holds infomation about a member of a type

	class MetaType;

	class MetaMember
	{
	public:
		template <typename MemberType, typename ParentType >
		MetaMember( const NameType & memberName,
					HashedNameType memberType,
					MemberType ParentType::*member,
					const char * description = nullptr );

		~MetaMember( );

		const NameType &	GetName( ) const;
		HashedNameType		GetHashedName( ) const;
		const MetaType &	GetType( ) const;
		OffsetType			GetOffset( ) const;
		const std::string & GetDescription( ) const;

		inline void *		GetPtr( void * obj ) const;
		inline const void * GetPtr( const void * obj ) const;

		template <typename T>
		T *					GetPtr( void * obj ) const;

		template <typename T>
		const T *			GetPtr( const void * obj ) const;

	private:

		NameType			mName;

		HashedNameType		mType;
		OffsetType			mOffset;

		bool				mIsPointer;

		std::string			mDescription;
	};

}

#include "MetaMember.inl"

#endif
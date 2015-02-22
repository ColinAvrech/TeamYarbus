#ifndef METATYPE_H_
#define METATYPE_H_

#include "MetaCommon.h"
#include "MetaMember.h"

#include <string>
#include <vector>

namespace Reflection
{

	// holds infomation about a type

	class MetaType
	{
	public:
		typedef std::vector< MetaMember >			MetaMemberContainer;
		typedef MetaMemberContainer::const_iterator MetaMemberIterator;

		typedef std::vector< NameType >				MetaTagContainer;

	public:
		~MetaType( );

		template <typename MemberType, typename ParentType>
		MetaType &					AddMember( const NameType & name, MemberType ParentType::*member );

		template <typename MemberType, typename ParentType, template<class T, class = std::allocator<T>> class ContainerType>
		MetaType &					AddMember( const NameType & name, ContainerType<MemberType> ParentType::*member );

		const NameType &			GetName( ) const;
		HashedNameType				GetHashedName( ) const;
		SizeType					GetSize( ) const;
		bool						IsBase( ) const;

		const MetaMember &			GetMember( const NameType & name ) const;
		const MetaMember &			GetMember( HashedNameType name ) const;
		const MetaMemberContainer &	GetMembers( ) const;

		bool						TagExist( const NameType & tag ) const;
		void						AddTag( const NameType & tag );
		void						RemoveTag( const NameType & tag );
		void						ReplaceTag( const NameType & prev, const NameType & curr );

		MetaMemberIterator			Begin( ) const;
		MetaMemberIterator			End( ) const;

	private:
		friend class MetaTypeDB;
		friend class MetaMember;

		MetaType( const NameType & typeName, SizeType typeSize );

		const MetaMember *			IsMemberExist( const NameType & name ) const;
		const MetaMember *			IsMemberExist( HashedNameType name ) const;

	private:
		NameType					mTypeName;
		HashedNameType				mTypeHash;
		SizeType					mTypeSize;
		bool						mIsBase = true;

		MetaMemberContainer			mMembers;
		MetaTagContainer			mTags;
	};

}

#include "MetaType.inl"

#endif
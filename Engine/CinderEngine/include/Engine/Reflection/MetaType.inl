#include "RemQual.h"

namespace Reflection
{

	template <typename MemberType, typename ParentType>
	MetaType & MetaType::AddMember( const NameType & name, MemberType ParentType::*member )
	{
		mMembers.emplace_back( name, member );
		return *this;
	}

	template <typename MemberType, typename ParentType, template<class T, class = std::allocator<T>> class ContainerType>
	MetaType & MetaType::AddMember( const NameType & name, ContainerType<MemberType> ParentType::*member )
	{
		mMembers.emplace_back( name, member );
		return *this;
	}

}
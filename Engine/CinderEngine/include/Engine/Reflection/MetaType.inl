#include "RemQual.h"

namespace Reflection
{

	template <typename MemberType, typename ParentType>
	MetaType & MetaType::AddMember( const NameType & name, MemberType ParentType::*member )
	{
		mMembers.emplace_back( name, Reflection::MetaTypeToHash< std::remove_pointer< RemQual< MemberType >::Type >::type >( ), member );
		return *this;
	}


}
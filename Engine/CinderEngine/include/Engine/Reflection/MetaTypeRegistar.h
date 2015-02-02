#ifndef METATYPEREGISTAR_H_
#define METATYPEREGISTAR_H_

#include "MetaCommon.h"

namespace Reflection
{

	template <typename T>
	inline const NameType & MetaTypeToName()
	{
		static const NameType mName = "Undefined";
		return mName;
	}

	template <typename T>
	inline const HashedNameType & MetaTypeToHash()
	{
		static const HashedNameType mHashed = GenerateHash( MetaTypeToName<T>() );
		return mHashed;
	};

}

#define MetaDeclareType( type, typeName )	\
	namespace Reflection {		\
	template <> inline const NameType & MetaTypeToName <type> () { static const NameType mName = #typeName; return mName; }; }


#endif
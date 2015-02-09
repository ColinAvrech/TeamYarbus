#ifndef METACREATOR_H_
#define METACREATOR_H_

#include "MetaCommon.h"
#include "MetaTypeRegistar.h"
#include "MetaTypeDB.h"
#include "RemQual.h"

#include "MathExtensionLibrary.h"

namespace Reflection
{

	template <typename T>
	struct MetaCreator
	{
		typedef T Type;

		static void * RegisterBase( )
		{
			mMetaType = &MetaTypeDB::GetDB( ).CreateBaseType < T >( );
			return nullptr;
		}

		static void * RegisterType( )
		{
			mMetaType = &MetaTypeDB::GetDB( ).CreateType < T >( );
			T::RegisterMeta( );
			return nullptr;
		}

		static MetaType * mMetaType;
	};

	template <typename T>
	MetaType * MetaCreator<T>::mMetaType = nullptr;
}

#define PASTE( _, __ )  _##__
#define GENERATE_LINE( _ ) PASTE( GENERATED_TOKEN_, _ )
#define GENERATE_FILE( _ ) PASTE( __FILE__, _ )
#define NAME_GENERATOR( ) GENERATE_FILE( __LINE__ )

#define META_DECLARE( type )	\
	typedef type _MetaType;		\
	static void RegisterMeta( )

#define META_DEFINE_BASE( type, typeName )\
	MetaDeclareType( type, typeName );	\
	namespace { void * NAME_GENERATOR( ) =:: Reflection::MetaCreator< ::Reflection::RemQual< type >::Type >::RegisterBase();  }

#define META_DEFINE( type, typeName )		\
	MetaDeclareType( type, typeName );	\
	namespace { void * NAME_GENERATOR( ) = ::Reflection::MetaCreator< ::Reflection::RemQual< type >::Type >::RegisterType();  } \
	void type::RegisterMeta( )

#define MEMBER( member ) \
	::Reflection::MetaCreator< ::Reflection::RemQual< _MetaType >::Type >::mMetaType->AddMember( #member, &_MetaType::member ) 

META_DEFINE_BASE( bool, bool );
META_DEFINE_BASE( char, char );
META_DEFINE_BASE( unsigned char, unsigned char );
META_DEFINE_BASE( short, short );
META_DEFINE_BASE( unsigned short, unsigned short );
META_DEFINE_BASE( int, int );
META_DEFINE_BASE( unsigned int, unsigned int );
META_DEFINE_BASE( float, float );
META_DEFINE_BASE( double, double );

META_DEFINE_BASE( glm::vec2, vec2 );
META_DEFINE_BASE( glm::vec3, vec3 );
META_DEFINE_BASE( glm::vec4, vec4 );
META_DEFINE_BASE( std::string, string )

#endif
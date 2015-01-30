#ifndef METACREATOR_H_
#define METACREATOR_H_

#include "MetaCommon.h"
#include "MetaTypeRegistar.h"
#include "MetaTypeDB.h"
#include "RemQual.h"

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
	typedef type Type;			\
	static void RegisterMeta( )

#define META_DEFINE_BASE( type )\
	MetaDeclareType( type );	\
	namespace { void * NAME_GENERATOR( ) = Reflection::MetaCreator< Reflection::RemQual< type >::Type >::RegisterBase();  }

#define META_DEFINE( type )		\
	MetaDeclareType( type );	\
	namespace { void * NAME_GENERATOR( ) = Reflection::MetaCreator< Reflection::RemQual< type >::Type >::RegisterType();  } \
	void type::RegisterMeta( )

#define MEMBER( member ) \
	Reflection::MetaCreator< Reflection::RemQual< Type >::Type >::mMetaType->AddMember( #member, &Type::member ) 

META_DEFINE_BASE( bool );
META_DEFINE_BASE( char );
META_DEFINE_BASE( unsigned char );
META_DEFINE_BASE( short );
META_DEFINE_BASE( unsigned short );
META_DEFINE_BASE( int );
META_DEFINE_BASE( unsigned int );
META_DEFINE_BASE( float );
META_DEFINE_BASE( double );

#endif
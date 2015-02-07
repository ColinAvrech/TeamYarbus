#ifndef PANELTYPES_H_
#define PANELTYPES_H_

#include "AntTweakBar.h"
#include "Hash.h"

#include "MathExtensionLibrary.h"

namespace Panel
{
	class PanelTypeDB
	{
	private:
		typedef std::unordered_map< unsigned int, TwType > PanelTypeContainer;

	public:
		PanelTypeDB( ) { }
		~PanelTypeDB( ) { }

		void *	CreateType( unsigned int hash, TwType twtype )
		{
			mTypes[ hash ] = twtype;
			return nullptr;
		}

		TwType	GetTwType( unsigned int hash ) const
		{
			assert( mTypes.find( hash ) != mTypes.end( ) );
			return mTypes.find( hash )->second;
		}

		static PanelTypeDB & GetDB( )
		{
			static PanelTypeDB instance;
			return instance;
		}

	private:
		PanelTypeContainer mTypes;

	};

	template <typename T>
	struct PanelType
	{
		typedef T Type;

		static const TwType mType = TW_TYPE_UNDEF;
	};

#define HASH_TO_PANEL_TYPE( hash ) \
	::Panel::PanelTypeDB::GetDB().GetTwType( hash )

#define PASTE( _, __ )  _##__
#define GENERATE_LINE( _ ) PASTE( GENERATED_TOKEN_, _ )
#define GENERATE_FILE( _ ) PASTE( __FILE__, _ )
#define NAME_GENERATOR( ) GENERATE_FILE( __LINE__ )

#define PanelDefineType( type, typeName, twType ) \
	template <> struct PanelType <type> { static const TwType mType = twType; }; \
	void * NAME_GENERATOR( ) = PanelTypeDB::GetDB().CreateType( Hash::Hash::Generate(#typeName), twType )


	struct vec2;
	struct vec3;
	struct vec4;
	struct quad;

	// change int to your vector type
	typedef glm::vec2 Vec2;
	typedef glm::vec3 Vec3;
	typedef glm::vec4 Vec4;
	typedef glm::quat Quad;

	PanelDefineType( bool, bool, TW_TYPE_BOOLCPP );
	PanelDefineType( char, char, TW_TYPE_INT8 );
	PanelDefineType( short, short, TW_TYPE_INT16 );
	PanelDefineType( int, int, TW_TYPE_INT32 );
	PanelDefineType( unsigned char, unsigned char, TW_TYPE_UINT8 );
	PanelDefineType( unsigned short, unsigned short, TW_TYPE_UINT16 );
	PanelDefineType( unsigned int, unsigned int, TW_TYPE_UINT32 );
	PanelDefineType( float, float, TW_TYPE_FLOAT );
	PanelDefineType( double, double, TW_TYPE_DOUBLE );
	PanelDefineType( std::string, string, TW_TYPE_STDSTRING );

	PanelDefineType( vec2, vec2, TW_TYPE_DIR2F );
	PanelDefineType( glm::vec3, vec3, TW_TYPE_DIR3F );
	PanelDefineType( glm::vec4, vec4, TW_TYPE_DIR4F );
	PanelDefineType( glm::quat, quat, TW_TYPE_QUAT4F );

}

#endif
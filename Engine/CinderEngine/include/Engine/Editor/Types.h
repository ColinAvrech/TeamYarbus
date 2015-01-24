#ifndef PANELTYPES_H_
#define PANELTYPES_H_

#include "AntTweakBar.h"

namespace Panel
{

	template <typename T>
	struct PanelType
	{
		static const TwType mType = TW_TYPE_UNDEF;
	};

#define PanelDefineType( type, twType ) \
	template <> struct PanelType <type> { static const TwType mType = twType; }

	struct vec2;
	struct vec3;
	struct vec4;
	struct quad;

	// change int to your vector type
	typedef glm::vec2 Vec2;
	typedef glm::vec3 Vec3;
	typedef glm::vec4 Vec4;
	typedef glm::quat Quad;

	PanelDefineType( bool, TW_TYPE_BOOLCPP );
	PanelDefineType( char, TW_TYPE_INT8 );
	PanelDefineType( short, TW_TYPE_INT16 );
	PanelDefineType( int, TW_TYPE_INT32 );
	PanelDefineType( unsigned char, TW_TYPE_UINT8 );
	PanelDefineType( unsigned short, TW_TYPE_UINT16 );
	PanelDefineType( unsigned int, TW_TYPE_UINT32 );
	PanelDefineType( float, TW_TYPE_FLOAT );
	PanelDefineType( double, TW_TYPE_DOUBLE );
	PanelDefineType( std::string, TW_TYPE_STDSTRING );

	PanelDefineType( Vec3, TW_TYPE_DIR3F );
	PanelDefineType( Quad, TW_TYPE_QUAT4F );

}

#endif
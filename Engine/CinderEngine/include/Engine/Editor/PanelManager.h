#ifndef PANELMANAGER_H_
#define PANELMANAGER_H_

#include "Panel.h"

#include <list>

#if 0


Panel::PanelManager			* panels = new Panel::PanelManager( Panel::PanelManager::GraphicAPI::OPENGL, ClientWidth, ClientHeight );

Panel::PanelManager::PanelHandle h = panels->AddPanel( "TestPanel" );
h->AddLabel( "it's woring!" );

bool testBool = 1;
char testChar = 2;
short testShort = 3;
int testInt = 4;
unsigned char testUChar = 5;
unsigned short testUShort = 6;
unsigned int testUInt = 7;
float testFloat = 8;
double testDouble = 9;
std::string testString = "HelloWorld";

h->AddField( "testBool", &testBool, "1" );
h->AddField( "testChar", &testChar, "1", true );
h->AddField( "testShort", &testShort, "1" );
h->AddField( "testInt", &testInt, "1", true );
h->AddField( "testUChar", &testUChar, "1" );
h->AddField( "testUShort", &testUShort, "1", true );
h->AddField( "testUInt", &testUInt, "1" );
h->AddField( "testFloat", &testFloat, "2", true );
h->AddField( "testDouble", &testDouble, "2" );
h->AddField( "testString", &testString, "2", true );

glm::vec2 testVec2( 1, 2 );
glm::vec3 testVec3( 1, 2, 3 );
glm::vec4 testVec4( 1, 2, 3, 4 );
glm::quat testQuad( 1, 2, 3, 4 );

h->AddField( "testVec2", &testVec2, "3", true );
h->AddField( "testVec3", &testVec3, "3" );
h->AddField( "testVec4", &testVec4, "3" );
h->AddField( "testQuad", &testQuad, "3", true );

std::list< float > testListContainer;
testListContainer.push_back( 1.0f );
testListContainer.push_back( 2.0f );
testListContainer.push_back( 3.0f );
testListContainer.push_back( 4.0f );
testListContainer.push_back( 5.0f );
h->AddContainer( "testListContainer", testListContainer.begin( ), testListContainer.end( ), "4" );

std::vector< float > testVectorContainer;
testVectorContainer.push_back( 1.0f );
testVectorContainer.push_back( 2.0f );
testVectorContainer.push_back( 3.0f );
testVectorContainer.push_back( 4.0f );
testVectorContainer.push_back( 5.0f );
h->AddContainer( "testVectorContainer", testVectorContainer.begin( ), testVectorContainer.end( ), "5" );

delete panels;

#endif

namespace Panel
{

	template <typename T>
	void ErrorCheck( T r, const std::string & additionMsg = std::string( ) )
	{
		if ( r == 0 )
		{
			std::string msg = TwGetLastError( );

			if ( additionMsg.empty( ) == false )
				msg += std::string( "\n" ) + additionMsg;

			__debugbreak( );
		}
	}


	class PanelManager
	{
	private:
		friend class Panel;
		typedef int Result;
		typedef std::list< Panel > PanelContainer;

	public:
		enum class GraphicAPI
		{
			OPENGL = TW_OPENGL,
			OPENGLCORE = TW_OPENGL_CORE,
			DIRECTX9 = TW_DIRECT3D9,
			DIRECTX10 = TW_DIRECT3D10,
			DIRECTX11 = TW_DIRECT3D11
		};
		typedef PanelContainer::iterator PanelHandle;

	public:
		PanelManager( GraphicAPI graphics, unsigned int windowWidth, unsigned int windowHeight );
		~PanelManager( );

		static void CheckEvent( void * wnd, unsigned int msg, unsigned int wParam, int lParam );
		static void Draw( );
		void SetWindowSize( unsigned int width, unsigned int height );

		PanelHandle AddPanel( const std::string & panelName );
		PanelHandle GetPanel( const std::string & panelName );

		void RemovePanel( const std::string & panelName );
		void RemovePanel( PanelHandle panel );

		void RemoveAllPanels( );

		void SetLock( bool state );
		void SetVisible( bool state );
		void SetAlpha( unsigned int alpha );
		void SetPanelColor( unsigned char r, unsigned char g, unsigned char b );

		void Define( const std::string & attribute, const std::string & value );

	private:
		void InitATB( GraphicAPI graphics );
		void DeinitATB( );

		

		PanelContainer mPanels;
	};

	

}

namespace Framework
{
	extern Panel::PanelManager * PANELMANAGER;
}

#endif
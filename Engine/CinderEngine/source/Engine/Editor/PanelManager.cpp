#include "Precompiled.h"

#include "PanelManager.h"

namespace Framework
{
	Panel::PanelManager * PANELMANAGER = nullptr;
}

namespace Panel
{

	PanelManager::PanelManager( GraphicAPI graphics, unsigned int windowWidth, unsigned int windowHeight )
	{
		if ( Framework::PANELMANAGER != nullptr )
			__debugbreak( );

		Framework::PANELMANAGER = this;

		InitATB( graphics );
		SetWindowSize( windowWidth, windowHeight );
	}

	PanelManager::~PanelManager( )
	{
		RemoveAllPanels( );
		DeinitATB( );
	}

	void PanelManager::CheckEvent( void * wnd, unsigned int msg, unsigned int wParam, int lParam )
	{
		TwEventWin( wnd, msg, wParam, lParam );
	}

	void PanelManager::Draw( )
	{
		TwDraw( );
	}

	PanelManager::PanelHandle PanelManager::AddPanel( const std::string & panelName )
	{
		mPanels.emplace_front( panelName );
		return mPanels.begin( );
	}

	PanelManager::PanelHandle PanelManager::GetPanel( const std::string & panelName )
	{
		auto first = mPanels.begin( );
		auto last = mPanels.end( );

		while ( first != last )
		{
			if ( first->GetName( ) == panelName )
				break;

			++first;
		}

		return first;
	}

	void PanelManager::RemovePanel( const std::string & panelName )
	{
		auto first = mPanels.begin( );
		auto last = mPanels.end( );

		while ( first != last )
		{
			if ( first->GetName( ) == panelName )
			{
				first->Shutdown( );
				mPanels.erase( first );
				return;
			}

			++first;
		}
	}

	void PanelManager::RemovePanel( PanelHandle panel )
	{
		panel->Shutdown( );
		mPanels.erase( panel );
	}

	void PanelManager::RemoveAllPanels( )
	{
		auto first = mPanels.begin( );
		auto last = mPanels.end( );

		while ( first != last )
		{
			first->Shutdown( );
			++first;
		}

		mPanels.clear( );
	}

	void PanelManager::SetLock( bool state )
	{
		for ( auto & panel : mPanels )
			panel.SetLock( state );
	}

	void PanelManager::SetVisible( bool state )
	{
		for ( auto & panel : mPanels )
			panel.SetVisible( state );
	}

	void PanelManager::SetAlpha( unsigned int alpha )
	{
		for ( auto & panel : mPanels )
			panel.SetAlpha( alpha );
	}

	void PanelManager::SetPanelColor( unsigned char r, unsigned char g, unsigned char b )
	{
		for ( auto & panel : mPanels )
			panel.SetPanelColor( r, g, b );
	}

	void PanelManager::Define( const std::string & attribute, const std::string & value )
	{
		ErrorCheck( TwDefine( ( std::string( " GLOBAL " ) + attribute + "=" + value + " " ).c_str( ) ) );
	}

	void PanelManager::InitATB( GraphicAPI graphics )
	{
		ErrorCheck( TwInit( static_cast< ETwGraphAPI >( graphics ), NULL ) );
		TwCopyStdStringToClientFunc( [] ( std::string & des, const std::string & source ) { des = source; } );
	}

	void PanelManager::DeinitATB( )
	{
		ErrorCheck( TwTerminate( ) );
	}

	void PanelManager::SetWindowSize( unsigned int width, unsigned int height )
	{
		ErrorCheck( TwWindowSize( width, height ) );
	}
}
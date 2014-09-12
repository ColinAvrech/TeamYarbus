/******************************************************************************/
/*!
\file   WinMain.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  WinMain.cpp is the starting place for the engine. This initializes all the systems and
starts the game loop.
*/
/******************************************************************************/


#define WINDOWSBUILD
#ifdef WINDOWSBUILD

#include "WindowSystem.h"
#include "EventSystem.h"
#include "GraphicsSystem.h"
#include "AudioSystem.h"
#include "ObjectSystem.h"
#include "Common.h"     //! EnableMemoryleakChecking
#include "Core.h"

#define _DEGUB
using namespace Framework;

//! Window title
const char WindowTitle[] = "CinderEngine";
const int ClientWidth = 1024;
const int ClientHeight = 768;

/*!
@brief WinMain is the begining point for windows aplications.
@param hInst handle to the application instance.
@param hPrevInstance previous app instacne which you can use to prevent being launced multiple times.
@param LPSTR Command line string, We will use GetCommandLine() instead.
@param INT the way the application's window is displayed.
*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR, INT)
{
  EnableMemoryLeakChecking();

  // TODO (EXTRA): make a window to show while the game is loading
  Console::Create_Cinder_Console("CinderEngineConsole");
  // TODO Make console accept input by pressing '`', if '`' is pressed again return to game

  /*! Initialize the game engine*/
  
  //! Create the core engine which manages all systems.
  CoreEngine * engine         = new CoreEngine;
  WindowSystem * windows      = new WindowSystem (hInst, WindowTitle, ClientWidth, ClientHeight);
  ObjectSystem * objects      = new ObjectSystem();
  GraphicsSystem* graphics    = new GraphicsSystem ();
  AudioSystem* audio          = new AudioSystem();
  EventSystem * events        = new EventSystem ();

  engine->AddSystem(objects);
  engine->AddSystem(windows);
  engine->AddSystem(audio);
  engine->AddSystem(events);
  engine->AddSystem(graphics);


  //! Initialize all added Systems. DON'T INIT YOUR OWN
  engine->Initialize();
  audio->LoadAllSounds();

  windows->ActivateWindow();
  //! activate the window.

  //! Run the game! NOW!
  engine->GameLoop();

  //! Delete all systems
  engine->DestroySystems();

  //! Delete engine
  delete engine;

  //! Free console
  Console::Free_Cinder_Console();

  return 0;
}


#endif
/*!

/auther Micah
/file main.cpp

/brief main.cpp is the starting place for the engine. This initializes all the systems and
starts the game loop.

*/

#define WINDOWSBUILD
#ifdef WINDOWSBUILD

#include "WindowSystem.h"
#include "EventSystem.h"
#include "CinderDebug.h"        //! EnableMemoryleakChecking
#include "Core.h"
#include "Console.h"

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
  CoreEngine * engine = new CoreEngine;
  //! Initilize all systems
  WindowSystem * windows = new WindowSystem(hInst, WindowTitle, ClientWidth, ClientHeight);
  //EventSystem * events = new EventSystem();


  engine->AddSystem(windows);
  //engine->AddSystem(events);

  
  //! activate the window
  windows->ActivateWindow();

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
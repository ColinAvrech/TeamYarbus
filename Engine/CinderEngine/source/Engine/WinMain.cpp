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

#include "Common.h"
#include "WinMain.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "SceneManager.h"
#include "AudioSystem.h"
#include "ZilchCompiledLib.h"
#include "Core.h"
#include "Physics/Thermodynamics.h"
#include "Physics/PhysicsSystem.h"
#include "ResourceManager.h"
#include "Serializer/JSONSerializer.h"
#include "UISystem.h"
#include "ZInterface.h"

//testing includes
#include "ComponentInclude.h"
#include "RigidBody.h"
#include "IncludeForAllCollision.h"

#include "UpdateEvent.h"
#include "Zilch.hpp"
//#include "ZInterface.h"

#define _DEGUB
using namespace Framework;

//! Window title
const char WindowTitle [] = "CinderEngine";
const int ClientWidth = 1024;
const int ClientHeight = 1024;



int main (void)
{
  bool launchFullScreen = false;

  EnableMemoryLeakChecking ();

  // TODO (EXTRA): make a window to show while the game is loading
  CinderConsole::Create_Cinder_Console ("CinderEngineConsole");
  // TODO Make console accept input by pressing '`', if '`' is pressed again return to game
  
  //! Create the core engine which manages all systems.
  CoreEngine * engine = new CoreEngine ();
  Physics::ThermodynamicsSystem * thermo = new Physics::ThermodynamicsSystem ();
  WindowSystem * windows = new WindowSystem (WindowTitle, ClientWidth, ClientHeight, launchFullScreen);
  AudioSystem* audio = new AudioSystem ();
  EventSystem * events = new EventSystem ();
  ScriptSystem * zilch = new ScriptSystem();
  Physics::PhysicsSystem * phys = new Physics::PhysicsSystem ();
  ObjectSystem* objsys = new ObjectSystem ();
  UISystem* ui = new UISystem ();
  
  //Adding Pointer to ZilchInterface
  ZInterface::ObjectSys = objsys;
  ZInterface::WindowSys = windows;


  engine->AddSystem (phys);
  engine->AddSystem (windows);
  engine->AddSystem (audio);
  engine->AddSystem (events);
  engine->AddSystem (zilch);
  engine->AddSystem (thermo);
  engine->AddSystem(objsys);
  engine->AddSystem (ui);
  ////Cinder.PhysicsSystem = phys;
  ////Cinder.Thermodynamics = thero;
  //Cinder::Windows = windows;
  //Cinder::Audio = audio;
  //Cinder::Events = events;
  //Cinder::Zilch = zilch;
  //Cinder::Objsys = objsys;

  Resources resourceManager;
  resourceManager.Load_Resources ();

  //! Initialize all added Systems. DON'T INIT YOUR OWN
  engine->Initialize ();

  //Sound *test = audio->LoadSound("Pads.ogg", "NOISE", Sound::SOUND_2D, 1.0f);
  //test->GenerateNoise();
  //test->LowPassFilter();
  //test->SetLPF(0, 1.0);

  audio->LoadMicData ();


  //! activate the window.
  OBJECTSYSTEM->LoadLevel("ZilchTestLevel");


  // Connect example
  //MyClass _myclass;
  //EVENTSYSTEM->mConnect<KeyEvent, MyClass>(Events::KEY_RIGHT, &_myclass, &MyClass::WhenRightIsPressed);
  //EVENTSYSTEM->mConnect<KeyEvent, MyClass>(Events::KEY_LEFT, &_myclass, &MyClass::WhenLeftIsPressed);
  //OBJECTSYSTEM->LoadLevel("PhysicsTest.data");

  // Connect example
  //MyClass _myclass;
  //EVENTSYSTEM->mConnect<UpdateEvent, MyClass>(Events::UPDATEEVENT, &_myclass, &MyClass::Print);
	//ZInterface::OpenFile();

  //! Run the game! NOW!
  engine->GameLoop ();

  

  //! Delete all systems
  engine->DestroySystems ();

  //! Delete engine
  delete engine;

  //! Free console
  CinderConsole::Free_Cinder_Console ();

  return 0;
}


#endif
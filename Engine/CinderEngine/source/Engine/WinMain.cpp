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
#include "AudioEvents.h"
#include "ZilchCompiledLib.h"
#include "Core.h"
#include "Physics/Thermodynamics.h"
#include "Physics/PhysicsSystem.h"
#include "PhysicsSystemNew.h"
#include "ResourceManager.h"
#include "Serializer/JSONSerializer.h"
#include "UISystem.h"
#include "ZInterface.h"
#include "CheatCodes.h"

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

//class MyClass
//{
//public:
//  void TestUEDisconnectMember(UpdateEvent * e){
//    std::cout << "Member disconnect" << std::endl;
//  }
//};


void TestUEDisconnect(UpdateEvent* e)
{
  std::cout << "Global Disconnect:" << std::endl;
}

int main (void)
{
  bool launchFullScreen = false;

  EnableMemoryLeakChecking ();

  // TODO (EXTRA): make a window to show while the game is loading
  CinderConsole::Create_Cinder_Console ("CinderEngineConsole");
  // TODO Make console accept input by pressing '`', if '`' is pressed again return to game
  
  //! Create the core engine which manages all systems.
  CoreEngine                    * engine      = new CoreEngine ();
  Physics::ThermodynamicsSystem * thermo      = new Physics::ThermodynamicsSystem ();
  PhysicsSystemNew              * physNew     = new PhysicsSystemNew (1.0f / 60.0f, 10);
  WindowSystem                  * windows     = new WindowSystem (WindowTitle, ClientWidth, ClientHeight, launchFullScreen);
  AudioSystem                   * audio       = new AudioSystem ();
  EventSystem                   * events      = new EventSystem ();
  AudioEvents                   * audioEvents = new AudioEvents();
  ScriptSystem                  * zilch       = new ScriptSystem();
  ObjectSystem                  * objsys      = new ObjectSystem ();
  UISystem                      * ui          = new UISystem ();

  //Physics::PhysicsSystem * phys = new Physics::PhysicsSystem ();
  
  //Adding Pointer to ZilchInterface
  ZInterface::ObjectSys = objsys;
  ZInterface::WindowSys = windows;


  engine->AddSystem (physNew);
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
  ZInterface::ResourceSystem = &resourceManager;
  resourceManager.Load_Resources ();
  

  //! Initialize all added Systems. DON'T INIT YOUR OWN
  engine->Initialize ();
  audioEvents->Initialize();


  audio->LoadMicData ();

  //! activate the window.
  //Currently broken;
  //OBJECTSYSTEM->LoadAllLevels("..//..//Resources//Levels//MasterLevelFile.txt");

  OBJECTSYSTEM->LoadLevel("TeamLogo");
 //Initialize Cheat Codes
  Cheats::InitializeCheats();


  // Connect and Disconnect example
  //MyClass _myclass;
  //EVENTSYSTEM->mConnect<UpdateEvent, MyClass>(Framework::Events::UPDATEEVENT, &_myclass, &MyClass::TestUEDisconnectMember);
  //EVENTSYSTEM->mDisconnect<UpdateEvent, MyClass>(Framework::Events::UPDATEEVENT, &_myclass, &MyClass::TestUEDisconnectMember);
  //
  //EVENTSYSTEM->gConnect<UpdateEvent>(Framework::Events::UPDATEEVENT, &TestUEDisconnect);
  //EVENTSYSTEM->gDisconnect<UpdateEvent>(Framework::Events::UPDATEEVENT, &TestUEDisconnect);

	//ZInterface::OpenFile();
  // Update the ObjectManager to load in the first level
  OBJECTSYSTEM->Update(0.016);

  //! Run the game! NOW!
  engine->GameLoop ();

  

  //! Delete all systems
  engine->DestroySystems ();

  //! Delete engine
  delete engine;
  delete audioEvents;

  //! Free console
  CinderConsole::Free_Cinder_Console ();

  return 0;
}


#endif
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

#include <Precompiled.h>
#include "ZInterface.h"
#include "CheatCodes.h"
#define WINDOWSBUILD
#ifdef WINDOWSBUILD

#define _DEGUB
using namespace Framework;

//! Window title
const char WindowTitle [] = "CinderEngine";
const int ClientWidth = 1024;
const int ClientHeight = 576;

//class MyClass
//{
//public:
//  void TestUEDisconnectMember(UpdateEvent * e){
//    std::cout << "Member disconnect" << std::endl;
//  }
//};

#ifdef _DEBUG
#include "Profiler\SamplingProfiler.h"

SamplingProfiler* gProfiler;
void CALLBACK ProfilerCallback(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR)
{
  if (gProfiler)
  {
    gProfiler->TakeSample();
  }
}
#endif

void TestUEDisconnect(UpdateEvent* e)
{
  std::cout << "Global Disconnect:" << std::endl;
}

int main (void)
{
#ifdef _DEBUG
  gProfiler = new SamplingProfiler(100000); // 10000 is the default max number of samples to collect. For a fuller profile increase this number and for a quicker report decrease it.

  bool launchFullScreen = false;

  EnableMemoryLeakChecking ();

  // TODO (EXTRA): make a window to show while the game is loading
  CinderConsole::Create_Cinder_Console ("CinderEngineConsole");
#else
  bool launchFullScreen = true;
#endif

  // TODO Make console accept input by pressing '`', if '`' is pressed again return to game
  
  //! Create the core engine which manages all systems.
  CoreEngine                    * engine      = new CoreEngine ();
  EventSystem                   * events      = new EventSystem ();
  Physics::ThermodynamicsSystem * thermo      = new Physics::ThermodynamicsSystem ();
  Physics::FluidDynamicsSystem  * fluid       = new Physics::FluidDynamicsSystem();
  PhysicsSystem                 * phys        = new PhysicsSystem (1.0f / 60.0f, 10);
  WindowSystem                  * windows     = new WindowSystem (WindowTitle, ClientWidth, ClientHeight, launchFullScreen);
  AudioSystem                   * audio       = new AudioSystem ();
  AudioEvents                   * audioEvents = new AudioEvents();
  ScriptSystem                  * zilch       = new ScriptSystem();
  ObjectSystem                  * objsys      = new ObjectSystem ();
  UISystem                      * ui          = new UISystem ();

  //Physics::PhysicsSystem * phys = new Physics::PhysicsSystem ();
  
  //Adding Pointer to ZilchInterface
  ZInterface::ObjectSys = objsys;
  ZInterface::WindowSys = windows;


  engine->AddSystem (phys);
  engine->AddSystem (windows);
  engine->AddSystem (audio);
  engine->AddSystem (events);
  engine->AddSystem (zilch);
  engine->AddSystem (thermo);
  engine->AddSystem(fluid);
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


  audio->LoadMicData ();
  Sound *SplashScreenMusic = resourceManager.Get_Sound("SplashScreen.ogg");
    AUDIOEVENTS->unmanagedSounds.push_back(SplashScreenMusic);

  //! activate the window.
  //Currently broken;
  //OBJECTSYSTEM->LoadAllLevels("..//..//Resources//Levels//MasterLevelFile.txt");

  SplashScreenMusic->Play();
  OBJECTSYSTEM->LoadLevel("ZilchTestLevel");
  //OBJECTSYSTEM->ZilchLoadLevel(Zilch::String("ZilchTestLevel"));

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
  OBJECTSYSTEM->Update(0.016f);

  //! Run the game! NOW!
  engine->GameLoop ();

  //! Delete all systems
  engine->DestroySystems ();

  //! Delete engine
  delete engine;
  delete audioEvents;
  engine = nullptr;
  audioEvents = nullptr;

  //! Free console
  CinderConsole::Free_Cinder_Console ();

#ifdef _DEBUG
  delete gProfiler;
#endif

  return 0;
}


#endif
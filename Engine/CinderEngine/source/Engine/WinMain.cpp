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
#include "Shlwapi.h" //used for PathAppend and PathFileExists for file/directory management

#include "LevelEditor.h"

#define WINDOWSBUILD
#ifdef WINDOWSBUILD

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

void TestUEDisconnect(UpdateEvent* e)
{
  std::cout << "Global Disconnect:" << std::endl;
}

int main (void)
{
#ifdef _DEBUG
  bool launchFullScreen = false;

  EnableMemoryLeakChecking ();

  // TODO (EXTRA): make a window to show while the game is loading
  CinderConsole::Create_Cinder_Console ("CinderEngineConsole");

#pragma region RemoveProfilingReportsDirectory
  const int bufferSize = MAX_PATH;
  char oldDir[bufferSize]; // store the current directory

  // get the current directory, and store it
  if (!GetCurrentDirectory(bufferSize, oldDir)) 
  {
    std::cerr << "Error getting current directory: #" << GetLastError();
  }
  else
  {
    std::cout << "Current directory: " << oldDir << '\n';
  }

  // new directory
  char newDir[bufferSize];
  strcpy(newDir, oldDir);

  // old way of setting the directory to the profiling reports folder
  //  sprintf_s(newDir, "%s/%s", oldDir, R"(\..\logs\ProfilingReports)");
  if (PathAppend(newDir, R"(..\logs)"))
  {
    char* profilingReportsPath = newDir;
    if (PathFileExists(profilingReportsPath))
    {
      RemoveDirectory(profilingReportsPath);
    }
  }
#pragma endregion

#else
  bool launchFullScreen = true;
#endif

  // TODO Make console accept input by pressing '`', if '`' is pressed again return to game
  
  //! Create the core engine which manages all systems.
  CoreEngine                    * engine      = new CoreEngine ();
  EventSystem                   * events      = new EventSystem ();
  //Physics::ThermodynamicsSystem * thermo      = new Physics::ThermodynamicsSystem ();
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
  //engine->AddSystem (thermo);
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
  Sound *SplashScreenMusic = resourceManager.Get_Sound("CreditsMusic.ogg");
    AUDIOEVENTS->unmanagedSounds.push_back(SplashScreenMusic);

  //! activate the window.
  //Currently broken;
  //OBJECTSYSTEM->LoadAllLevels("..//..//Resources//Levels//MasterLevelFile.txt");

  SplashScreenMusic->Play();
  OBJECTSYSTEM->LoadLevel("L3");
  
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

  Panel::PanelManager			* panels = new Panel::PanelManager( Panel::PanelManager::GraphicAPI::OPENGL,
																	ClientWidth, ClientHeight );
  Editor::LevelEditor			* le = new Editor::LevelEditor( );
  le->InitializeEditor( );

  //! Run the game! NOW!
  engine->GameLoop ();

  // delete editors
  delete le;
  delete panels;

  //! Delete engine
  delete engine;
  delete audioEvents;
  engine = nullptr;
  audioEvents = nullptr;

  //! Free console
  CinderConsole::Free_Cinder_Console ();

  return 0;
}


#endif
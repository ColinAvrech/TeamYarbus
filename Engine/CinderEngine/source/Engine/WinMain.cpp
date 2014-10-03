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
#include "WindowSystem.h"
#include "EventSystem.h"
#include "SceneManager.h"
#include "AudioSystem.h"
#include "ZilchCompiledLib.h"
#include "Core.h"
#include "Physics/Thermodynamics.h"
#include "ResourceManager.h"
#include "Serializer\JSONSerializer.h"

//testing includes
#include "ComponentInclude.h"
#include "RigidBody.h"
#include "ColliderShape.h"
#include "ObjectSystem.h"


#define _DEGUB
using namespace Framework;

//! Window title
const char WindowTitle[] = "CinderEngine";
const int ClientWidth = 1024;
const int ClientHeight = 768;

void TestEventTest(GameObject* obj, CollisionEvent* _event)
{
  // Test for CollisionEvent
  std::cout << Console::darkmagenta << "COLLISION EVENT" << std::endl;


  //Test for LogicUpdateEvent
  //std::cout << Console::red << "I am UpdateEvent!" << std::endl;
  //std::cout << Console::blue << "dt:" << _event->Dt << std::endl;
  //std::cout << Console::green << "TimePassed:" << _event->TimePassed << std::endl;
}

int main(void)
{
  EnableMemoryLeakChecking();

  // TODO (EXTRA): make a window to show while the game is loading
  Console::Create_Cinder_Console("CinderEngineConsole");
  // TODO Make console accept input by pressing '`', if '`' is pressed again return to game

  //Test parser
  Serializer::ZeroSerializer testarchive;

  testarchive.open("Level.data");

  testarchive.CreateArchive();

  testarchive.DumpArchive(testarchive.GetTrunk());

  /*! Initialize the game engine*/
  
  //! Create the core engine which manages all systems.
  CoreEngine * engine         = new CoreEngine;
  WindowSystem * windows      = new WindowSystem (WindowTitle, ClientWidth, ClientHeight);
  SceneManager* sceneManager = new SceneManager ();
  AudioSystem* audio          = new AudioSystem();
  EventSystem * events        = new EventSystem ();
  ScriptSystem::
  ScriptSystem * zilch      = new ScriptSystem::ScriptSystem();
  Physics::
    ThermodynamicsSystem * thermo = new Physics::ThermodynamicsSystem();

  //test
  ObjectSystem* objsys = new ObjectSystem();
  

  engine->AddSystem (sceneManager);
  engine->AddSystem (windows);
  engine->AddSystem(audio);
  engine->AddSystem(events);
  engine->AddSystem(zilch);
  engine->AddSystem(thermo);
  engine->AddSystem(objsys);

  Resources resourceManager;
  resourceManager.Load_Resources();

  //! Initialize all added Systems. DON'T INIT YOUR OWN
  engine->Initialize();

  //! activate the window.
  //resourceManager.Get_Sound("music2.mp3")->LowPassFilter(60, 10);

  // Test of CollisionEvent
  /*
  GameObject * myobj = new GameObject(1666);
  std::string ColEvent = std::string("COLLISION_1666");
  std::cout << "Connecting_To_Event_Called:" << ColEvent << std::endl;
  EVENTSYSTEM->Connect(myobj, ColEvent, BaseEvent::BaseCall(TestEventTest));
  */

  
  GameObject* myobj = objsys->CreateObject();
  myobj->Transform = new Transform;
  static_cast<Transform*>(myobj->Transform)->setPos(0, 0, 0);
  static_cast<Transform*>(myobj->Transform)->setScale(100, 100, 0);
  myobj->Collider = new Physics::Circle(myobj);

  GameObject* myobj2 = objsys->CreateObject();
  myobj2->Transform = new Transform;
  static_cast<Transform*>(myobj2->Transform)->setPos(10, 0, 0);
  static_cast<Transform*>(myobj2->Transform)->setScale(100, 100, 0);
  myobj2->Collider = new Physics::Circle(myobj2);

  static_cast<Physics::Circle*>(myobj->Collider)->DetectCircle(static_cast<Physics::Circle*>(myobj2->Collider));
  std::cout << "I did naat";
  ///*Transform* transform =*/ (myobj->AddComponent(Transform::Name))->Initalize();
  //static_cast<Transform*>(myobj->GetComponent("Transform"))->setPos(0, 0, 0);
  //static_cast<RigidBody*>(myobj->AddComponent(Physics::RigidBody::Name));
  //static_cast<Circle*>myobj->AddComponent(Physics::Circle::Name);
  
 

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
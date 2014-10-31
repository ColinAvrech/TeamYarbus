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
#include "Physics/PhysicsSystem.h"
#include "ResourceManager.h"
#include "Serializer/JSONSerializer.h"


//testing includes
#include "ComponentInclude.h"
#include "RigidBody.h"
#include "ShapeCollider.h"
#include "ObjectSystem.h"

#include "UpdateEvent.h"


#define _DEGUB
using namespace Framework;

//! Window title
const char WindowTitle [] = "CinderEngine";
const int ClientWidth = 1024;
const int ClientHeight = 768;

// Connect example Class

//class MyClass
//{
//public:
//	void Print(UpdateEvent* e)
//	{
//		std::cout << "My Update: " << e->Dt << std::endl;
//	}
//};
//SCENE CHANGER
class MyClass
{
public:
	MyClass();

	void WhenRightIsPressed(KeyEvent* e)
	{
		if (e->KeyDown && !e->KeyRepeat)
		{
			int MaxSlides = LevelStates.size();
			if (Slide < MaxSlides - 1)
			{
				++Slide;
				std::string mystring = LevelStates[Slide]->c_str();
				OBJECTSYSTEM->LoadLevel(mystring);
			}
		}
	}

	void WhenLeftIsPressed(KeyEvent* e)
	{
		if (e->KeyDown && !e->KeyRepeat)
		{
			if (Slide > 0)
			{
				--Slide;
				std::string mystring = LevelStates[Slide]->c_str();
				OBJECTSYSTEM->LoadLevel(mystring);
			}
		}
	}
	int Slide = 0;

	std::vector<std::string*> LevelStates;


private:

};

MyClass::MyClass()
{
	LevelStates.push_back(new std::string("Slide1.data"));
	LevelStates.push_back(new std::string("Slide2.data"));
	LevelStates.push_back(new std::string("Level.data"));
	LevelStates.push_back(new std::string("Slide3.data"));
	LevelStates.push_back(new std::string("Slide4.data"));
	LevelStates.push_back(new std::string("Slide5.data"));
	LevelStates.push_back(new std::string("Slide6.data"));
	LevelStates.push_back(new std::string("Slide7.data"));
	//LevelStates.push_back(new std::string("Slide3.data"));
}

int main (void)
{
  EnableMemoryLeakChecking ();

  // TODO (EXTRA): make a window to show while the game is loading
  Console::Create_Cinder_Console ("CinderEngineConsole");
  // TODO Make console accept input by pressing '`', if '`' is pressed again return to game
  
  //! Create the core engine which manages all systems.
  CoreEngine * engine = new CoreEngine ();
  WindowSystem * windows = new WindowSystem (WindowTitle, ClientWidth, ClientHeight);
  SceneManager* sceneManager = new SceneManager ();
  AudioSystem* audio = new AudioSystem ();
  EventSystem * events = new EventSystem ();
  ScriptSystem * zilch = new ScriptSystem();
  Physics::ThermodynamicsSystem * thermo = new Physics::ThermodynamicsSystem ();
  Physics::PhysicsSystem * phys = new Physics::PhysicsSystem ();
  ObjectSystem* objsys = new ObjectSystem ();

  engine->AddSystem (phys);
  engine->AddSystem (thermo);
  //engine->AddSystem (sceneManager);
  engine->AddSystem (windows);
  engine->AddSystem (audio);
  engine->AddSystem (events);
  engine->AddSystem (zilch);
  engine->AddSystem (objsys);

  Resources resourceManager;
  resourceManager.Load_Resources ();

  //! Initialize all added Systems. DON'T INIT YOUR OWN
  engine->Initialize ();

  Sound *pads;
  pads = resourceManager.Get_Sound("Pads.ogg");
  pads->Play();
  pads->VolumeFade(0.6f, 3);
  pads->HighPassFilter();
  pads->SetHPF(600, 1);
  pads->Reverb();
  pads->SetReverbPreset(Sound::ARENA);

  audio->LoadMicData ();


  //! activate the window.
  OBJECTSYSTEM->LoadLevel("Slide1.data");

  // Connect example
  MyClass _myclass;
  EVENTSYSTEM->mConnect<KeyEvent, MyClass>(Events::KEY_RIGHT, &_myclass, &MyClass::WhenRightIsPressed);
  EVENTSYSTEM->mConnect<KeyEvent, MyClass>(Events::KEY_LEFT, &_myclass, &MyClass::WhenLeftIsPressed);
  //OBJECTSYSTEM->LoadLevel("PhysicsTest.data");

  // Connect example
  //MyClass _myclass;
  //EVENTSYSTEM->mConnect<UpdateEvent, MyClass>(Events::UPDATEEVENT, &_myclass, &MyClass::Print);


  //! Run the game! NOW!
  engine->GameLoop ();

  //! Delete all systems
  engine->DestroySystems ();

  //! Delete engine
  delete engine;

  //! Free console
  Console::Free_Cinder_Console ();

  return 0;
}


#endif
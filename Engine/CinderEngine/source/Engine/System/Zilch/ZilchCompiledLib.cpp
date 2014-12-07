/******************************************************************************/
/*!
\file   WindowSystem.h
\author Micah Rust, Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains all of the framework for creating and maintaining a window.
*/
/******************************************************************************/

#include "Common.h"
#include "ZilchCompiledLib.h"
#include <iostream>


#define ScriptFileList "../../Resources/Scripts/ScriptFiles.txt"
#define ScriptFilePath "../../Resources/Scripts/"


namespace Framework
{
  ScriptSystem* ZILCH = NULL;

  ScriptSystem::ScriptSystem() : ScriptCount(0)
  {
    ZILCH = this;
    LibList = new std::unordered_map < const char*, Zilch::LibraryRef > ;
  }

  bool ScriptSystem::Initialize()
  {
    std::cout << "========= Initializing Zilch ==========\n";
    /////////////////////////////////////////////////////////////////
    ZilchStartup(Debugging::UseZilchErrorHandler);

    //Zilch::Debugger debugger;
    //debugger.Host(8000);


	// Setup the console so that when we call 'Console.WriteLine' it outputs to stdio
	Zilch::EventConnect(&Console::Events, Events::ConsoleWrite, DefaultWriteText);

	// We can also setup the console so that any 'Read' functions will attempt to read from stdin
	Zilch::EventConnect(&Console::Events, Events::ConsoleRead, DefaultReadText);

    // This class encompasses all compilation errors that can occur when compiling Zilch code
    // Its responsibility is to provide friendly error messages, error codes, and callbacks to the user
    //CompilationErrors errors; //this is a public member of Zilch system

    // Here, we can register our own callback for when compilation errors occur
    // The default callback prints the file, line/character number, and message to stderr
    Errors.AddCallback(DefaultErrorCallback, nullptr);

    // A project contains all of the code we combine together to make a single Zilch library
    Project project(Errors);

    LoadZilchFiles(project);

    // Create a list of dependent libraries, in our case we're really not adding anything to this
    // A side note: the Core library in Zilch is always added as a dependency, because Core includes
    // things like Integer, Boolean, Real, the basic vector types, String, etc
    Module dependencies;
    dependencies.push_back(CinderZilch::GetInstance().GetLibrary());
    CompileScripts(project, dependencies);

    LinkedLibs = dependencies.Link();
    ErrorIf(LinkedLibs == nullptr, "Failed to link libraries together");
	LinkedLibs->SetExceptionCallback((Zilch::ExceptionCallback)Zilch::DefaultExceptionCallback, nullptr);
    
    //debugger.AddState(LinkedLibs);
    /////////////////////////////////////////////////////////////////
    std::cout << "Zilch Initialized" << std::endl;
    return true;
  }
    
  const string ScriptSystem::GetName()
  {
    return "ScriptSystem";
  }

  void ScriptSystem::Update(const double &dt)
  {
      
  }

  ScriptSystem::~ScriptSystem()
  {
    delete LinkedLibs;
    delete LibList;
    LinkedLibs = nullptr;
    LibList = nullptr;
    Zilch::ZilchShutdown();
  }

  void ScriptSystem::LoadZilchFiles(Zilch::Project & project)
  {
    std::cout << "Loading Scripts \n";
    std::fstream fs;
    fs.open(ScriptFileList);

    ErrorIf(!fs.is_open(), "Error, Could not open ScriptFileList");
    string zilchfile;

    while (!fs.eof())
    {
      //Make filename to load
      zilchfile.append(ScriptFilePath);
      char filename[256];
      fs.getline(filename, 256);
      if (filename[0] != '\0')
      {
        zilchfile.append(filename);
        std::cout << "Loading : " << filename << std::endl;

        //Load file ISN't WORKING?!?
        project.AddCodeFromFile(zilchfile.c_str());
      }
      zilchfile.clear();
    }
  }

  void ScriptSystem::CompileScripts(Project& project, Module& dependencies)
  {
    std::cout << "Compiling scripts \n";
    //This will loop through all the scripts//
    // The 'Compile' function actually returns a Zilch Library
    // Be careful! If the code fails to compile, this function will return null
    //not sure what "Test" is?!?
    lib = project.Compile("Test", dependencies, EvaluationMode::Project);
	    ErrorIf(lib == nullptr, "Failed to compiler library");

    // We want to link together all the libraries that we depended upon, along with our own library
    dependencies.push_back(lib);
  }

  Zilch::LibraryRef *ScriptSystem::GetZilchLib(const char *ScriptName)
  {
    return &(*LibList)[ScriptName];
  }
} //namespace Framework
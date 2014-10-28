
#include "Common.h"
#include "ZilchCompiledLib.h"
#include <iostream>

ZilchStaticLibrary(CinderZilch);

#define ScriptFileList "../../Resources/Scripts/ScriptFiles.txt"
#define ScriptFilePath "../../Resources/Scripts/"

using namespace Zilch;

namespace Framework
{
  ScriptSystem::ScriptSystem() : ScriptCount(0)
  {
    //SCRIPTSYSTEM = this;
    LibList = new std::unordered_map < const char*, Zilch::LibraryRef > ;
  }

  bool ScriptSystem::Initialize()
  {
    std::cout << "========= Initializing Zilch ==========\n";
    /////////////////////////////////////////////////////////////////
    ZilchStartup(Debugging::UseZilchErrorHandler);

    //Debugger debugger;


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
    Module dependencies(Errors);
    //dependencies.AddLibrary(Cinder::GetInstance().GetLibrary());
    CompileScripts(project, dependencies);

    LinkedLibs = dependencies.Link();
    ErrorIf(LinkedLibs == nullptr, "Failed to link libraries together");
    
    /////////////////////////////////////////////////////////////////
    std::cout << "Zilch Initialized" << std::endl;
    return true;
  }
    
  const std::string ScriptSystem::GetName()
  {
    return "ScriptSystem";
  }

  void ScriptSystem::Update(const double dt)
  {
      
  }

  ScriptSystem::~ScriptSystem()
  {
    delete LinkedLibs;
    delete LibList;
    Zilch::ZilchShutdown();
  }

  void ScriptSystem::LoadZilchFiles(Zilch::Project & project)
  {
    std::cout << "Loading Scripts \n";
    std::fstream fs;
    fs.open(ScriptFileList);

    ErrorIf(!fs.is_open(), "Error, Could not open ScriptFileList");
    std::string zilchfile;

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
    LibraryRef library = project.Compile("Test", dependencies, EvaluationMode::Project);
    ErrorIf(library == nullptr, "Failed to compiler library");

    // We want to link together all the libraries that we depended upon, along with our own library
    dependencies.AddLibrary(library);
  }

  Zilch::LibraryRef *ScriptSystem::GetZilchLib(const char *ScriptName)
  {
    return &(*LibList)[ScriptName];
  }
} //namespace Framework
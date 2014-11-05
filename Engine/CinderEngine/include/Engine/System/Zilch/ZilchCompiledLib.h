/******************************************************************************/
/*!
\file   ZilchCompiledLib.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Container for the compiled Zilch libraries.
*/
/******************************************************************************/

#pragma once
/*------------------------------------------------------------------------------
// Includes
------------------------------------------------------------------------------*/
#include "Common.h"
#include "Zilch.hpp"
#include "BaseSystem.h"

using namespace Zilch;
ZilchStaticLibrary(CinderZilch);

namespace Framework
{
  /*------------------------------------------------------------------------------
  // Class
  ------------------------------------------------------------------------------*/
  class ScriptSystem : public BaseSystem
  {
  public:
    ScriptSystem();
    ~ScriptSystem();
      
    // Called after System is Initialized
    virtual bool Initialize(void) override;

    // Called every frame
    virtual void Update(const double dt) override;

    // Returns name of System
    virtual const std::string GetName();

    //Returns pointer to the required LibraryRef
    Zilch::LibraryRef *GetZilchLib(const char *ScriptName);

    //Returns pointer to the dependency library
    Zilch::ExecutableState *GetDependencies(){ return LinkedLibs; }

    //Hack!!!!!!!
    Zilch::LibraryRef lib;
  private:
    int ScriptCount;

    /* This class encompasses all compilation errors that can occur when compiling 
        Zilch code. Its responsibility is to provide friendly error messages, 
        error codes, and callbacks to the user*/
    Zilch::CompilationErrors Errors;

    Zilch::Debugger * Debugger;

    /* Link all the libraries together into one ExecutableState*/
    Zilch::ExecutableState* LinkedLibs;

    //LoadAllZilchFiles into the static Library
    void LoadZilchFiles(Zilch::Project & project);
    //Compile all the scripts
    void CompileScripts(Zilch::Project& project, Zilch::Module& dependencies);

    //Unordered map of compiled Zilch libraries
    std::unordered_map<const char*, Zilch::LibraryRef>* LibList;

  };  //class ScriptSystem

  extern ScriptSystem * ZILCH;

} //Framework



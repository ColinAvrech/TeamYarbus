/******************************************************************************/
/*!
\file   ZilchCompiledLib.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Container for the compiled Zilch libraries.
*/
/******************************************************************************/

#ifndef _SCRIPT_H
#define _SCRIPT_H

/*------------------------------------------------------------------------------
// Includes
------------------------------------------------------------------------------*/
#include "Common.h"
#include "Zilch.hpp"
#include "BaseSystem.h"
#include <unordered_map>

namespace Framework
{
  namespace ScriptSystem
  {
    /*------------------------------------------------------------------------------
    // Class
    ------------------------------------------------------------------------------*/
    class ScriptSystem : public BaseSystem
    {
    public:
      /*------------------------------------------------------------------------------
      // Constructors
      ------------------------------------------------------------------------------*/
#pragma region Constructors
      ScriptSystem();
#pragma endregion

      /*------------------------------------------------------------------------------
      // Public Variables
      ------------------------------------------------------------------------------*/
#pragma region Public Variables

#pragma endregion

      /*----------------------------------------------------------------------------*/
      // Public Structs
      /*----------------------------------------------------------------------------*/
#pragma region Public Structs

#pragma endregion

      /*------------------------------------------------------------------------------
      // Public Functions
      ------------------------------------------------------------------------------*/
#pragma region Public Functions

      // Called after System is Initialized
      bool Initialize();

      // Called every frame
      void Update(const double dt);

      // Returns name of System
      const std::string GetName();
      //Returns pointer to the required LibraryRef
      Zilch::LibraryRef *GetZilchLib(const char *ScriptName);

      //Returns pointer to the dependency library
      Zilch::ExecutableState *GetDependencies(){ return LinkedLibs; }

#pragma endregion


      /*------------------------------------------------------------------------------
      // Static Public Variables
      ------------------------------------------------------------------------------*/
#pragma region Static Public Variables

#pragma endregion


      /*------------------------------------------------------------------------------
      // Static Public Functions
      ------------------------------------------------------------------------------*/
#pragma region Static Public Functions

#pragma endregion


      /*------------------------------------------------------------------------------
      // Destructor
      ------------------------------------------------------------------------------*/
#pragma region Destructors
      ~ScriptSystem();

#pragma endregion
    private:
      /*------------------------------------------------------------------------------
      // Private Variables
      ------------------------------------------------------------------------------*/
#pragma region Private Variables
      //Total number of scripts added
      int ScriptCount;
#pragma endregion


      /*------------------------------------------------------------------------------
      // Private Structs
      ------------------------------------------------------------------------------*/
#pragma region Private Structs
      /* This class encompasses all compilation errors that can occur when compiling 
         Zilch code. Its responsibility is to provide friendly error messages, 
         error codes, and callbacks to the user*/
      Zilch::CompilationErrors Errors;

      /*// Link all the libraries together into one ExecutableState*/
      Zilch::ExecutableState* LinkedLibs;

      //Unordered map of compiled Zilch libraries
      std::unordered_map<const char*, Zilch::LibraryRef>* LibList;

#pragma endregion

      /*------------------------------------------------------------------------------
      // Private Functions
      ------------------------------------------------------------------------------*/
#pragma region Private Functions

#pragma endregion


      /*------------------------------------------------------------------------------
      // Static Functions
      ------------------------------------------------------------------------------*/
#pragma region Static Functions

#pragma endregion

    };  //class ScriptSystem
  } //ScriptSystem
} //Framework



#endif				// _SCRIPT_H
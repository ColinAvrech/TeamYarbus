/******************************************************************************/
/*!
\file   Graphics.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

/*------------------------------------------------------------------------------
// Includes
------------------------------------------------------------------------------*/
#pragma region Includes
#include "GraphicsSystem.h"
#pragma endregion

namespace Framework 
{
/*------------------------------------------------------------------------------
// Class Implementation
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
// Constructors
------------------------------------------------------------------------------*/
#pragma region Constructors

  GraphicsSystem :: GraphicsSystem ()
  {
    int argc = 1;
    char *argv [1] = { (char*)"Something" };
    GRAPHICSSYSTEM = this;
  }


#pragma endregion

/*------------------------------------------------------------------------------
// Public Variables
------------------------------------------------------------------------------*/
#pragma region Public Variables

  GraphicsSystem* GRAPHICSSYSTEM = NULL;

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

  bool GraphicsSystem::Initialize ()
  {
    std::cout << GraphicsSystem::GetName () + " initialized";
    return true;
  }


  void GraphicsSystem::Update (const double dt)
  {
  }

  // MAIN FUNCTION - DRAWS EVERYTHING ON THE SCREEN
  void Draw ()
  {
    std::cout << "Drawing";
  }


  void Loop ()
  {

  }


  // CALLED AFTER WINDOW IS RESIZED
  void Resized (int width, int height)
  {

  }


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
#pragma region Destructor

  GraphicsSystem :: ~GraphicsSystem ()
  {
  }

#pragma endregion


/*------------------------------------------------------------------------------
// Private Variables
------------------------------------------------------------------------------*/
#pragma region Private Variables

#pragma endregion


/*------------------------------------------------------------------------------
// Private Structs
------------------------------------------------------------------------------*/
#pragma region Private Structs

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

}
//------------------------------------------------------------------------------

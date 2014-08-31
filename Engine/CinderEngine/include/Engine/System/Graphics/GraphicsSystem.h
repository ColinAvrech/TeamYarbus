/******************************************************************************/
/*!
\file   Graphics.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

/*------------------------------------------------------------------------------
// Includes
------------------------------------------------------------------------------*/
#pragma region Includes
#include "BaseSystem.h"
#include <iostream>
#pragma endregion

namespace Framework
{
/*------------------------------------------------------------------------------
// Class
------------------------------------------------------------------------------*/
class GraphicsSystem : public BaseSystem
{
public:

/*------------------------------------------------------------------------------
// Constructors
------------------------------------------------------------------------------*/
#pragma region Constructors
  GraphicsSystem ();
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
  virtual bool Initialize ();

  // Called every frame
  virtual void Update (const double dt);

  // Returns name of System
  virtual const std::string GetName ()
  {
    return "GraphicsSystem"; 
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
#pragma region Destructors
  ~GraphicsSystem ();

#pragma endregion

private:
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

};

extern GraphicsSystem* GRAPHICSSYSTEM;

void Draw ();
void Loop ();
void Resized (int, int);

}

//------------------------------------------------------------------------------

#endif				// _GRAPHICS_H
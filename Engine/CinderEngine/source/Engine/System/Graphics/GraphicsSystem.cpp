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

  GraphicsSystem::GraphicsSystem()
  {
    GRAPHICSSYSTEM = this;
  }

#pragma endregion

/*------------------------------------------------------------------------------
// Public Variables
------------------------------------------------------------------------------*/
#pragma region Public Variables

  GraphicsSystem* GRAPHICSSYSTEM = NULL;

  // THIS IS HOW A TYPICAL GAMEOBJECT WILL LOOK AFTER COMPONENT BASED DESIGN
  // IS FULLY IMPLEMENTED
  // Transform transform;      // HOLDS ALL TRANSFORMATION DATA
  // SpriteRenderer renderer;  // RENDERS THE SPRITE OBJECT
  //////////////////////////////////////////////////////////////////////////
  // SpriteRenderer renderer1;
  // Point verts [] = { Point (3, 2), Point (-4, 5), Point (-6, -7), Point (5, -8) };
  // SpriteRenderer renderer2 (verts);


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
  }

  /*!
  This function sets the Window's Pixelformat
  */
  void GraphicsSystem::SetupPixelFormatWindows(HDC hDC)
  {
  }


  void GraphicsSystem::Update (const double dt)
  {
  }


  void KeyPressed (unsigned char c, int, int)
  {
  }
 

  void GraphicsSystem::DrawDebug(const double dt)
  {
  }

  void GraphicsSystem::DrawSprites(const double dt)
  {
  }

  void GraphicsSystem::DrawParticles(const double dt)
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

  GraphicsSystem::~GraphicsSystem()
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

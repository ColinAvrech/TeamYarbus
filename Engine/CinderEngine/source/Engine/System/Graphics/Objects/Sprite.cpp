/******************************************************************************/
/*!
\file   Sprite.cpp
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
#include "Sprite.h"
#pragma endregion


namespace Framework
{

/*------------------------------------------------------------------------------
// Class Implementation
------------------------------------------------------------------------------*/
#pragma region Class Implementation


/*------------------------------------------------------------------------------
// Constructors
------------------------------------------------------------------------------*/
#pragma region Constructors
  Sprite::Sprite ()
  {
    vertices [0] = Point (1, 1);
    vertices [1] = Point (-1, 1);
    vertices [2] = Point (-1, -1);
    vertices [3] = Point (1, -1);
  }

  Sprite::Sprite (Point verts [])
  {
    vertices [0] = verts [0];
    vertices [1] = verts [1];
    vertices [2] = verts [2];
    vertices [3] = verts [3];
  }
#pragma endregion

/*------------------------------------------------------------------------------
// Public Variables - Methods
------------------------------------------------------------------------------*/
#pragma region Public
#pragma endregion

/*------------------------------------------------------------------------------
// Static Public Variables - Methods
------------------------------------------------------------------------------*/
#pragma region Static Public
#pragma endregion

/*------------------------------------------------------------------------------
// Private Variables - Methods
------------------------------------------------------------------------------*/
#pragma region Private

#pragma endregion


  //----------------------------------------------------------------------------

#pragma endregion

}

//------------------------------------------------------------------------------
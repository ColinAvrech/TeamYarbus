/******************************************************************************/
/*!
\file   SpriteRenderer.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _SPRITE_RENDERER_H
#define _SPRITE_RENDERER_H

/*------------------------------------------------------------------------------
// Includes
------------------------------------------------------------------------------*/
#pragma region Includes
#include "Sprite.h"
#include "glut.h"
#pragma endregion


namespace Framework
{

/*------------------------------------------------------------------------------
// Class
------------------------------------------------------------------------------*/
#pragma region Class
  class  SpriteRenderer
  {
  public:

/*------------------------------------------------------------------------------
// Constructors
------------------------------------------------------------------------------*/
#pragma region Constructors
    SpriteRenderer ();
    SpriteRenderer (Point []);
#pragma endregion

/*------------------------------------------------------------------------------
// Public Variables - Methods
------------------------------------------------------------------------------*/
#pragma region Public
    Sprite sprite;
    Point temp [4];
    void Render (int material = 0);

#pragma endregion

/*------------------------------------------------------------------------------
// Static Public Variables - Methods
------------------------------------------------------------------------------*/
#pragma region Static Public
#pragma endregion

  private:
/*------------------------------------------------------------------------------
// Private Variables - Methods
------------------------------------------------------------------------------*/
#pragma region Private
#pragma endregion
  };

  //----------------------------------------------------------------------------
#pragma endregion

}

//------------------------------------------------------------------------------

#endif				// _SPRITE_RENDERER_H
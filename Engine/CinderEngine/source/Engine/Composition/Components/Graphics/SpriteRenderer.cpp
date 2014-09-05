/******************************************************************************/
/*!
\file   SpriteRenderer.cpp
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
#include "SpriteRenderer.h"
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
  SpriteRenderer::SpriteRenderer ()
  {}

  SpriteRenderer::SpriteRenderer (Point verts [])
  {
    sprite.vertices [0] = verts [0];
    sprite.vertices [1] = verts [1];
    sprite.vertices [2] = verts [2];
    sprite.vertices [3] = verts [3];
  }
#pragma endregion

/*------------------------------------------------------------------------------
// Public Variables - Methods
------------------------------------------------------------------------------*/
#pragma region Public

  void SpriteRenderer::Render (int material)
  {
    glColor3f (0, 0, 1);
    glBegin (GL_POLYGON);
    glVertex2f (temp [0].x, temp [0].y);
    glVertex2f (temp [1].x, temp [1].y);
    glVertex2f (temp [2].x, temp [2].y);
    glEnd ();

    glColor3f (1, 0, 0);
    glBegin (GL_POLYGON);
    glVertex2f (temp [0].x, temp [0].y);
    glVertex2f (temp [3].x, temp [3].y);
    glVertex2f (temp [2].x, temp [2].y);
    glEnd ();
  }

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
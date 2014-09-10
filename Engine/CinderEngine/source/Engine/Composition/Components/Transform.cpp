/******************************************************************************/
/*!
\file   Transform.cpp
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
#include "Transform.h"
#pragma endregion


namespace Framework
{

/*------------------------------------------------------------------------------
// Class Implementation
------------------------------------------------------------------------------*/
#pragma region Class Implementation

#pragma endregion

/*------------------------------------------------------------------------------
// Public Variables - Methods
------------------------------------------------------------------------------*/
#pragma region Public
  void Transform::Translate (float x, float y)
  {
    Trans (Vector (x, y));
  }

  void Transform::Translate (Vec2&  pos)
  {
    Trans (Vector (pos.x_, pos.y_));
  }

  void Transform::Rotate (float z)
  {
    Rot (z);
  }

  void Transform::ScaleXY (float x, float y)
  {
    Scale (x, y);
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
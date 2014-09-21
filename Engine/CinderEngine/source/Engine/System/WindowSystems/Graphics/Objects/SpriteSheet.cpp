/******************************************************************************/
/*!
\file   SpriteSheet.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#include "SpriteSheet.h"

namespace Framework
{

  // Constructor
  SpriteSheet::SpriteSheet (const char* filename, const int _rows, const int _columns, int _samples) :
    Texture (filename), rows (_rows), columns (_columns), samples (_samples)
  {
  }

  // Destructor
  SpriteSheet::~SpriteSheet ()
  {}


  int SpriteSheet::Get_Rows ()
  {
    return rows;
  }

  int SpriteSheet::Get_Columns ()
  {
    return columns;
  }

  int SpriteSheet::Get_Samples ()
  {
    return samples;
  }
}
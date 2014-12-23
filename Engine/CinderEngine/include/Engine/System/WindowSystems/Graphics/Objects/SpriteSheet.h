/******************************************************************************/
/*!
\file   SpriteSheet.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SPRITE_SHEET_H
#define _SPRITE_SHEET_H

namespace Framework
{

  class SpriteSheet : public Texture
  {
  public:
    SpriteSheet (const char* filename, const int _rows, const int _columns, int _samples);
    ~SpriteSheet ();
  
    int Get_Rows ();
    int Get_Columns ();
    int Get_Samples ();

  private:
    const int rows;
    const int columns;
    int samples;
  };  
}

#endif
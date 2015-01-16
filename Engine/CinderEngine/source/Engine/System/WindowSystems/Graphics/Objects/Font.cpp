/******************************************************************************/
/*!
\file   Texture.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Font.h"
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>

namespace Framework
{
	ZilchDefineType(Font, CinderZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
		ZilchBindFieldGet(Name);
	}

  Font::Font ()
  {
  }


  // LOADS TEXTURE FROM SPECIFIED FILE
  Font::Font (const char* filename, string name)
  {
	  Name = Zilch::String(name.c_str());
	  Load_Font (filename);
  }


  // DESTRUCTOR
  // DELETES FONT OBJECT
  Font::~Font ()
  {
	  if (face)
	  {
		  FT_Done_Face(face);
	  }
  }


  // LOADS FONT USING FREETYPE
  void Font::Load_Font(const char* filename)
  {
	  int error = FT_New_Face(Resources::RS->FontLibrary, filename, 0, &face);
	  if (error == FT_Err_Unknown_File_Format)
	  {
		  std::cout << "ERROR: THE FONT FILE COULD BE OPENED AND READ BUT ITS FORMAT IS UNSUPPORTED!" << std::endl;
	  }
	  else if (error)
	  {
		  std::cout << "ERROR: THE FONT FILE FAILED TO OPEN!" << std::endl;
	  }
  }
}
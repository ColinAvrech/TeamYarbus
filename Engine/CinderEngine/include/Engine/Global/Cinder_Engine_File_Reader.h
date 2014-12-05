/******************************************************************************/
/*!
\file   Cinder_Engine_File_Reader.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/


#ifndef _CINDER_ENGINE_FILE_READER
#define _CINDER_ENGINE_FILE_READER
	
#include <assert.h>
#include <fstream>
#include "Cinder_Engine_Data_Types.h"

namespace Framework
{
  class Cinder_File_Reader
  {
  public:

    void LoadFile (const CHAR* file_name)
    {
      input.open (file_name, std::ifstream::binary);
    }

    void ReadCharacters (CHAR* characters, INT32 num_of_characters)
    {
      input.read (characters, num_of_characters);
    }

    void SkipCharacters (INT32 num_of_characters)
    {
      input.ignore (num_of_characters);
    }

    INT32 toInt (const char* bytes)
    {
      return (INT32) (((unsigned char) bytes [3] << 24) |
        ((unsigned char) bytes [2] << 16) |
        ((unsigned char) bytes [1] << 8) |
        (unsigned char) bytes [0]);
    }

    INT16 toShort (const char* bytes)
    {
      return (INT16) (((unsigned char) bytes [1] << 8) |
        (unsigned char) bytes [0]);
    }

    int readInt ()
    {
      char buffer [4];
      ReadCharacters (buffer, 4);
      return toInt (buffer);
    }

    short readShort ()
    {
      char buffer [2];
      ReadCharacters (buffer, 2);
      return toShort (buffer);
    }

    void CloseTheFile ()
    {
      input.close ();
    }

    void SeekInBeginning (int data)
    {
      input.seekg (data, std::ios_base::beg);
    }

  private:
    std::ifstream input;
  };
}

#endif
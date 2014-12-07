/******************************************************************************/
/*!
\file   Cinder_Image_Loader.cpp
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "Cinder_Engine_Image_Loader.h"
#include "Cinder_Engine_File_Reader.h"
#include "Cinder_Engine_Data_Types.h"

namespace Framework
{
  void Cinder_Image_Loader::LoadBMP (Cinder_Image_Loader & image, const CHAR * filename)
  {
    INT32
      dataOffset,
      headerSize,
      width,
      height,
      bytesPerRow,
      size;
    CHAR
      * pixels3,
      *pixels2;
    INT32
      index_x,
      index_y,
      index_c;
    Cinder_File_Reader
      file_reader_engine;

    file_reader_engine.LoadFile (filename);
    file_reader_engine.SkipCharacters (10);

    dataOffset = file_reader_engine.readInt ();
    headerSize = file_reader_engine.readInt ();

    switch (headerSize)
    {
    case 40:
      width = file_reader_engine.readInt ();
      height = file_reader_engine.readInt ();
      break;
    case 12:
      width = file_reader_engine.readShort ();
      height = file_reader_engine.readShort ();
      break;
    }
    bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
    size = bytesPerRow * height;
    pixels3 = new CHAR [size];
    file_reader_engine.SeekInBeginning (dataOffset);
    file_reader_engine.ReadCharacters (pixels3, size);
    pixels2 = new CHAR [width * height * 3];

    CINDER_LOOP_THROUGH_INDEX (index_y, 0, height - 1)
    {
      CINDER_LOOP_THROUGH_INDEX (index_x, 0, width - 1)
      {
        CINDER_LOOP_THROUGH_INDEX (index_c, 0, 2)
        {
          pixels2 [3 * (width * index_y + index_x) + index_c] =
            pixels3 [bytesPerRow * index_y + 3 * index_x + (2 - index_c)];
        }
      }
    }
    file_reader_engine.CloseTheFile ();
    image = Cinder_Image_Loader (pixels2, width, height);
    delete [] pixels2;
    delete [] pixels3;
    pixels2 = nullptr;
    pixels3 = nullptr;
  }
}
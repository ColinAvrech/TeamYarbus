/******************************************************************************/
/*!
\file   Cinder_Engine_Image_Loader.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _CINDER_IMAGE_LOADER_H
#define _CINDER_IMAGE_LOADER_H

#include "Cinder_Engine_File_Reader.h"
#include "Cinder_Engine_Data_Types.h"

namespace Framework
{
  class Cinder_Image_Loader
  {
  public:

    Cinder_Image_Loader () :
      pixels (0),
      width (0),
      height (0)
    {}

    virtual ~Cinder_Image_Loader ()
    {
      delete [] pixels;
    }

    Cinder_Image_Loader (const Cinder_Image_Loader& source)
      :
      width (source.width),
      height (source.height)
    {
      INT32
        index_x,
        index_y,
        index_c;

      pixels = new CHAR [width * height * 3];

      CINDER_LOOP_THROUGH_INDEX (index_y, 0, height - 1)
      {
        CINDER_LOOP_THROUGH_INDEX (index_x, 0, width - 1)
        {
          CINDER_LOOP_THROUGH_INDEX (index_c, 0, 2)
          {
            pixels [3 * (width * index_y + index_x) + index_c] =
              source.pixels [3 * (width * index_y + index_x) + index_c];
          }
        }
      }
    }


    Cinder_Image_Loader (const CHAR* other_pixels, const INT32 w, const INT32 h) :
      width (w),
      height (h)
    {
      INT32
        index_x,
        index_y,
        index_c;

      pixels = new CHAR [width * height * 3];

      CINDER_LOOP_THROUGH_INDEX (index_y, 0, height - 1)
      {
        CINDER_LOOP_THROUGH_INDEX (index_x, 0, width - 1)
        {
          CINDER_LOOP_THROUGH_INDEX (index_c, 0, 2)
          {
            pixels [3 * (width * index_y + index_x) + index_c] =
              other_pixels [3 * (width * index_y + index_x) + index_c];
          }
        }
      }
    }


    Cinder_Image_Loader& operator= (const Cinder_Image_Loader& source)
    {
      INT32
        index_x,
        index_y,
        index_c;

      width = source.width;
      height = source.height;
      pixels = new CHAR [width * height * 3];

      CINDER_LOOP_THROUGH_INDEX (index_y, 0, height - 1)
      {
        CINDER_LOOP_THROUGH_INDEX (index_x, 0, width - 1)
        {
          CINDER_LOOP_THROUGH_INDEX (index_c, 0, 2)
          {
            pixels [3 * (width * index_y + index_x) + index_c] =
              source.pixels [3 * (width * index_y + index_x) + index_c];
          }
        }
      }

      return *this;
    }


    // Gettors
    INT32 GetHeight ()
    {
      return height;
    }


    INT32 GetWidth ()
    {
      return width;
    }


    const CHAR* GetPixels ()
    {
      return pixels;
    }


    static void LoadBMP (Cinder_Image_Loader& image, const CHAR* filename);

  private:
    CHAR * pixels;
    INT32 width;
    INT32 height;

  };
}

#endif

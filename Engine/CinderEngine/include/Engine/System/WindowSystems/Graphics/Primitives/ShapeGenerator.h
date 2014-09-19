/******************************************************************************/
/*!
\file   ShapeGenerator.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SHAPE_GEN_H
#define _SHAPE_GEN_H

#include "ShapeData.h"

namespace Framework
{

  class ShapeGenerator
  {
  public:

  ShapeGenerator ();
  ~ShapeGenerator ();

  static ShapeData Generate_Quad ();
  static ShapeData Generate_Cube ();
  
  private:

  };  
}

#endif
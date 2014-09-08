/******************************************************************************/
/*!
\file   Template.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The hierarchy component is used to parent objects to one another
*/
/******************************************************************************/


#pragma once

#include "Component.h"
#include "Affine.h"
#include "GOC.h"
#include "Common.h"
namespace Framework
{
  class Hierarchy : public GameComponent
  {
  public:
    //Constructor: The children list is initially empty
    Hierarchy(GOC* obj)
      :_Children(NULL){}
    //Destructor
    ~Hierarchy();
    //Parent another object to this object
    void parent(GOC *obj);

  private:
    //The list of parented objects
    std::vector <GOC*> _Children;
  };
}
/******************************************************************************/
/*!
\file   Fluid_Physics_Spring_Array.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _FLUID_PHYSICS_SPRING_ARRAY_H
#define _FLUID_PHYSICS_SPRING_ARRAY_H

#include "Cinder_Engine_Data_Types.h"
#include "Cinder_Engine_Array.h"
#include "Fluid_Physics_Spring.h"
#include "Fluid_Physics_Object.h"
#include "Cinder_Engine_Bool_Array2D.h"


namespace Framework
{
  class Fluid_Physics_Spring_Array : public CINDER_ARRAY<Fluid_Physics_Spring>
  {
  public:

    Fluid_Physics_Spring_Array () : ExistenceTable ()
    {}

    Fluid_Physics_Spring_Array (const Fluid_Physics_Spring_Array& source) : ExistenceTable (source.ExistenceTable)
    {}

    virtual ~Fluid_Physics_Spring_Array ()
    {}

    Fluid_Physics_Spring_Array& operator= (const Fluid_Physics_Spring_Array& source)
    {
      ExistenceTable = source.ExistenceTable;
    }

    // Gettors
    const Cinder_Bool_Array2D& GetExistenceTable () const
    {
      return ExistenceTable;
    }

    void SetExistenceTable (const Cinder_Bool_Array2D& existence_table)
    {
      ExistenceTable = existence_table;
    }

    void SetItemInExistenceTable (const INDEX index_x, const INDEX index_y, BOOL item)
    {
      ExistenceTable [index_x][index_y] = item;
    }

    void SetMaxNumberOfPoints (const UINT32 size)
    {
      UINT32
        counter;
      ExistenceTable.Allocate (size);

      CINDER_LOOP_THROUGH_INDEX (counter, 0, size - 1)
      {
        ExistenceTable [counter].Allocate (size);
      }
    }

  private:
    Cinder_Bool_Array2D ExistenceTable;
  };
}

#endif
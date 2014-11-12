/******************************************************************************/
/*!
\file   Cinder_Engine_Array.h
\author Manas Sudhir Kulkarni
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#ifndef _CINDER_ENGINE_ARRAY_TYPE
#define _CINDER_ENGINE_ARRAY_TYPE


#include <vector>
#include "Cinder_Engine_Data_Types.h"
using namespace std;

namespace Framework
{
  template < class my_type >
  class CINDER_ARRAY : private vector< my_type >
  {
  public:

    CINDER_ARRAY () : vector< my_type > ()
    {}

    CINDER_ARRAY (const CINDER_ARRAY& source) : vector< my_type > (source)
    {}

    virtual ~CINDER_ARRAY ()
    {}

    CINDER_ARRAY& operator= (const CINDER_ARRAY& source)
    {
      vector< my_type >::operator =(source);

      return *this;
    }

    my_type& operator[] (const INDEX index)
    {
      return vector< my_type >::operator [](index);
    }


    const my_type& operator[] (const INDEX index) const
    {
      return vector< my_type >::operator [](index);
    }


    const void clear ()
    {
      vector< my_type >::clear ();
    }


    void push_back (const my_type& item)
    {
      vector< my_type >::push_back (item);
    }


    void erase (const UINT32 index)
    {
      vector< my_type >::erase (vector< my_type >::begin () + index);
    }


    void Allocate (const COUNTER memory_units)
    {
      COUNTER index;

      CINDER_LOOP_THROUGH_INDEX (index, 0, memory_units)
      {
        vector< my_type >::push_back (my_type ());
      }
    }


    const UINT32 size () const
    {
      return vector< my_type >::size ();
    }

  };
}

#endif
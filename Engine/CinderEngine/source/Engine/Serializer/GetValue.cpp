/******************************************************************************/
/*!
\file   GetValue.cpp
\author Sagnik Chowdhury
\brief
This file contains the instantiations for the templated
GetValue function
*/
/******************************************************************************/
#ifndef GETVALUE_CPP

#define GETVALUE_CPP

#include "Serializer/JSONSerializer.h"

namespace Framework
{
  namespace Serializer
  {
    template <typename T>
    void GetValue(T* store, int keys, const char* ...)
    {

    }



    /////////////////////////////////////////////////////////////////////////////
    //Helper function
    /////////////////////////////////////////////////////////////////////////////
    DataNode* ZeroSerializer::FindElement(DataNode* list, const char* key)
    {
      DataNode* temp = list;
    
      while (temp && temp->objectName.compare(key) != 0)
      {
        temp = temp->next;
      }
    
      return temp;
    }
  } //namespace serializer
} //namespace framework

#endif

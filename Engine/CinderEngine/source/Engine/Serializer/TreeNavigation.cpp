/*****************************************************************************/
/*!
\file   TreeNavigation.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Constains functions to navigate around the tree
*/
/*****************************************************************************/

#include "Serializer/JSONSerializer.h"

namespace Framework
{
  namespace Serializer
  {
    DataNode* ZeroSerializer::FindElement(DataNode* branch, const char* key)
    {
      auto it = branch;
      std::string name(key);
      while (it && it->objectName.compare(name) != 0)
      {
        it = it->next;
      }
      return it;
    }

    DataNode* ZeroSerializer::FindStem(DataNode* current)
    {
      auto it = current;
      while (it && it->dataType != TYPE_OBJECT/* == it->previous->next*/)
      {
        it = it->previous;
      }
      return it;
    }

    void ZeroSerializer::DumpArchive(DataNode* stem, int indentation)
    {
      //int findent = indentation;
      DataNode* it = stem;
      while (it)
      {
        indent(indentation);
        std::cout << it->objectName << std::endl;
        if (it->dataType == TYPE_OBJECT)
        {
          DumpArchive(it->branch, indentation + 1);
        }

        it = it->next;
      }
    }

    DataNode* ZeroSerializer::GetValue(std::initializer_list<const char*> keys)
    {
      DataNode* walk = trunk;
      for (auto it = keys.begin(); it != keys.end(); ++it)
      {
        if (walk)
          walk = FindElement(walk, *it);
        else
        {
          std::cout << "Branch not found";
          return NULL;
        }
      }
      if (walk)
        return walk;
      else
        return NULL;
    }

    void ZeroSerializer::recursiveFree(DataNode* Obj)
    {
      while (Obj)
      {
        DataNode *temp = Obj->next;
        if (Obj->dataType == TYPE_OBJECT)
        {
          recursiveFree(Obj->branch);
        }
        delete Obj;
        Obj = temp;
      }
    }
  } // serializer
} // framework
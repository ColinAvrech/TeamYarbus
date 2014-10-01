/*****************************************************************************/
/*!
\file   Serializer.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The JSON serializer will parse JSON data files and return values.
*/
/*****************************************************************************/

#include "Serializer/JSONSerializer.h"

#include <cstdarg> //for variadic crap

namespace Framework
{
  namespace Serializer
  {
    ZeroSerializer::ZeroSerializer(const char* filename)
    {
      if (filename)
      {
        dataFile.open(filename);
      }
      
      //Create data type tree
      TypeList["int"]    = TYPE_INT;
      TypeList["uint"]   = TYPE_UINT;
      TypeList["long"]   = TYPE_LONG;
      TypeList["float"]  = TYPE_FLOAT;
      TypeList["double"] = TYPE_DOUBLE;
      TypeList["bool"]   = TYPE_BOOL;
      TypeList["string"] = TYPE_STRING;
      TypeList["Vec2"]   = TYPE_VEC2;
      TypeList["Vec3"]   = TYPE_VEC3;
      TypeList["Vec4"]   = TYPE_VEC4;
      TypeList["Enum"]   = TYPE_ENUM;

      trunk = NULL;
      CurrentNode = NULL;
      CurrentStem = NULL;
      inObject = 0;
    } //constructor JSONSerializer

    bool ZeroSerializer::open(const char* filepath)
    {
      if (dataFile.is_open())
      {
        dataFile.close();
      }
      dataFile.open(filepath);
      if (dataFile)
      {
        std::string dummy;
        //Skip first 2 lines
        std::getline(dataFile, dummy);
        std::getline(dataFile, dummy);
        ++inObject;
        trunk = AddNode(trunk, TYPE_OBJECT, "Level", 0);
        CurrentNode = CurrentStem = trunk;
        return true;
      }
      return false;
    } //function open

    void ZeroSerializer::close()
    {
      if (dataFile.is_open())
        dataFile.close();
    } //function close


    
    ZeroSerializer::~ZeroSerializer()
    {
      recursiveFree(trunk);
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

    void ZeroSerializer::ReadLine()
    {
      if (dataFile.is_open())
        std::getline(dataFile, CurrentLine);
    }

    void ZeroSerializer::ParseLine()
    {
      std::string currentname;
      std::vector<std::string> tokens = Tokenize();
      if (CurrentLine.back() == ' ')
      {
        //Entering new object
        currentname = tokens[0];
        
        ++inObject;
        CurrentNode->branch =  AddNode(CurrentNode->branch, TYPE_OBJECT, currentname.c_str(), 0);
        CurrentNode->next->previous = CurrentNode;
        CurrentStem = CurrentNode;
        CurrentNode = CurrentNode->branch;        
        //Skip the next line
        std::string dummy;
        std::getline(dataFile, dummy);
      }
      //if the line ends in a ','
      else if (CurrentLine.back() == ',')
      {
        //if the object has ended
        if (CurrentLine[CurrentLine.length() - 2] == '}')
        {
          //Close object
          --inObject;
        }
        //if it is a data field
        else
        {
          //Interpret data
        }        
      }
    }

    void ZeroSerializer::DumpArchive(DataNode* stem, int indentation)
    {
      int findent = indentation;
      DataNode* it = stem;
      while (it)
      {
        indent(findent);
        std::cout << it->objectName << std::endl;
        if (it->dataType == TYPE_OBJECT)
        {
          ++findent;
          DumpArchive(it->branch, findent);
        }

        it = it->next;
      }
    }

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
      while (it && it == it->previous->next)
      {
        it = it->previous;
      }
      return it;
    }


    /**************************************************************************/
    //Other helpers
    /**************************************************************************/
    //Split the line into seperate tokens
    std::vector<std::string> ZeroSerializer::Tokenize()
    {
      std::vector<std::string> tokens;
      std::size_t found = CurrentLine.find_first_not_of(" \t\n");
      std::size_t previous = found;

      unsigned int wordlength = CurrentLine.size();

      while (found != CurrentLine.npos)
      {
        found = CurrentLine.find_first_of(" \t\n", previous);
        std::string str;
        if (found >= wordlength)
          found = wordlength;
        for (unsigned int i = previous; i < found; ++i)
          str.push_back(CurrentLine[i]);
        previous = found;
        found = CurrentLine.find_first_not_of(" \t\n", previous);
        previous = found;
        if (str.size() > 0)
          tokens.push_back(str);
      }
      return tokens;
    }

    void ZeroSerializer::indent(int indentation)
    {
      for (int i = 0; i < indentation; ++i)
        std::cout << "  ";
    }

   
    

  } //namespace Serializer
} //namespace Framework
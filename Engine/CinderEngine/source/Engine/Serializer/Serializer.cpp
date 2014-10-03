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
    ZeroSerializer::ZeroSerializer()
    {      
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
      exitted = false;
    } //constructor JSONSerializer

    bool ZeroSerializer::open(const char* filepath)
    {
      if (dataFile.is_open())
      {
        dataFile.close();
      }
      dataFile.open(filepath);
      //if (dataFile)
      //{
        std::string dummy;
        //Skip first 2 lines
        std::getline(dataFile, dummy);
        std::getline(dataFile, dummy);
        ++inObject;
        trunk = AddNode(trunk, TYPE_OBJECT, "Level", 0);
        CurrentNode = CurrentStem = trunk;
        return true;
      //}
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

    void ZeroSerializer::CreateArchive()
    {
      for (int it = 0; inObject > 0; ++it)
      {
        ReadLine();
        ParseLine();
      }
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
        //Encountered new object
        currentname = tokens[0];
        if (!exitted) //Entering new object
        {
          ++inObject;
          CurrentNode->branch = AddNode(CurrentNode->branch, TYPE_OBJECT, currentname.c_str(), 0);
          CurrentNode->branch->previous = CurrentNode;
          CurrentStem = CurrentNode;
          CurrentNode = CurrentNode->branch;
          exitted = false;
        }
        else  //This is a new parallel object
        {
          ++inObject;
          CurrentNode->next = AddNode(CurrentNode->branch, TYPE_OBJECT, currentname.c_str(), 0);
          CurrentNode->next->previous = CurrentNode;
          CurrentNode = CurrentNode->next;
        }
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
          CurrentStem = FindStem(CurrentNode);
          CurrentNode = CurrentStem;
          CurrentStem = FindStem(CurrentNode);
          exitted = true;
        }
        //if it is a data field
        else
        {
          currentname = tokens[1];
          if (CurrentNode->dataType == TYPE_OBJECT)
          {
            CurrentNode->branch = AddNode(CurrentNode->branch, TYPE_INT, currentname.c_str(), 0);
            CurrentNode->branch->previous = CurrentNode;
            CurrentStem = CurrentNode;
            CurrentNode = CurrentNode->branch;
          }
          else
          {
            CurrentNode->next = AddNode(CurrentNode->next, TYPE_INT, currentname.c_str(), 20);
            CurrentNode->next->previous = CurrentNode;
            CurrentNode = CurrentNode->next;
          }
        }        
      }
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
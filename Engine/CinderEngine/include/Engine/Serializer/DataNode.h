/*****************************************************************************/
/*!
\file   DataNode.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The JSON serializer will parse JSON data files and return values.
*/
/*****************************************************************************/
//!make sure this is only included once
#ifndef DATANODE_H

#define DATANODE_H

#include <string>
#include <vector>

namespace Framework
{
  namespace Serializer
  {

    /////////////////////////////////////////////////////////////////
    enum TYPE
    {
      TYPE_NONE,
      TYPE_OBJECT,
      TYPE_UINT,
      TYPE_INT,
      TYPE_LONG,
      TYPE_FLOAT,
      TYPE_DOUBLE,
      TYPE_BOOL,
      TYPE_STRING,
      TYPE_VEC2,
      TYPE_VEC3,
      TYPE_VEC4,
      TYPE_ENUM,
      TYPE_CUSTOM
    }; //enum TYPE
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    typedef struct DynamicElement {
      DynamicElement(TYPE type, const char* name)
        : previous(NULL), next(NULL), branch(NULL),
        dataType(type), objectName(std::string(name)){};

      DynamicElement* previous;
      TYPE dataType;
      std::string objectName;
      union
      {
        int Int_;
        unsigned int UInt_;
        long Long_;
        float Float_;
        double Double_;
        bool Bool_;
        std::string* String_;
        std::vector<float>* VecN_; //2, 3 or 4
        std::string* Enum_;
        std::string* Custom_;
      } value_;
      DynamicElement* branch;
      DynamicElement* next;
      //Overloaded setter/////////////////////////////////////
      void SetValue(unsigned int& value);
      void SetValue(int& value);
      void SetValue(long& value);
      void SetValue(float& value);
      void SetValue(double& value);
      void SetValue(bool& value);
      void SetValue(std::string& value);
      void SetValue(std::vector<float>& value);
      ///////////////////////////////////////////////////////
    private:
      
    }DataNode;
  } //namespace serializer
} //namespace framework


#endif
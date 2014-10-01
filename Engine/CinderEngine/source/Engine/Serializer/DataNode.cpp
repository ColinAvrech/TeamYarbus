/*****************************************************************************/
/*!
\file   DataNode.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The overloaded functions for setting the value of a node.
*/
/*****************************************************************************/

#ifndef DATANODE_CPP

#define DATANODE_CPP

#include "Serializer/DataNode.h"

namespace Framework
{
  namespace Serializer
  {
    
    void DataNode::SetValue(unsigned int& value)
    {
      value_.UInt_ = value;
    }

    void DataNode::SetValue(int& value)
    {
      value_.Int_ = value;
    }

    void DataNode::SetValue(long& value)
    {
      value_.Long_ = value;
    }

    void DataNode::SetValue(float& value)
    {
      value_.Float_ = value;
    }

    void DataNode::SetValue(double& value)
    {
      value_.Double_ = value;
    }

    void DataNode::SetValue(bool& value)
    {
      value_.Bool_ = value;
    }
    
    void DataNode::SetValue(std::string& value)
    {
      value_.String_ = new std::string(value);
    }

    void DataNode::SetValue(std::vector<float>& value)
    {
      value_.VecN_ = new std::vector<float>(value);
    }

  }
}

#endif
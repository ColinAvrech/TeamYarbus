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
#include "GraphicsCommon.h"


namespace Framework
{
  namespace Serializer
  {
    //////////////////////////////////////////////////////////////////
    //SetValue
    //////////////////////////////////////////////////////////////////
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

    //////////////////////////////////////////////////////////////////////////
    //GetValue
    //////////////////////////////////////////////////////////////////////////
    void DataNode::GetValue(unsigned int* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.UInt_;
    }
    void DataNode::GetValue(int* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.Int_;
    }
    void DataNode::GetValue(long* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.Long_;
    }
    void DataNode::GetValue(float* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.Float_;
    }
    void DataNode::GetValue(double* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.Double_;
    }
    void DataNode::GetValue(bool* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.Bool_;
    }
    void DataNode::GetValue(std::string* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      store = value_.String_;
    }
    void DataNode::GetValue(glm::vec2* store)
    {
      ErrorIf(value_.VecN_->size() < 2, "Wrong data type");
      store->x = (*value_.VecN_)[0];
      store->y = (*value_.VecN_)[1];
    }
    void DataNode::GetValue(glm::vec3* store)
    {
      ErrorIf(value_.VecN_->size() < 3, "Wrong data type");
      store->x = (*value_.VecN_)[0];
      store->y = (*value_.VecN_)[1];
      store->y = (*value_.VecN_)[2];
    }
    void DataNode::GetValue(glm::vec4* store)
    {
      ErrorIf(value_.VecN_->size() < 4, "Wrong data type");
      store->x = (*value_.VecN_)[0];
      store->y = (*value_.VecN_)[1];
      store->y = (*value_.VecN_)[2];
      store->z = (*value_.VecN_)[3];
    }
    /////////////////////////////////////////////////////////////////
  }
}

#endif
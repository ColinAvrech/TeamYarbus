/******************************************************************************/
/*!
\file   DragEffect.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include "Component.h"
#include "DataNode.h"

namespace Framework
{
  class DragEffect : public Component
  {
  public:
    DragEffect(){};
    ~DragEffect(){};
    //Properties==================================
    bool Active;
    bool WakeUpOnChange;
    float Drag;

    //virtual member functions===================
    /*!Telegraph that the component is active*/
    virtual void Initalize();

    /*!TODO IMPLIMENT SERIALIZATION!*/
    virtual void Serialize(Serializer::DataNode* data);


    // The non-base component uses DefineComponentName macro to name component
    const static std::string Name;

  private:

  };
} //Framework
/******************************************************************************/
/*!
\file   GravityEffect.h
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include "Common.h"
#include "Component.h"
#include "glm.hpp"
#include "DataNode.h"

namespace Framework
{
  class GravityEffect : public Component
  {
  public:
    GravityEffect(){};
    ~GravityEffect(){};
    //Properties==================================
    bool Active;
    bool WakeUpOnChange;
    bool LocalSpace;
    float Strength;      //scale gravity vector by this
    glm::vec3 Direction; //not normalized

    //virtual member functions===================
    /*!Telegraph that the component is active*/
    virtual void Initialize();

    /*!TODO IMPLIMENT SERIALIZATION!*/
    virtual void Serialize(Serializer::DataNode* data);
    

    // The non-base component uses DefineComponentName macro to name component
    const static std::string Name;

  private:
    
  };
} //Framework
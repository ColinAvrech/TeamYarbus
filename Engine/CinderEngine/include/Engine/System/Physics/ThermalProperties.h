/******************************************************************************/
/*!
\file   ThermalProperties.h
\author Sagnik Chowdhury
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
  class ThermalProperties : public Component
  {
  public:
    float T;       //Temperature in K
    float Hc;      //Heat transfer coefficient
    float c;       //Specific heat

  private:

  };
}




/******************************************************************************/
/*!
\file   Combustible.h
\author Sagnik Chowwdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains the combustion properties of a given object.
*/
/******************************************************************************/

/*
All the crap in this file will require further research in thermodynamics
before logic is implemented.
*/

#pragma once

#include "Common.h"

namespace Framework
{
  class Combustion_Properties : public GameComponent
  {
  public:
    Combustion_Properties(GOC* obj);
    ~Combustion_Properties();

  private:
    //The total amount of fuel left in the object
    float fuel;
    //The temperature at which the material auto ignites
    float critical_temp;
    //The temperature at which the material is hot enough for a fire particle
    //to ignite it.
    float ignition_temp;
    //The rate at which the material burns
    float burn_rate;
    //The rate of absorption/release of heat
    float heat_transfer_coefficient;
    //pointer to object's collider
    //--
    //vector of ignition particles
    //--
  };
}
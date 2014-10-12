/******************************************************************************/
/*!
\file   TDLib.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/
#pragma once

#include "type_vec.hpp"

namespace Framework
{
  namespace Physics
  {
    namespace Constant
    {
      //Add physics constants here
      long c   = 299792458;  //m.s-1       | Speed of light in vaccum
      long atm = 101325;     //Pa          | Standard Atmospheric pressure
      long R   = 8.31451;    //J.mol-1.K-1 | Molar gas constant
      int  M_a = 28.97;      //Kg.Kmol-1   | Molar mass of air
    } //namespace Constant

    float ComputeBuoyancy(float density, float mass);
    float ComputeDensity();
    float DiffuseHeat();
    float DiffuseDensity();
    glm::vec2 ComputeAdvection();

  } //namespace Physics
} //namespace Framework
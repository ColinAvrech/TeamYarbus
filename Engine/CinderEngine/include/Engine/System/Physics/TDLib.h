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
      static const long  c = 299792458;  //m.s-1       | Speed of light in vaccum
      static const long  atm = 101325;     //Pa          | Standard Atmospheric pressure
      static const float R = 8.31451;    //J.mol-1.K-1 | Molar gas constant
      static const int   M_a = 28.97;      //Kg.Kmol-1   | Molar mass of air

      //Properties of specific materials--------------------
      //Specific heat
      static const float c_Air = 1046;
      static const float c_Water = 4186;
      static const float c_Soil = 1046;
      static const float c_Stone = 858;
      static const float c_Iron = 452;
      static const float c_Steel = 452;
      static const float c_Lead = 128;
      static const float c_Wood = 1674;

      //Thermal Conductivity
      static const float K_Air = 0.024f;
      static const float K_Wood = 0.048f;
      static const float K_drySoil = 1.5f;
      static const float K_Bronze = 110.f;
      static const float K_Carbon = 1.7f;
      static const float K_cotton = 0.029f;
      static const float K_Cement = 1.73f;
      static const float K_Stone = 1.7f;
      static const float K_Copper = 401.f;

      //Heat transfer coefficient
      static const float Hc_Air;
    } //namespace Constant

    enum Material
    {
      AIR,
      WATER,
      SOIL,
      STONE,
      LEAD,
      IRON,
      STEEL,
      WOOD,
      COTTON
    };

    float Buoyancy(float density, float mass, float CellSize);
    float ConductiveHeatTransfer(float K, float T1, float T2, const double dt, float Size);
    float ConvectiveHeatTransfer(float Hc, float T1, float T2, const double dt);
    float dTemp(float dQ, float m, float c);
    float DiffuseDensity();

  } //namespace Physics
} //namespace Framework
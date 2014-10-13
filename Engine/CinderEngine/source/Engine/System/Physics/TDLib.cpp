/******************************************************************************/
/*!
\file   TDLib.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
*/
/******************************************************************************/

#include "TDLib.h"
#include "PhysicsLibrary.h"

namespace Framework
{
  namespace Physics
  {
    //Acceleration caused by buoyant force
    float Buoyancy(float density, float mass, float CellSize)
    {
      float Fb = density * Constant::g * CellSize * CellSize * CellSize;
      return Fb / mass;
    }
    //Heat transfer by conduction
    float ConductiveHeatTransfer(float K, float T1, float T2, const double dt, float Size)
    {
      float dTemp = T2 - T1;
      float dQ = K * dTemp * dt;
      return dQ;
    }
    //Heat transfer by radiation
    float ConvectiveHeatTransfer(float Hc, float T1, float T2, const double dt)
    {
      float dTemp = T2 - T1;
      float dQ = Hc * dTemp * dt;
      return dQ;
    }
    //Change of temperature caused by heat transfer
    float dTemp(float dQ, float m, float c)
    {
      float deltaT = dQ / (m * c);
      return deltaT;
    }
  }
}
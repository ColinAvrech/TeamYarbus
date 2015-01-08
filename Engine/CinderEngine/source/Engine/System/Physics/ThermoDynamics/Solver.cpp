/******************************************************************************/
/*!
\file   FluidSolver.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  NOT USED IN F2014 build
*/
/******************************************************************************/
#include <Precompiled.h>

#define VO(x1, x2) y_offset[x1] - y_offset[x2]

namespace Framework
{
  namespace Physics
  {
    namespace Const = Constant;

    void ThermodynamicsSystem::diffuse(int start, int end, const float dt)
    {
      float _dt = simulation_speed * dt;

      for (int j = 0; j < MapSize.y; ++j)
      {
        for (int i = start; i < end; ++i)
        {
          float netdQ = 0.f;
          //Loop through surrounding cells
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              int _y = y + y_offset[i] - y_offset[x];
              if ((x == i - 1 && _y == j) ||
                (x == i + 1 && _y == j) ||
                (x == i && _y == j - 1) ||
                (x == i && _y == j + 1))
              {
                if (x < MapSize.x && x >= 0 && _y < MapSize.y && _y >= 0)
                {
                  float dQ = ConductiveHeatTransfer(materialList[Terrain.Get(i, j)].K,
                    TemperatureMap_Prev.Get(i, j), TemperatureMap_Prev.Get(x, _y), _dt, 1.0f);
                  netdQ += dQ;
                }
                else
                {
                  float dQ = ConductiveHeatTransfer(materialList[Terrain.Get(i, j)].K,
                    TemperatureMap_Prev.Get(i, j), AtmosphericTemperature, _dt, 1.0f);
                  netdQ += dQ;
                }
              }
            }
          }
          //Set calculated temperature
          TemperatureMap.Set(i, j, TemperatureMap_Prev.Get(i, j) + dTemp(netdQ, DensityMap.Get(i, j) * 1.0f, Const::c_Air));
        }//for i
      }//for j
    } //function diffuse

    void ThermodynamicsSystem::advect(int start, int end, const float dt)
    {

    }

    //Update temperatures
    void ThermodynamicsSystem::temp_step(const int& start_index, const int& end_index, const float &dt)
    {
      //swap current with previous
      TemperatureMap.Swap(TemperatureMap_Prev);
      //diffuse heat
      diffuse(start_index, end_index, dt);
      //move heat
      advect(start_index, end_index, dt);
    }//function Update temp


    //Update velocity vectors
    void ThermodynamicsSystem::vel_step(const int& start_index, const int& end_index, const float& dt)
    {
      VelocityMapX.Swap(VelocityMap_PrevX);
      VelocityMapY.Swap(VelocityMap_PrevY);


    }
  } //namespace physics
} //namespace framework

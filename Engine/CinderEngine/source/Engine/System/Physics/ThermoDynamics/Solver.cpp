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
#define FOR(s, e) for (int j = 0; j < MapSize.y; ++j) { for (int i = start; i < end; ++i) {
#define END_FOR }}

namespace Framework
{
  namespace Physics
  {
    namespace Const = Constant;

    void ThermodynamicsSystem::diffuse(int start, int end, const float dt)
    {
      float _dt = simulation_speed * dt;

      FOR(start, end)
          float netdQ = 0.f;
          //Loop through surrounding cells
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              int _y = y + y_offset[i] - y_offset[x];
              //only adjacent cells
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
                else //boundary condittion
                {
                  float dQ = ConductiveHeatTransfer(materialList[Terrain.Get(i, j)].K,
                    TemperatureMap_Prev.Get(i, j), AtmosphericTemperature, _dt, 1.0f);
                  netdQ += dQ;
                }
              } //update only directly adjacent cells
          END_FOR
          //Set calculated temperature
          TemperatureMap.Set(i, j, TemperatureMap_Prev.Get(i, j) + dTemp(netdQ, DensityMap.Get(i, j) * 1.0f, Const::c_Air));
      END_FOR
    } //function diffuse

    void ThermodynamicsSystem::advect(Grid2D<float> &g, Grid2D<float> &g0, Grid2D<float> &u, Grid2D<float> &v, int start, int end, const float dt)
    {

    }

    void ThermodynamicsSystem::project(int start, int end)
    {

    }

    void ThermodynamicsSystem::release_pressure(int start, int end, const float dt)
    {
      int h = MapSize.y * MapSize.y;
      float a = dt * viscosity * h;
      float c = 1 + 4 * a;

      FOR(start, end)
          float net_vel_x = 0.f;
          float net_vel_y = 0.f;
          //Loop through surrounding cells
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              int _y = y + y_offset[i] - y_offset[x];
              //only adjacent cells
              if ((x == i - 1 && _y == j) ||
                (x == i + 1 && _y == j) ||
                (x == i && _y == j - 1) ||
                (x == i && _y == j + 1))
              {
                if (x < MapSize.x && x >= 0 && _y < MapSize.y && _y >= 0)
                {
                  net_vel_x += VelocityMapX.Get(x, _y);
                  net_vel_y += VelocityMapY.Get(x, _y);
                }
              } //update only directly adjacent cells
          END_FOR
          float res_vel_x = (VelocityMap_PrevX.Get(i, j) + a * net_vel_x) / c;
          VelocityMapX.Set(i, j, res_vel_x);
          float res_vel_y = (VelocityMap_PrevY.Get(i, j) + a * net_vel_y) / c;
          VelocityMapY.Set(i, j, res_vel_y);
      END_FOR
    }

    //Update temperatures
    void ThermodynamicsSystem::temp_step(const int& start_index, const int& end_index, const float &dt)
    {
      //swap current with previous
      TemperatureMap.Swap(TemperatureMap_Prev);
      //diffuse heat
      diffuse(start_index, end_index, dt);
      //swap current with previous
      //TemperatureMap.Swap(TemperatureMap_Prev);
      //move heat
      advect(TemperatureMap, TemperatureMap_Prev, VelocityMapX, VelocityMapY, start_index, end_index, dt);
    }//function Update temp


    //Update velocity vectors
    void ThermodynamicsSystem::vel_step(const int& start_index, const int& end_index, const float& dt)
    {
      //Swap arrays
      VelocityMapX.Swap(VelocityMap_PrevX);
      VelocityMapY.Swap(VelocityMap_PrevY);
      //diffuse velocity if EqualizePressure is true
      if (EqualizePressure)
      {
        release_pressure(start_index, end_index, dt);
      }
      //compute total applied forces
      project(start_index, end_index);
      //Swap arrays
      //VelocityMapX.Swap(VelocityMap_PrevX);
      //VelocityMapY.Swap(VelocityMap_PrevY);
      //advect wind velocities
      //no need for now
    }
  } //namespace physics
} //namespace framework

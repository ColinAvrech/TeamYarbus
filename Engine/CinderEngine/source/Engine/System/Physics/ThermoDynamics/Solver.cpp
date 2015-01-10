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
#define FOR(s, e) for (int j = 2; j < MapSize.y - 1; ++j) { for (int i = start + 1; i < end - 1; ++i) {
#define END_FOR }}

namespace Framework
{
  namespace Physics
  {
    namespace Const = Constant;

    void ThermodynamicsSystem::lin_solve(int start, int end, int b, Grid2D<float> &x1, Grid2D<float> &x0, float a, float c)
    {
      FOR(start, end)
        float net_vel_x = 0.f;
        //Loop through surrounding cells
        for (int y = j - 1; y <= j + 1; ++y)
        {
          for (int x = i - 1; x <= i + 1; ++x)
          {
            int _y = y + VO(i, x);
            //only adjacent cells
            if ((x == i - 1 && _y == j) ||
              (x == i + 1 && _y == j) ||
              (x == i && _y == j - 1) ||
              (x == i && _y == j + 1))
            {
              if (x < MapSize.x && x >= 0 && _y < MapSize.y && _y >= 0)
              {
                net_vel_x += x1.Get(x, _y);
              }
            } //update only directly adjacent cells
        END_FOR
        float res_vel_x = (x0.Get(i, j) + a * net_vel_x) / c;
        x1.Set(i, j, res_vel_x);
        END_FOR

        set_bnd(start, end, b, x1);
    }

    void ThermodynamicsSystem::diffuse(int start, int end, const float dt)
    {
      float _dt = simulation_speed * dt;

      for (int j = 0; j < MapSize.y; ++j) {
        for (int i = start; i < end; ++i) {
          float netdQ = 0.f;
          //Loop through surrounding cells
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              int _y = y + VO(i, x);
              //only adjacent cells
              if ((x == i - 1 && _y == j - 1) ||
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

    void ThermodynamicsSystem::convect(int start, int end, const float dt)
    {
      float _dt = simulation_speed * dt;
      FOR(start, end)
        int dy = 0;
        if (materialList[Terrain.Get(i, j)].isFluid && materialList[Terrain.Get(i, j + 1)].isFluid)
        {
          VelocityMapY.Set(i, j, VelocityMapY.Get(i, j) + (TemperatureMap_Prev.Get(i, j) - TemperatureMap_Prev.Get(i, j + 1)) / 300.f);
          dy = 1;
        }
        
        int dx = 0;// VelocityMapX.Get(i, j) / 10;
        float dQConv;
        if (i + dx >= 0 && i + dx < MapSize.x && j + dy + VO(i, i + dx) >= 0 && j + dy + VO(i, i + dx) < MapSize.y)
        {
          dQConv = ConvectiveHeatTransfer(materialList[Terrain.Get(i, j)].Hc,
            TemperatureMap_Prev.Get(i, j), TemperatureMap_Prev.Get(i + dx, j + dy), _dt);
        }
        else
        {
          dQConv = ConvectiveHeatTransfer(materialList[Terrain.Get(i, j)].Hc,
            TemperatureMap_Prev.Get(i, j), AtmosphericTemperature, _dt);
        }
        TemperatureMap.Set(i, j, TemperatureMap_Prev.Get(i, j + 1) + dTemp(dQConv, DensityMap.Get(i, j + 1) * 1.0f, Const::c_Air));
      END_FOR
    }

    void ThermodynamicsSystem::advect(Grid2D<float> &g, Grid2D<float> &g0, Grid2D<float> &u, Grid2D<float> &v, int start, int end, int b, const float dt)
    {
      //float dt0 = (MapSize.y - 2) * simulation_speed * dt;
      float dt0 = 16 * simulation_speed * dt;
      FOR(start, end)
        float x = i - dt0 * VelocityMapX.Get(i, j);
        float y = j - dt0 * VelocityMapY.Get(i, j);

        if (x < 0.5f) x = 0.5f; if (x > MapSize.y - 2 + 0.5f) x = MapSize.y + 0.5f;
        int i0 = (int)x; int i1 = i0 + 1;

        if (y < 0.5f) y = 0.5f; if (y > MapSize.y - 2 + 0.5f) y = MapSize.y + 0.5f;
        int j0 = (int)y; int j1 = j0 + 1;

        float s1 = x - i0; float s0 = 1 - s1; float t1 = y - j0; float t0 = 1 - t1;

        if (j0 < 0) j0 = 0; if (j1 < 0) j1 = 0;

        float sum = s0 * (t0 * g0.Get(i0, j0) + t1 * g0.Get(i0, j1)) +
          s1 * (t0 * g0.Get(i1, j0) + t1 * g0.Get(i1, j1));

        g.Set(i, j, sum);
        END_FOR

        set_bnd(start, end, b, g);
    }

    void ThermodynamicsSystem::project(int start, int end)
    {
      //p = ux0, div = uy0
      //u = ux, v = uy
      
      FOR(start, end)
        float div = 0.f;
        int m = -1; //for changing sign
        
        //i+1, j
        int _j = j + VO(i, i + 1);
        if (_j > 0 && _j < MapSize.y)
          div += VelocityMapX.Get(i + 1, _j);

        _j = j + VO(i, i - 1);
        if (_j > 0 && _j < MapSize.y)
          div -= VelocityMapX.Get(i - 1, _j);

        if (j + 1 < MapSize.y)
          div += VelocityMapY.Get(i, j + 1);

        if (j - 1 > 0)
          div -= VelocityMapY.Get(i, j - 1);

        float pre_vy = -0.5f * div / (16 - 2);
        VelocityMap_PrevY.Set(i, j, pre_vy);
        VelocityMap_PrevX.Set(i, j, 0.f);
        END_FOR

        set_bnd(start, end, 0, VelocityMap_PrevY);
        set_bnd(start, end, 0, VelocityMap_PrevX);

      lin_solve(start, end, 0, VelocityMap_PrevX, VelocityMap_PrevY, 1, 4);

      FOR(start, end)
        int _j = j + VO(i, i + 1);
        float x_sum = 0.f;
        float y_sum = 0.f;
        //x velocity
        if (_j > 0 && _j < MapSize.y)
          x_sum += VelocityMap_PrevX.Get(i + 1, _j);

        _j = j + VO(i, i - 1);

        if (_j > 0 && _j < MapSize.y)
          x_sum -= VelocityMap_PrevX.Get(i - 1, _j);
        
        float val_x = 0.5f * (16 - 2) * x_sum;
        VelocityMapX.Set(i, j, VelocityMapX.Get(i, j) - val_x);

        //y velocity
        if (j + 1 < MapSize.y)
          y_sum += VelocityMap_PrevX.Get(i, j + 1);
        if (j - 1 > 0)
          y_sum -= VelocityMap_PrevX.Get(i, j - 1);

        float val_y = 0.5f * (16 - 2) * y_sum;
        VelocityMapY.Set(i, j, VelocityMapY.Get(i, j) - val_y);
        END_FOR

        set_bnd(start, end, 1, VelocityMapX); set_bnd(start, end, 2, VelocityMapY);
    }

    void ThermodynamicsSystem::release_pressure(int start, int end, const float dt)
    {
      //int h = MapSize.y * MapSize.y;
      int h = 16 * 16;
      float a = simulation_speed * dt * viscosity * h;
      float c = 1 + 4 * a;

      lin_solve(start, end, 1, VelocityMapX, VelocityMap_PrevX, a, c);
      lin_solve(start, end, 2, VelocityMapY, VelocityMap_PrevY, a, c);
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
      convect(start_index, end_index, dt);
      //advect(TemperatureMap, TemperatureMap_Prev, VelocityMapX, VelocityMapY, start_index, end_index, dt);
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
      VelocityMapX.Swap(VelocityMap_PrevX);
      VelocityMapY.Swap(VelocityMap_PrevY);
      //advect wind velocities
      advect(VelocityMapX, VelocityMap_PrevX, VelocityMap_PrevX, VelocityMap_PrevY, start_index, end_index, 1, dt);
      advect(VelocityMapY, VelocityMap_PrevY, VelocityMap_PrevX, VelocityMap_PrevY, start_index, end_index, 2, dt);

      //project(start_index, end_index);
      //lin_solve(start_index, end_index, 0, VelocityMap_PrevX, VelocityMap_PrevY, 1, 4);
      lin_solve(start_index, end_index, 0, VelocityMapX, VelocityMapY, 1, 4);
      //Swap arrays
      VelocityMapX.Swap(VelocityMap_PrevX);
      VelocityMapY.Swap(VelocityMap_PrevY);
      
      //project(start_index, end_index);
      //lin_solve(start_index, end_index, 0, VelocityMapX, VelocityMapY, 1, 4);
    }

    void ThermodynamicsSystem::set_bnd(int start, int end, int b, Grid2D<float> &x)
    {
      for (int i = start + 1; i < end - 1; ++i) {
        x.Set(i, 1, b == 2 ? -x.Get(i, 1) : x.Get(i, 1));
        x.Set(i, MapSize.y - 1, b == 2 ? -x.Get(i, MapSize.y - 2) : x.Get(i, MapSize.y - 2));
      }

      for (int i = 1; i < MapSize.y - 1; ++i)
      {
        x.Set(start, i, b == 1 ? -x.Get(1, i) : x.Get(1, i));
        x.Set(end - 1, i, b == 1 ? -x.Get(end - 2, i) : x.Get(end - 2, i));
      }

      x.Set(start, 0, 0.5f * (x.Get(start + 1, 0) + x.Get(start, 1)));
      x.Set(start, MapSize.y - 1, 0.5f * (x.Get(start + 1, MapSize.y - 1) + x.Get(start, MapSize.y - 2)));
      x.Set(end - 1, 0, 0.5f * (x.Get(end - 2, 0) + x.Get(end - 1, 1)));
      x.Set(end - 1, MapSize.y - 1, 0.5f * (x.Get(end - 2, MapSize.y - 1) + x.Get(end - 1, MapSize.y - 2)));
    }
  } //namespace physics
} //namespace framework

#include "Physics/Thermodynamics.h"
#include "TDLib.h"
#include "AudioSystem.h"
#include "TerrainCreator.h"
#include "FractalNoise.h"
#include "ThreadFunctions.h"
#include "solver.c"

#define SIZE 10

namespace Framework
{
  namespace Physics
  {
    glm::ivec2 ThermodynamicsSystem::MapSize;

    //!Null untill the ObjectSystem has been created
    ThermodynamicsSystem * THERMODYNAMICS = NULL;
    namespace Const = Constant;
    //Constructor
    ThermodynamicsSystem::ThermodynamicsSystem ()
    {
      //Do stuff
      CellSize = 0.1f;
      THERMODYNAMICS = this;
      MapSize = { 100, 100 };
    }

    //Destructor
    ThermodynamicsSystem::~ThermodynamicsSystem ()
    {
      ReleaseThreads ();
    }

    /*-----------------------------------------------------------------------
    // Public Functions
    -----------------------------------------------------------------------*/

    bool ThermodynamicsSystem::Initialize ()
    {
      std::cout << "Thermodynamics Initialized." << std::endl;

      //Scan level
      MapSize = { 128, 128 };
      std::cout << "Grid " << MapSize.x << "x " << MapSize.y << std::endl;
      MapOffset = { 50, 50 };
      AtmosphericTemperature = 300.f;
      //Allocate heatmap
      TemperatureMap.allocate (MapSize.x, MapSize.y);
      TemperatureMap.fill(300.f);

      //Allocate Oxygen/Density map
      DensityMap.allocate (MapSize.x + 2, MapSize.y + 2);
      DensityMap_Prev.allocate (MapSize.x + 2, MapSize.y + 2);
      DensityMap.fill (Constant::K_Air);
      DensityMap_Prev.fill(Constant::K_Air);

      //Allocate Velocity map
      VelocityMapX.allocate (MapSize.x + 2, MapSize.y + 2);
      VelocityMapY.allocate (MapSize.x + 2, MapSize.y + 2);
      VelocityMap_PrevX.allocate (MapSize.x + 2, MapSize.y + 2);
      VelocityMap_PrevY.allocate (MapSize.x + 2, MapSize.y + 2);
      VelocityMapX.fill ({ 0 });
      VelocityMapY.fill ({ 0 });
      VelocityMap_PrevX.fill ({ 0 });
      VelocityMap_PrevY.fill ({ 0 });

      //Allocate Terrain map
      Terrain.allocate(MapSize.x, MapSize.y);
      Terrain.fill(AIR);

      WaterMap.allocate(MapSize.x, MapSize.y);
      WaterMap.fill(0.0f);

      //Allocate Fire map
      FireMap.allocate(MapSize.x, MapSize.y);
      FireMap.fill(0.0f);

      //Allocate Fuel map
      FuelMap.allocate(MapSize.x, MapSize.y);
      FuelMap.fill(0.0f);

      SpawnThreads ();

      return true;
    }

    // Called every frame
    void ThermodynamicsSystem::Update (const double& dt)
    {
      UpdateMultiThreaded ();
      solver.vel_step
        (
        MapSize.x,
        VelocityMapX.GetArray(), VelocityMapY.GetArray(),
        VelocityMap_PrevX.GetArray(), VelocityMap_PrevY.GetArray(),
        0.0f,
        0.1f
        );

      solver.dens_step
        (
        MapSize.x,
        DensityMap.GetArray(), DensityMap_Prev.GetArray(),
        VelocityMapX.GetArray(), VelocityMapY.GetArray(),
        0.0f,
        0.1f
        );
    }

    // Getters
    //Get cell temperature
    float ThermodynamicsSystem::GetCellTemperature (const float& x, const float& y)
    {
      vec2 sub = GetSubscript(x, y);
      int sub_x = int (sub.x);
      int sub_y = int (sub.y);
      if (sub_x < 0 || sub_x >= MapSize.x || sub_y < 0 || sub_y >= MapSize.y)
        return 0.f;
      return TemperatureMap.Get(sub_x, sub_y);
    }

    //Get cell oxygen content
    float ThermodynamicsSystem::GetCellOxygen (const float& x, const float& y)
    {
      vec2 sub = GetSubscript(x, y);
      int sub_x = int(sub.x);
      int sub_y = int(sub.y);
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
        return 0.f;
      return DensityMap.Get(sub_x, sub_y);
    }
    //Get cell velocity
    vec2 ThermodynamicsSystem::GetCellVelocity(const float& x, const float& y)
    {
      vec2 sub = GetSubscript(x, y);
      int sub_x = int(sub.x);
      int sub_y = int(sub.y);
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
      {
        return vec2 (0, 0);
      }
      return vec2 (VelocityMapX.Get(sub_x, sub_y), VelocityMapY.Get(sub_x, sub_y));
    }

    // Setters
    void ThermodynamicsSystem::ToggleAutoDissipation ()
    {
      EqualizePressure = !EqualizePressure;
    }

    float ThermodynamicsSystem::SetCellTemperature(const float& x, const float& y, const float& temp, const double& dt)
    {
      vec2 sub = GetSubscript(x, y);
      int sub_x = int(sub.x);
      int sub_y = int(sub.y);
      float dQ;
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
        dQ = ConductiveHeatTransfer (Const::K_Wood, AtmosphericTemperature, temp, dt, 1);
      else
      {
        dQ = ConductiveHeatTransfer (Const::K_Air, TemperatureMap.Get(sub_x, sub_y), temp, dt, 1);
        float deltaTemp = dTemp (dQ, DensityMap.Get(sub_x, sub_y) * 0.001f, /*Const::c_Air*/100);
        TemperatureMap.Set(sub_x, sub_y, TemperatureMap.Get(sub_x, sub_y) + deltaTemp);
      }
      return dQ;
    }

    void ThermodynamicsSystem::SetCellVelocity(const int x, const int y, vec2 v)
    {
      if (x < 0 || x > MapSize.x || y < 0 || y > MapSize.y)
        return;
      VelocityMapX.Set (x, y, v.x);
      VelocityMapY.Set (x, y, v.y);
    }

    /*-----------------------------------------------------------------------
    // Private Functions
    -----------------------------------------------------------------------*/

    //Update temperatures
    void ThermodynamicsSystem::UpdateTemp(const int& start_index, const int& end_index, const double& dt)
    {
      //std::cout << start_index << "\n";
      //std::cout << "Updated Temperature/Density/Pressure" << std::endl;
      for (int j = start_index; j < end_index; ++j)
      {
        for (int i = 0; i < 100; ++i)
        {
          float netdQ = 0.f;
          float oTemp = TemperatureMap.Get (i, j);
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              if (x != i || y != j)
              {
                if (x < MapSize.x && x >= 0 && y < MapSize.y && y >= 0)
                {
                  float dQ = ConductiveHeatTransfer (Const::K_Air, TemperatureMap.Get (i, j), TemperatureMap.Get (x, y), dt, 0.1f);
                  netdQ += dQ;
                  float oTemp = TemperatureMap.Get (x, y);
                  TemperatureMap.Set (x, y, TemperatureMap.Get (x, y) - dTemp (dQ, DensityMap.Get (x, y) * 0.001f, Const::c_Air));

                  float factor = TemperatureMap.Get (x, y) / oTemp;
                  //DensityMap.Set (x, y, DensityMap.Get (x, y) / factor);
                }
                else
                {
                  float dQ = ConductiveHeatTransfer (Const::K_Air, TemperatureMap.Get(i, j), AtmosphericTemperature, dt, 0.1f);
                  netdQ += dQ;
                }
              }
            }
          }
          if (j + 1 <= MapSize.y)
          {
            if (Terrain.Get(i, j) == AIR && Terrain.Get(i, j + 1) == AIR)
            {
              float dQConv = ConvectiveHeatTransfer (Const::Hc_Air, TemperatureMap.Get(i, j), TemperatureMap.Get(i, j + 1), dt);
              float oTempConv = TemperatureMap.Get(i, j + 1);
              netdQ += dQConv;
              TemperatureMap.Set(i, j + 1, TemperatureMap.Get(i, j + 1) - dTemp (dQConv, DensityMap.Get(i, j + 1) * 0.001f, Const::c_Air));
              float factor2 = TemperatureMap.Get(i, j + 1) / oTempConv;
              //DensityMap.Set(i, j + 1, DensityMap.Get(i, j) / factor2);
            }
          }
          else
          {
            if (Terrain.Get(i, j) == 0 && Terrain.Get(i, j + 1) == 0)
            {
              float dQConv = ConvectiveHeatTransfer (Const::Hc_Air, TemperatureMap.Get(i, j), AtmosphericTemperature, dt);
              netdQ += dQConv;
            }
          }
          TemperatureMap.Set(i, j, TemperatureMap.Get(i, j) + dTemp (netdQ, DensityMap.Get(i, j) * 0.001f, Const::c_Air));
          float factor1 = TemperatureMap.Get(i, j) / oTemp;
          //DensityMap.Set(i, j, DensityMap.Get(i, j) / factor1);
        }//for
      }//for
    }//function

    //Update velocity vectors
    void ThermodynamicsSystem::ComputeVelocity(const int& start_index, const int& end_index, const double& dt)
    {
      //vec2 dirvec [8] = {
      //  { -1, -1 },
      //  { 0, -1 },
      //  { 1, -1 },
      //  { -1, 0 },
      //  { 1, 0 },
      //  { -1, 1 },
      //  { 0, 1 },
      //  { 1, 1 }
      //};
      //for (int j = start_index; j < end_index; ++j)
      //{
      //  for (int i = 1; i < 100; ++i)
      //  {
      //    float dSum = 0.f;
      //    for (int y = j - 1; y <= j + 1; ++y)
      //    {
      //      for (int x = i - 1; x <= i + 1; ++x)
      //      {
      //        dSum += DensityMap->Get(x, y);
      //      }
      //    }
      //    float meanDensity = dSum / 8;
      //    float buoyancy = Buoyancy (meanDensity, DensityMap->Get(i, j), 1.f);

      //    int vectorindex = 0;
      //    float dDenseSum = 0.f;
      //    VelocityMap->Set(i, j, { 0, 0 });
      //    for (int y = j - 1; y <= j + 1; ++y)
      //    {
      //      for (int x = i - 1; x <= i + 1; ++x)
      //      {
      //        if (x != i || y != j)
      //        {
      //          float dDense = DensityMap->Get(x, y) - DensityMap->Get(i, j);
      //          VelocityMap->Set(i, j, VelocityMap->Get(i, j) - (dirvec [vectorindex] * (dDense / 8)));
      //          //if (EqualizePressure)
      //          //{
      //          //OxygenMap[x][y] += (dDense / 8) * (float)dt;
      //          dDenseSum += (dDense / 8);
      //          //}
      //          ++vectorindex;
      //        }

      //      } //for x
      //    } //for y
      //    //OxygenMap[i][j] -= dDenseSum * (float)dt;
      //    VelocityMap->Get(i, j) += (vec2 (0, 1) * buoyancy);
      //  }//for i
      //} //for j
    }

    //Update fire
    void ThermodynamicsSystem::UpdateFire(const int& start_index, const int& end_index, const double& dt)
    {
      //std::cout << "Updated Fire" << std::endl;
      for (int j = start_index; j < end_index; ++j)
      {
        for (int i = 1; i < 100; ++i)
        {
          int OxyCount = 0;
          float OxyAmount = 0.f;
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              if ((x != i || y != j) && Terrain.Get(i, j) == 0)
              {
                ++OxyCount;
                //OxyAmount += OxygenMap[x][y] * CellSize*CellSize*CellSize;
              }
            } //for x
          } //for y
          float tempRange = Const::BT_Organics - Const::IT_Wood;
          float tempDiff = TemperatureMap.Get(i, j) - Const::IT_Wood;
          float tempFactor = tempDiff / tempRange;
          tempFactor *= 10;
          if (FuelMap.Get(i, j) >= 0.f &&
            OxyCount > 0 && TemperatureMap.Get(i, j) > Const::IT_Wood)
          {
            if (TemperatureMap.Get(i, j) <= Const::BT_Organics)
            {
              //HeatMap[i][j] += tempRange * (float)dt;
            }
            //float oxyfactor = 
          }//if
          else
          {
            if (TemperatureMap.Get(i, j) >= Const::IT_Wood)
            {
              TemperatureMap.Set(i, j, TemperatureMap.Get(i, j) - tempRange * (float) dt);
            }
          }
          FireMap.Set(i, j, tempFactor);
        }//for i
      }//for j
    }//function

    void ThermodynamicsSystem::Reset ()
    {
      AtmosphericTemperature = 300.f;
      TemperatureMap.fill (300.f);

      DensityMap.fill (Const::p_Air);

      VelocityMapX.fill ({ 0 });
      VelocityMapY.fill ({ 0 });
      VelocityMap_PrevX.fill ({ 0 });
      VelocityMap_PrevY.fill ({ 0 });

      FireMap.fill (0.0f);

      FuelMap.fill (10.f);
    }

    vec2 ThermodynamicsSystem::GetSubscript(const float &x, const float &y)
    {
      int sub_x = int(std::abs(((x)* (MapSize.x / 2 - 1) + MapOffset.x - 1)));
      int sub_y = int(std::abs(((y)* (MapSize.y / 2 - 1) + MapOffset.y - 1)));
      vec2 res(sub_x, sub_y);
      return res;
    }

    void ThermodynamicsSystem::SpawnThreads ()
    {
      // Temperature Threads
      for (int i = 0; i < kNumThreads; ++i)
      {
        eventStartTemperature [i] = CreateEvent (NULL, false, NULL, NULL);
        eventEndTemperature [i] = CreateEvent (NULL, true, NULL, NULL);
        m_TemperatureThreads [i] = CreateThread (NULL, 0, UpdateTemperatureFunc, NULL, 0, NULL);
      }

      // Velocity Threads
      for (int i = 0; i < kNumThreads; ++i)
      {
        eventStartVelocity [i] = CreateEvent (NULL, false, NULL, NULL);
        eventEndFire [i] = CreateEvent (NULL, true, NULL, NULL);
        m_VelocityThreads [i] = CreateThread (NULL, 0, UpdateVelocityFunc, NULL, 0, NULL);
      }

      // Fire Threads
      for (int i = 0; i < kNumThreads; ++i)
      {
        eventStartFire [i] = CreateEvent (NULL, false, NULL, NULL);
        eventEndFire [i] = CreateEvent (NULL, true, NULL, NULL);
        m_FireThreads [i] = CreateThread (NULL, 0, UpdateFireFunc, NULL, 0, NULL);
      }
    }

    void ThermodynamicsSystem::ReleaseThreads ()
    {
      for (int i = 0; i < kNumThreads; ++i)
      {
        CloseHandle (eventStartTemperature [i]);
        CloseHandle (eventEndTemperature [i]);
        CloseHandle (m_TemperatureThreads [i]);
      }

      for (int i = 0; i < kNumThreads; ++i)
      {
        CloseHandle (eventStartVelocity [i]);
        CloseHandle (eventEndVelocity [i]);
        CloseHandle (m_VelocityThreads [i]);
      }

      for (int i = 0; i < kNumThreads; ++i)
      {
        CloseHandle (eventStartFire [i]);
        CloseHandle (eventEndFire [i]);
        CloseHandle (m_FireThreads [i]);
      }
    }

    void ThermodynamicsSystem::UpdateMultiThreaded ()
    {
      // Start Temperature Threads
      for (int i = 0; i < kNumThreads; ++i)
      {
        SetEvent (eventStartTemperature [i]);
      }

      // Start Temperature Threads
      for (int i = 0; i < kNumThreads; ++i)
      {
        SetEvent (eventStartVelocity [i]);
      }

      // Start Temperature Threads
      for (int i = 0; i < kNumThreads; ++i)
      {
        SetEvent (eventStartFire [i]);
      }

      WaitForMultipleObjects (kNumThreads, eventEndTemperature, true, INFINITE);
      WaitForMultipleObjects (kNumThreads, eventEndVelocity, true, INFINITE);
      WaitForMultipleObjects (kNumThreads, eventEndFire, true, INFINITE);

      for (int i = 0; i < kNumThreads; ++i)
      {
        ResetEvent (eventEndTemperature [i]);
      }
      for (int i = 0; i < kNumThreads; ++i)
      {
        ResetEvent (eventEndVelocity [i]);
      }
      for (int i = 0; i < kNumThreads; ++i)
      {
        ResetEvent (eventEndFire [i]);
      }
    }

  }//namespace Physics
}//namespace Framework

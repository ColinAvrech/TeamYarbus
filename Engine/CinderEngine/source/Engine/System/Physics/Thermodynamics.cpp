#include "Physics/Thermodynamics.h"
#include "TDLib.h"
#include "AudioSystem.h"
#include "TerrainCreator.h"
#include "FractalNoise.h"
#include "ThreadFunctions.h"

#define SIZE 10

namespace Framework
{
  namespace Physics
  {
    glm::vec2 ThermodynamicsSystem::MapSize;

    //!Null untill the ObjectSystem has been created
    ThermodynamicsSystem * THERMODYNAMICS = NULL;
    namespace Const = Constant;
    //Constructor
    ThermodynamicsSystem::ThermodynamicsSystem () :
      HeatMap(NULL), OxygenMap(NULL), WaterMap(NULL),
      VelocityMap (NULL), Terrain (NULL), FireMap (NULL)
    {
      //Do stuff
      CellSize = 0.1f;
      THERMODYNAMICS = this;
      MapSize = { 100, 100 };
      //Heatmap = new Grid2D<float>(MapSize.x, MapSize.y);
    }

    //Destructor
    ThermodynamicsSystem::~ThermodynamicsSystem ()
    {
      //Delete all the shit-load of allocated memory
      if (HeatMap)
        delete HeatMap;
      //if (OxygenMap)
      //  delete OxygenMap;
      //if (VelocityMap)
      //  delete VelocityMap;
      //if (Terrain)
      //  delete Terrain;
      //if (FireMap)
      //  delete FireMap;
      //if (WaterMap)
      //  delete WaterMap;
      //if (FuelMap)
      //  delete FuelMap;

      ReleaseThreads ();
    }

    /*-----------------------------------------------------------------------
    // Public Functions
    -----------------------------------------------------------------------*/

    bool ThermodynamicsSystem::Initialize ()
    {
      std::cout << "Thermodynamics Initialized." << std::endl;

      //Procedural::TerrainCreator t(100, 50, 5);


      //Scan level
      MapSize = { 100, 100 };
      std::cout << "Grid " << MapSize.x << "x " << MapSize.y << std::endl;
      MapOffset = { 50, 50 };
      AtmosphericTemperature = 300.f;
      //Allocate heatmap
      HeatMap = new Grid2D<float>((int)MapSize.x, (int)MapSize.y);
      HeatMap->fill(300.f);

      //Allocate Oxygen/Density map
      OxygenMap = new Grid2D<float>((int)MapSize.x, (int)MapSize.y);
      OxygenMap->fill(Const::p_Air);

      //Allocate Velocity map
      VelocityMap = new Grid2D<glm::vec2>((int)MapSize.x, (int)MapSize.y);
      VelocityMap->fill({ 0, 0 });

      //Allocate Terrain map
      Terrain = new Grid2D<Material>((int)MapSize.x, (int)MapSize.y);
      Terrain->fill(AIR);

      WaterMap = new Grid2D<float>((int)MapSize.x, (int)MapSize.y);
      WaterMap->fill(0.0f);

      //Allocate Fire map
      FireMap = new Grid2D<float>((int)MapSize.x, (int)MapSize.y);
      FireMap->fill(0.0f);

      //Allocate Fuel map
      FuelMap = new Grid2D<float>((int)MapSize.x, (int)MapSize.y);
      FuelMap->fill(0.0f);

      SpawnThreads ();

      return true;
    }

    // Called every frame
    void ThermodynamicsSystem::Update (const double dt)
    {
      //UpdateTemp (0, 100, 0.016);
      //ComputeVelocity (0, 100, 0.016);
      //UpdateFire (0, 100, 0.016);

      UpdateMultiThreaded ();
    }

    // Getters
    //Get cell temperature
    float ThermodynamicsSystem::GetCellTemperature (float x, float y)
    {
      glm::vec2 sub = GetSubscript(x, y);
      int sub_x = int (sub.x);
      int sub_y = int (sub.y);
      if (sub_x < 0 || sub_x >= MapSize.x || sub_y < 0 || sub_y >= MapSize.y)
        return 0.f;
      return HeatMap->Get(sub_x, sub_y);
    }

    //Get cell oxygen content
    float ThermodynamicsSystem::GetCellOxygen (float x, float y)
    {
      glm::vec2 sub = GetSubscript(x, y);
      int sub_x = int(sub.x);
      int sub_y = int(sub.y);
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
        return 0.f;
      return OxygenMap->Get(sub_x, sub_y);
    }
    //Get cell velocity
    glm::vec2 ThermodynamicsSystem::GetCellVelocity (float x, float y)
    {
      glm::vec2 sub = GetSubscript(x, y);
      int sub_x = int(sub.x);
      int sub_y = int(sub.y);
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
      {
        return glm::vec2 (0, 0);
      }
      return VelocityMap->Get(sub_x, sub_y);
    }

    // Setters
    void ThermodynamicsSystem::ToggleAutoDissipation ()
    {
      EqualizePressure = !EqualizePressure;
    }

    float ThermodynamicsSystem::SetCellTemperature (const float x, const float y, const float temp, const double dt)
    {
      glm::vec2 sub = GetSubscript(x, y);
      int sub_x = int(sub.x);
      int sub_y = int(sub.y);
      float dQ;
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
        dQ = ConductiveHeatTransfer (Const::K_Wood, AtmosphericTemperature, temp, dt, 1);
      else
      {
        dQ = ConductiveHeatTransfer (Const::K_Air, HeatMap->Get(sub_x, sub_y), temp, dt, 1);
        float deltaTemp = dTemp (dQ, OxygenMap->Get(sub_x, sub_y) * 0.001f, /*Const::c_Air*/100);
        HeatMap->Set(sub_x, sub_y, HeatMap->Get(sub_x, sub_y) + deltaTemp);
      }
      return dQ;
    }

    /*-----------------------------------------------------------------------
    // Private Functions
    -----------------------------------------------------------------------*/

    //Update temperatures
    void ThermodynamicsSystem::UpdateTemp(int start_index, int end_index, const double dt)
    {
      //std::cout << start_index << "\n";
      //std::cout << "Updated Temperature/Density/Pressure" << std::endl;
      for (int j = start_index; j < end_index; ++j)
      {
        for (int i = 0; i < 100; ++i)
        {
          float netdQ = 0.f;
          float oTemp = HeatMap->Get(i, j);
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              if (x != i || y != j)
              {
                if (x < MapSize.x && x >= 0 && y < MapSize.y && y >= 0)
                {
                  float dQ = ConductiveHeatTransfer (Const::K_Air, HeatMap->Get(i, j), HeatMap->Get(x, y), dt, 0.1f);
                  netdQ += dQ;
                  float oTemp = HeatMap->Get(x, y);
                  HeatMap->Set(x, y, HeatMap->Get(x, y) - dTemp (dQ, OxygenMap->Get(x, y) * 0.001f, Const::c_Air));

                  float factor = HeatMap->Get(x, y) / oTemp;
                  OxygenMap->Set(x, y, OxygenMap->Get(x, y) / factor);
                }
                else
                {
                  float dQ = ConductiveHeatTransfer (Const::K_Air, HeatMap->Get(i, j), AtmosphericTemperature, dt, 0.1f);
                  netdQ += dQ;
                }
              }
            }
          }
          if (j + 1 <= MapSize.y)
          {
            if (Terrain->Get(i, j) == AIR && Terrain->Get(i, j + 1) == AIR)
            {
              float dQConv = ConvectiveHeatTransfer (Const::Hc_Air, HeatMap->Get(i, j), HeatMap->Get(i, j + 1), dt);
              float oTempConv = HeatMap->Get(i, j + 1);
              netdQ += dQConv;
              HeatMap->Set(i, j + 1, HeatMap->Get(i, j + 1) - dTemp (dQConv, OxygenMap->Get(i, j + 1) * 0.001f, Const::c_Air));
              float factor2 = HeatMap->Get(i, j + 1) / oTempConv;
              OxygenMap->Set(i, j + 1, OxygenMap->Get(i, j) / factor2);
            }
          }
          else
          {
            if (Terrain->Get(i, j) == 0 && Terrain->Get(i, j + 1) == 0)
            {
              float dQConv = ConvectiveHeatTransfer (Const::Hc_Air, HeatMap->Get(i, j), AtmosphericTemperature, dt);
              netdQ += dQConv;
            }
          }
          HeatMap->Set(i, j, HeatMap->Get(i, j) + dTemp (netdQ, OxygenMap->Get(i, j) * 0.001f, Const::c_Air));
          float factor1 = HeatMap->Get(i, j) / oTemp;
          OxygenMap->Set(i, j, OxygenMap->Get(i, j) / factor1);
        }//for
      }//for
    }//function

    //Update velocity vectors
    void ThermodynamicsSystem::ComputeVelocity(int start_index, int end_index, const double dt)
    {
      glm::vec2 dirvec [8] = {
        { -1, -1 },
        { 0, -1 },
        { 1, -1 },
        { -1, 0 },
        { 1, 0 },
        { -1, 1 },
        { 0, 1 },
        { 1, 1 }
      };
      for (int j = start_index; j < end_index; ++j)
      {
        for (int i = 1; i < 100; ++i)
        {
          float dSum = 0.f;
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              dSum += OxygenMap->Get(x, y);
            }
          }
          float meanDensity = dSum / 8;
          float buoyancy = Buoyancy (meanDensity, OxygenMap->Get(i, j), 1.f);

          int vectorindex = 0;
          float dDenseSum = 0.f;
          VelocityMap->Set(i, j, { 0, 0 });
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              if (x != i || y != j)
              {
                float dDense = OxygenMap->Get(x, y) - OxygenMap->Get(i, j);
                VelocityMap->Set(i, j, VelocityMap->Get(i, j) - (dirvec [vectorindex] * (dDense / 8)));
                //if (EqualizePressure)
                //{
                //OxygenMap[x][y] += (dDense / 8) * (float)dt;
                dDenseSum += (dDense / 8);
                //}
                ++vectorindex;
              }

            } //for x
          } //for y
          //OxygenMap[i][j] -= dDenseSum * (float)dt;
          VelocityMap->Get(i, j) += (glm::vec2 (0, 1) * buoyancy);
        }//for i
      } //for j
    }

    //Update fire
    void ThermodynamicsSystem::UpdateFire(int start_index, int end_index, const double dt)
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
              if ((x != i || y != j) && Terrain->Get(i, j) == 0)
              {
                ++OxyCount;
                //OxyAmount += OxygenMap[x][y] * CellSize*CellSize*CellSize;
              }
            } //for x
          } //for y
          float tempRange = Const::BT_Organics - Const::IT_Wood;
          float tempDiff = HeatMap->Get(i, j) - Const::IT_Wood;
          float tempFactor = tempDiff / tempRange;
          tempFactor *= 10;
          if (FuelMap->Get(i, j) >= 0.f &&
            OxyCount > 0 && HeatMap->Get(i, j) > Const::IT_Wood)
          {
            if (HeatMap->Get(i, j) <= Const::BT_Organics)
            {
              //HeatMap[i][j] += tempRange * (float)dt;
            }
            //float oxyfactor = 
          }//if
          else
          {
            if (HeatMap->Get(i, j) >= Const::IT_Wood)
            {
              HeatMap->Set(i, j, HeatMap->Get(i, j) - tempRange * (float) dt);
            }
          }
          FireMap->Set(i, j, tempFactor);
        }//for i
      }//for j
    }//function

    void ThermodynamicsSystem::Reset ()
    {
      AtmosphericTemperature = 300.f;
      HeatMap->fill(300.f);

      OxygenMap->fill(Const::p_Air);

      VelocityMap->fill({ 0, 0 });

      FireMap->fill(0.0f);

      FuelMap->fill(10.f);
    }

    glm::vec2 ThermodynamicsSystem::GetSubscript(const float x, const float y)
    {
      int sub_x = int(std::abs(((x)* (MapSize.x / 2 - 1) + MapOffset.x - 1)));
      int sub_y = int(std::abs(((y)* (MapSize.y / 2 - 1) + MapOffset.y - 1)));
      glm::vec2 res(sub_x, sub_y);
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

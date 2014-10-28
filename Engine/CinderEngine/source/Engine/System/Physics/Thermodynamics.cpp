#include "Physics/Thermodynamics.h"
#include "AudioSystem.h"
#include "TDLib.h"

#define SIZE 10

namespace Framework
{
  namespace Physics
  {
    //!Null untill the ObjectSystem has been created
    ThermodynamicsSystem * THERMODYNAMICS = NULL;
    namespace Const = Constant;
    //Constructor
    ThermodynamicsSystem::ThermodynamicsSystem() :
      HeatMap(NULL), OxygenMap(NULL),
      VelocityMap(NULL), Terrain(NULL), FireMap(NULL)
    {
      //Do stuff
      CellSize = 0.1f;
      THERMODYNAMICS = this;
      MapSize = { 100, 100 };
    }

    //Destructor
    ThermodynamicsSystem::~ThermodynamicsSystem()
    {
      //Delete all the shit-load of allocated memory
      if (HeatMap)
      {
        for (int i = 0; i < MapSize.x; ++i)
          delete[] HeatMap[i];
        delete[] HeatMap;
      }
      if (OxygenMap)
      {
        for (int i = 0; i < MapSize.x; ++i)
          delete[] OxygenMap[i];
        delete[] OxygenMap;
      }
      if (VelocityMap)
      {
        for (int i = 0; i < MapSize.x; ++i)
          delete[] VelocityMap[i];
        delete[] VelocityMap;
      }
      if (Terrain)
      {
        for (int i = 0; i < MapSize.x; ++i)
          delete[] Terrain[i];
        delete[] Terrain;
      }
      if (FireMap)
      {
        for (int i = 0; i < MapSize.x; ++i)
          delete[] FireMap[i];
        delete[] FireMap;
      }
    }

    /*-----------------------------------------------------------------------
    // Public Functions
    -----------------------------------------------------------------------*/

    bool ThermodynamicsSystem::Initialize()
    {
      std::cout << "Thermodynamics Initialized." << std::endl;
      std::cout << "Grid 1000 x 1000" << std::endl;

      //Scan level
      
      //Allocate heatmap
      HeatMap = new float*[100];
      for (int i = 0; i < 100; ++i)
        HeatMap[i] = new float[100];
      for (int j = 0; j < 100; ++j)
      {
        for (int i = 0; i < 100; ++i)
        {
          HeatMap[i][j] = 300.f;
        }
      }
      //HeatMap[50][50] = 500.f;

      //Allocate Oxygen/Density map
      OxygenMap = new float*[100];
      for (int i = 0; i < 100; ++i)
        OxygenMap[i] = new float[100];

      for (int j = 0; j < 100; ++j)
      {
        for (int i = 0; i < 100; ++i)
        {
          OxygenMap[i][j] = 1.225f;
        }
      }

      //Allocate Velocity map
      VelocityMap = new glm::vec2*[100];
      for (int i = 0; i < 100; ++i)
        VelocityMap[i] = new glm::vec2[100];
      for (int j = 0; j < 100; ++j)
      {
        for (int i = 0; i < 100; ++i)
        {
          VelocityMap[i][j] = { 0, 0 };
        }
      }


      //Allocate Terrain map
      Terrain = new int*[100];
      for (int i = 0; i < 100; ++i)
        Terrain[i] = new int[100];

      for (int j = 0; j < 100; ++j)
      {
        for (int i = 0; i < 100; ++i)
        {
          Terrain[i][j] = 0;
        }
      }
      for (int i = 0; i < 100; ++i)
        Terrain[i][0] = 1;

      FireMap = new float*[100];
      for (int i = 0; i < 100; ++i)
        FireMap[i] = new float[100];

      for (int j = 0; j < 100; ++j)
      {
        for (int i = 0; i < 100; ++i)
          FireMap[i][j] = 0;
      }

      FuelMap = new float*[100];
      for (int i = 0; i < 100; ++i)
        FuelMap[i] = new float[100];

      for (int j = 0; j < 100; ++j)
      {
        for (int i = 0; i < 100; ++i)
        {
          FuelMap[i][j] = 0;
        }
      }
      for (int i = 0; i < 100; ++i)
        FuelMap[i][0] = 10.f;

      return true;
    }
   
    // Called every frame
    void ThermodynamicsSystem::Update(const double dt)
    {
      HeatMap[50][50] += 10 * AUDIOSYSTEM->input.peaklevel[0];
      UpdateTemp(0.016);
      ComputeVelocity(0.016);
      UpdateFire(0.016);
      //std::cout << "{ " << Physics::THERMODYNAMICS->GetCellVelocity(20, 20).x << ", " << Physics::THERMODYNAMICS->GetCellVelocity(20, 20).y << " }\n";
      //std::cout << HeatMap[50][50] << " , ";
      //std::cout << HeatMap[51][50] << " \n";
      //std::cout << "{" << VelocityMap[50][50].x << " , " << VelocityMap[50][50].y << "}\n";
    }

    // Getters
    //Get cell temperature
    float ThermodynamicsSystem::GetCellTemperature(float x, float y)
    {
      int sub_x = static_cast<int>((x) + MapOffset.x);
      int sub_y = static_cast<int>((y) + MapOffset.y);
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
        return 0.f;
      return HeatMap[sub_x][sub_y];
    }

    //Get cell oxygen content
    float ThermodynamicsSystem::GetCellOxygen(float x, float y)
    {
      int sub_x = static_cast<int>((x / CellSize) + MapOffset.x);
      int sub_y = static_cast<int>((y / CellSize) + MapOffset.y);
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
        return 0.f;
      return OxygenMap[sub_x][sub_y];
    }
    //Get cell velocity
    glm::vec2 ThermodynamicsSystem::GetCellVelocity(float x, float y)
    {
      int sub_x = static_cast<int>((x) + MapOffset.x);
      int sub_y = static_cast<int>((y) + MapOffset.y);
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
      {
        return glm::vec2 (0, 0);
      }
      return VelocityMap[sub_x][sub_y];
    }

    // Setters
    void ThermodynamicsSystem::ToggleAutoDissipation()
    {
      if (EqualizePressure)
        EqualizePressure = false;
      else
        EqualizePressure = true;
    }

    float ThermodynamicsSystem::SetCellTemperature(const float x, const float y, const float temp, const double dt)
    {
      int sub_x = static_cast<int>((x / CellSize) + MapOffset.x);
      int sub_y = static_cast<int>((y / CellSize) + MapOffset.y);
      float dQ;
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
        dQ = ConductiveHeatTransfer(Const::K_Wood, 0, temp, dt, 1);
      else
      {
        dQ = ConductiveHeatTransfer(Const::K_Wood, HeatMap[sub_x][sub_y], temp, dt, 1);
        float deltaTemp = dTemp(dQ, OxygenMap[sub_x][sub_y] * CellSize*CellSize*CellSize, Const::c_Air);
        HeatMap[sub_x][sub_y] += deltaTemp;
      }
      return dQ;
    }

    /*-----------------------------------------------------------------------
    // Private Functions
    -----------------------------------------------------------------------*/

    //Update temperatures
    void ThermodynamicsSystem::UpdateTemp(const double dt)
    {
      //std::cout << "Updated Temperature/Density/Pressure" << std::endl;
      for (int j = 1; j < 99; ++j)
      {
        for (int i = 1; i < 99; ++i)
        {
          float netdQ = 0.f;
          float oTemp = HeatMap[i][j];
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              if (x != i || y != j)
              {
                float dQ = ConductiveHeatTransfer(Const::K_Air, HeatMap[i][j], HeatMap[x][y], dt, 0.1f);
                netdQ += dQ;
                float oTemp = HeatMap[x][y];
                HeatMap[x][y] -= dTemp(dQ, OxygenMap[x][y]*0.001f, Const::c_Air);
                
                float factor = HeatMap[x][y] / oTemp;
                OxygenMap[x][y] /= factor;
              }
            }
          }
          if (Terrain[i][j] == 0 && Terrain[i][j + 1] == 0)
          {
            float dQConv = ConvectiveHeatTransfer(Const::Hc_Air, HeatMap[i][j], HeatMap[i][j + 1], dt);
            float oTempConv = HeatMap[i][j + 1];
            netdQ += dQConv;
            HeatMap[i][j + 1] -= dTemp(dQConv, OxygenMap[i][j + 1]*0.001f, Const::c_Air);
            float factor2 = HeatMap[i][j + 1] / oTempConv;
            OxygenMap[i][j + 1] /= factor2;
          }
          HeatMap[i][j] += dTemp(netdQ, OxygenMap[i][j]*0.001f, Const::c_Air);
          float factor1 = HeatMap[i][j] / oTemp;
          OxygenMap[i][j] /= factor1;
        }//for
      }//for
    }//function
    
    //Update velocity vectors
    void ThermodynamicsSystem::ComputeVelocity(const double dt)
    {
      glm::vec2 dirvec[8] = {
          { -1, -1 },
          { 0, -1 },
          { 1, -1 },
          { -1, 0 },
          { 1, 0 },
          { -1, 1 },
          { 0, 1 },
          { 1, 1 }
      };
      for (int j = 1; j < 99; ++j)
      {
        for (int i = 1; i < 99; ++i)
        {
          float dSum = 0.f;
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              dSum += OxygenMap[x][y];
            }
          }
          float meanDensity = dSum / 8;
          float buoyancy = Buoyancy(meanDensity, OxygenMap[i][j], 0.1f);
          
          int vectorindex = 0;
          float dDenseSum = 0.f;
          //VelocityMap[i][j] = { 0, 0 };
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              if (x != i || y != j)
              {
                float dDense = OxygenMap[x][y] - OxygenMap[i][j];
                VelocityMap[i][j] -= (dirvec[vectorindex] * (dDense/8));
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
          VelocityMap[i][j] += (glm::vec2(0, 1) * buoyancy);
        }//for i
      } //for j
    }
    
    //Update fire
    void ThermodynamicsSystem::UpdateFire(const double dt)
    {
      //std::cout << "Updated Fire" << std::endl;
      for (int j = 1; j < 99; ++j)
      {
        for (int i = 1; i < 99; ++i)
        {
          int OxyCount = 0;
          float OxyAmount = 0.f;
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              if ((x != i || y != j) && Terrain[x][y] == 0)
              {
                ++OxyCount;
                //OxyAmount += OxygenMap[x][y] * CellSize*CellSize*CellSize;
              }
            } //for x
          } //for y
          float tempRange = Const::BT_Organics - Const::IT_Wood;
          float tempDiff = HeatMap[i][j] - Const::IT_Wood;
          float tempFactor = tempDiff / tempRange;
          tempFactor *= 10;
          if (FuelMap[i][j] >= 0.f &&
              OxyCount > 0)
          {
            if (HeatMap[i][j] <= Const::BT_Organics)
            {
             // HeatMap[i][j] += tempRange * (float)dt;
            }
            //float oxyfactor = 
          }//if
          else
          {
            if (HeatMap[i][j] >= Const::IT_Wood)
            {
              //HeatMap[i][j] -= tempRange * (float)dt;
            }
          }
          FireMap[i][j] = tempFactor;
        }//for i
      }//for j
    }//function
  }//namespace Physics
}//namespace Framework

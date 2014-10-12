#include "Physics/Thermodynamics.h"

namespace Framework
{
  namespace Physics
  {
    //Constructor
    ThermodynamicsSystem::ThermodynamicsSystem() :
      HeatMap(NULL), OxygenMap(NULL),
      VelocityMap(NULL), Terrain(NULL), FireMap(NULL)
    {
      //Do stuff
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
      HeatMap = new float*[1000];
      for (int i = 0; i < 1000; ++i)
        HeatMap[i] = new float[1000];

      //Allocate Oxygen/Density map
      OxygenMap = new float*[1000];
      for (int i = 0; i < 1000; ++i)
        OxygenMap[i] = new float[1000];

      //Allocate Velocity map
      VelocityMap = new glm::vec2*[1000];
      for (int i = 0; i < 1000; ++i)
        VelocityMap[i] = new glm::vec2[1000];

      //Allocate Terrain map
      Terrain = new int*[1000];

      return true;
    }
   
    // Called every frame
    void ThermodynamicsSystem::Update(const double dt)
    {
      UpdateTemp();
      ComputeVelocity();
      UpdateFire();
    }

    // Getters
    //Get cell temperature
    float ThermodynamicsSystem::GetCellTemperature(float x, float y)
    {
      int sub_x = static_cast<int>((x / CellSize) + MapOffset.x);
      int sub_y = static_cast<int>((y / CellSize) + MapOffset.y);
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
      int sub_x = static_cast<int>((x / CellSize) + MapOffset.x);
      int sub_y = static_cast<int>((y / CellSize) + MapOffset.y);
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
      {
        glm::vec2 space(0, 0);
        return space;
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
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
        return 0;
      float dTemp = temp - HeatMap[sub_x][sub_y];
      if (dTemp > 0)
      {
        HeatMap[sub_x][sub_y] += dTemp * float (dt);
        return (temp - (dTemp * float (dt)));
      }
      return (temp + (dTemp * float (dt)));
    }

    /*-----------------------------------------------------------------------
    // Private Functions
    -----------------------------------------------------------------------*/

    //Update temperatures
    void ThermodynamicsSystem::UpdateTemp()
    {
      //std::cout << "Updated Temperature/Density/Pressure" << std::endl;

    }
    
    //Update velocity vectors
    void ThermodynamicsSystem::ComputeVelocity()
    {
      //std::cout << "Updated Velocities" << std::endl;
    }
    
    //Update fire
    void ThermodynamicsSystem::UpdateFire()
    {
      //std::cout << "Updated Fire" << std::endl;
    }
  }
}

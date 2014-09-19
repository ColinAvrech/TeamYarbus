#include "Physics/Thermodynamics.h"

namespace Framework
{
  namespace Physics
  {
    //Constructor
    ThermodynamicsSystem::ThermodynamicsSystem() :
      HeatMap(NULL), PressureMap(NULL), OxygenMap(NULL),
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
        for (int i = 0; i < MapSize.x_; ++i)
          delete[] HeatMap[i];
        delete[] HeatMap;
      }
      if (PressureMap)
      {
        for (int i = 0; i < MapSize.x_; ++i)
          delete[] PressureMap[i];
        delete[] PressureMap;
      }
      if (OxygenMap)
      {
        for (int i = 0; i < MapSize.x_; ++i)
          delete[] OxygenMap[i];
        delete[] OxygenMap;
      }
      if (VelocityMap)
      {
        for (int i = 0; i < MapSize.x_; ++i)
          delete[] VelocityMap[i];
        delete[] VelocityMap;
      }
      if (Terrain)
      {
        for (int i = 0; i < MapSize.x_; ++i)
          delete[] Terrain[i];
        delete[] Terrain;
      }
      if (FireMap)
      {
        for (int i = 0; i < MapSize.x_; ++i)
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
      int sub_x = static_cast<int>((x / CellSize) + MapOffset.x_);
      int sub_y = static_cast<int>((y / CellSize) + MapOffset.y_);
      if (sub_x < 0 || sub_x > MapSize.x_ || sub_y < 0 || sub_y > MapSize.y_)
        return 0.f;
      return HeatMap[sub_x][sub_y];
    }
    //Get cell pressure
    float ThermodynamicsSystem::GetCellPressure(float x, float y)
    {
      int sub_x = static_cast<int>((x / CellSize) + MapOffset.x_);
      int sub_y = static_cast<int>((y / CellSize) + MapOffset.y_);
      if (sub_x < 0 || sub_x > MapSize.x_ || sub_y < 0 || sub_y > MapSize.y_)
        return 0.f;
      return PressureMap[sub_x][sub_y];
    }
    //Get cell oxygen content
    float ThermodynamicsSystem::GetCellOxygen(float x, float y)
    {
      int sub_x = static_cast<int>((x / CellSize) + MapOffset.x_);
      int sub_y = static_cast<int>((y / CellSize) + MapOffset.y_);
      if (sub_x < 0 || sub_x > MapSize.x_ || sub_y < 0 || sub_y > MapSize.y_)
        return 0.f;
      return OxygenMap[sub_x][sub_y];
    }
    //Get cell velocity
    Vec2 ThermodynamicsSystem::GetCellVelocity(float x, float y)
    {
      int sub_x = static_cast<int>((x / CellSize) + MapOffset.x_);
      int sub_y = static_cast<int>((y / CellSize) + MapOffset.y_);
      if (sub_x < 0 || sub_x > MapSize.x_ || sub_y < 0 || sub_y > MapSize.y_)
      {
        Vec2 space(0, 0);
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
      int sub_x = static_cast<int>((x / CellSize) + MapOffset.x_);
      int sub_y = static_cast<int>((y / CellSize) + MapOffset.y_);
      if (sub_x < 0 || sub_x > MapSize.x_ || sub_y < 0 || sub_y > MapSize.y_)
        return 0;
      float dTemp = temp - HeatMap[sub_x][sub_y];
      if (dTemp > 0)
      {
        HeatMap[sub_x][sub_y] += dTemp * dt;
        return (temp - (dTemp * dt));
      }
      return (temp + (dTemp * dt));
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

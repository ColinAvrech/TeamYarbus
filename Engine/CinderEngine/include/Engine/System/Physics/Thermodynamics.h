/*****************************************************************************/
/*!
\file   Thermodynamics.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The thermodynamics system controls the flow of fluids around the level.
*/
/*****************************************************************************/

#pragma once


/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes


#include "BaseSystem.h"
#include <unordered_map>
#include "Grid2D.h"
#include "FluidSolver.h"

#pragma endregion

namespace Framework
{
  namespace Physics
  {
    typedef void* ThreadHandle;

    //Possible materials used in terrain
    enum Material
    {
      AIR,
      WATER,
      WOOD,
      SOIL,
      GRASS,
      STONE,
      IRON,
      LEAD,
      STEEL,
      COTTON,
      CEMENT,
      CARBON,
      FUEL,
    };//enum material
    /*---------------------------------------------------------------------------
    // Class
    ---------------------------------------------------------------------------*/
    class ThermodynamicsSystem : public BaseSystem
    {
    public:

      /*-----------------------------------------------------------------------
      // Constructors
      -----------------------------------------------------------------------*/
#pragma region Constructors

      ThermodynamicsSystem();

#pragma endregion

      /*-----------------------------------------------------------------------
      // Public Variables
      -----------------------------------------------------------------------*/
#pragma region Public Variables

#pragma endregion

      /*---------------------------------------------------------------------*/
      // Public Structs
      /*---------------------------------------------------------------------*/
#pragma region Public Structs

#pragma endregion

      /*-----------------------------------------------------------------------
      // Public Functions
      -----------------------------------------------------------------------*/
#pragma region Public Functions
      //!Initialize the system
      bool Initialize();

      // Called every frame
      void Update(const double dt);

      //Update temperatures
      void UpdateTemp (int start_index, int end_index, const double dt);
      //Calculate velocity vectors
      void ComputeVelocity (int start_index, int end_index, const double dt);
      //Update fire
      void UpdateFire (int start_index, int end_index, const double dt);

      void Reset ();

      //////////////////////////////////////////////////////////////////////////
      // MULTI THREADING
      //////////////////////////////////////////////////////////////////////////
      static const int kNumThreads = 8;

    private:
      // Variables
      ThreadHandle m_TemperatureThreads [kNumThreads];
      ThreadHandle m_VelocityThreads [kNumThreads];
      ThreadHandle m_FireThreads [kNumThreads];

      // Methods
      void SpawnThreads ();
      void ReleaseThreads ();
      void UpdateMultiThreaded ();
      //////////////////////////////////////////////////////////////////////////

    public:
      // Returns name of System
      const std::string GetName() { return "ThermodynamicsSystem"; }

      // Getters
      //Get Cell Pixel size
      float GetCellSize(){ return CellSize; }
      //Get Map size
      glm::vec2 GetMapSize(){ return MapSize; }
      //Get cell temperature
      float GetCellTemperature(float x, float y);
      //Get cell oxygen content
      float GetCellOxygen(float x, float y);
      //Get cell velocity
      glm::vec2 GetCellVelocity(float x, float y);

      // Setters
      void ToggleAutoDissipation();
      float SetCellTemperature(const float x, const float y, const float temp, const double dt);
      void SetCellVelocity (const int x, const int y, glm::vec2 v);

#pragma endregion


      /*-----------------------------------------------------------------------
      // Static Public Variables
      -----------------------------------------------------------------------*/
#pragma region Static Public Variables

      static glm::ivec2 MapSize;

#pragma endregion


      /*-----------------------------------------------------------------------
      // Static Public Functions
      -----------------------------------------------------------------------*/
#pragma region Static Public Functions

#pragma endregion


      /*-----------------------------------------------------------------------
      // Destructor
      -----------------------------------------------------------------------*/
#pragma region Destructors
      ~ThermodynamicsSystem();

#pragma endregion

      friend class Smoke_Grid;

    private:

      /*-----------------------------------------------------------------------
      // Private Variables
      -----------------------------------------------------------------------*/
#pragma region Private Variables
      //Automatically equalize pressure over time
      bool EqualizePressure;
      float CellSize;
      glm::vec2 MapOffset;
      float AtmosphericTemperature;

      //Temperature Map. Temperature is stored in Kelvin.
      Grid2D<float> TemperatureMap;
      //Oxygen density. Stored in Kg/m^2.
      Grid2D<float> DensityMap;
      Grid2D<float> DensityMap_Prev;
      //Velocity Map. Stores 2d vectors.
      Grid2D<float> VelocityMapX;
      Grid2D<float> VelocityMap_PrevX;
      Grid2D<float> VelocityMapY;
      Grid2D<float> VelocityMap_PrevY;
      //Terrain. Simple collision table using enums.
      Grid2D<Material> Terrain;
      //Water and moisture content
      Grid2D<float> WaterMap;
      //Fire Map. Stores intensity of flame on a scale of 0 - 10.
      Grid2D<float> FireMap;
      //Amount of fuel in the cell
      Grid2D<float> FuelMap;

      FluidSolver solver;
#pragma endregion


      /*-----------------------------------------------------------------------
      // Private Structs
      -----------------------------------------------------------------------*/
#pragma region Private Structs

      struct conductionProperties
      {
        //std::string name will be used to map these;
        bool  isFluid;  //Is the material a fluid
        bool  Volatile; //Can this material catch fire
        float Hc;       //Heat transfer coefficient
        float Mass;     //Mass of 1 block of material
        float c;        //Specific heat
      };

      std::unordered_map<std::string, conductionProperties*> materialList;
#pragma endregion

      /*-----------------------------------------------------------------------
      // Private Functions
      -----------------------------------------------------------------------*/
#pragma region Private Functions
      //Determine subscript from position
      glm::vec2 GetSubscript(const float x, const float y);
#pragma endregion


      /*-----------------------------------------------------------------------
      // Static Functions
      -----------------------------------------------------------------------*/
#pragma region Static Functions

#pragma endregion

    }; //class thermodynamics

    extern ThermodynamicsSystem *THERMODYNAMICS;
  } //namespace physics
} //namespace framework

//-----------------------------------------------------------------------------

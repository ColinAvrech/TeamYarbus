/*****************************************************************************/
/*!
\file   Thermodynamics.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The thermodynamics system controls the flow of fluids around the level.
*/
/*****************************************************************************/

#ifndef _THERMO_H
#define _THERMO_H

/*-----------------------------------------------------------------------------
// Includes
-----------------------------------------------------------------------------*/
#pragma region Includes


#include "BaseSystem.h"
#include "Common.h"
#include "Vec2.h"
#include <unordered_map>

#pragma endregion

namespace Framework
{
  namespace Physics
  {
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

      // Returns name of System
      const std::string GetName(){ return "ThermodynamicsSystem"; }

      // Getters
      //Get Cell Pixel size
      int GetCellSize(){ return CellSize; }
      //Get Map size
      Vec2 GetMapSize(){ return MapSize; }
      //Get cell temperature
      float GetCellTemperature(float x, float y);
      //Get cell pressure
      float GetCellPressure(float x, float y);
      //Get cell oxygen content
      float GetCellOxygen(float x, float y);
      //Get cell velocity
      Vec2 GetCellVelocity(float x, float y);

      // Setters
      void ToggleAutoDissipation();
      float SetCellTemperature(const float x, const float y, const float temp, const double dt);

#pragma endregion


      /*-----------------------------------------------------------------------
      // Static Public Variables
      -----------------------------------------------------------------------*/
#pragma region Static Public Variables

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

    private:

      /*-----------------------------------------------------------------------
      // Private Variables
      -----------------------------------------------------------------------*/
#pragma region Private Variables
      //Automatically equalize pressure over time
      bool EqualizePressure;
      Vec2 MapSize;
      int CellSize;
      Vec2 MapOffset;

      //Temperature Map. Temperature is stored in Kelvin.
      float **HeatMap;
      //Pressure Map. Pressure is stored in Pascals.
      float **PressureMap;
      //Oxygen density. Stored in Kg/m^2.
      float **OxygenMap;
      //Velocity Map. Stores 2d vectors.
      Vec2 **VelocityMap;
      //Terrain. Simple collision table using enumed ints.
      int **Terrain;
      //Fire Map. Stores intensity of flame on a scale of 1 - 10.
      float **FireMap;

#pragma endregion


      /*-----------------------------------------------------------------------
      // Private Structs
      -----------------------------------------------------------------------*/
#pragma region Private Structs    
      struct conductionProperties
      {
        //std::string name will be used to map these;
        bool isFluid;    //Is the material a fluid
        bool Volatile; //Can this material catch fire
        float Hc;      //Heat transfer coefficient
        float Mass;    //Mass of 1 block of material
        float c;       //Specific heat
      };

      std::unordered_map<std::string, conductionProperties*> materialList;
#pragma endregion

      /*-----------------------------------------------------------------------
      // Private Functions
      -----------------------------------------------------------------------*/
#pragma region Private Functions
      //Update temperatures
      void UpdateTemp();
      //Calculate velocity vectors
      void ComputeVelocity();
      //Update fire
      void UpdateFire();

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

#endif				// _AUDIO_H
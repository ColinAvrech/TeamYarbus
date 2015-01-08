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

#pragma endregion
#define GRID_Y_SIZE 32

namespace Framework
{
  namespace Physics
  {
    typedef void* ThreadHandle;

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

      virtual bool UpdatesOnPaused();

      // Called every frame
      void Update(const float& dt);

      //Update temperatures
      void temp_step(const int& start_index, const int& end_index, const float& dt);
      //Calculate velocity vectors
      void vel_step(const int& start_index, const int& end_index, const float& dt);
      //Update fire
      void UpdateFire(const float& dt);

      void Reset ();
      void Clear();

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
      const string GetName() { return "ThermodynamicsSystem"; }

      // Getters
      //Get Cell Pixel size
      float GetCellSize(){ return CellSize; }
      //Get Map size
      vec2 GetMapSize(){ return MapSize; }
      //Get cell temperature
      float GetCellTemperature(int x, int y);
      //Get cell oxygen content
      float GetCellOxygen(const float& x, const float& y);
      //Get cell velocity
      vec2 GetCellVelocity(const float& x, const float& y);
	    //Return Terrain binary map value
	    int GetCellMaterial(int x, int y);

      // Setters
      void SetMapSize(int size_x, int size_y = GRID_Y_SIZE);
      void ToggleAutoDissipation();
      float SetCellTemperature(const float& x, const float& y, const float& temp, const float& dt);
      void SetCellVelocity (const int x, const int y, vec2 v);
      void Add_Object(FireStarter *obj);
      void Add_Group(FireGroup *obj);
      void Draw ();
#pragma endregion


      /*-----------------------------------------------------------------------
      // Static Public Variables
      -----------------------------------------------------------------------*/
#pragma region Static Public Variables

      static glm::ivec2 MapSize;
      static std::vector <glm::vec2> TerrainPoints;

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
      friend class FireSystem;
      friend class Terrain2D;
      friend class FireStarter;

    private:

      /*-----------------------------------------------------------------------
      // Private Variables
      -----------------------------------------------------------------------*/
#pragma region Private Variables
      //Automatically equalize pressure over time
      bool EqualizePressure;
      bool Allocated;
      float CellSize;
      vec2 MapOffset;
      float AtmosphericTemperature;
      int simulation_speed;

      //Temperature Map. Temperature is stored in Kelvin.
      Grid2D<float> TemperatureMap;
      Grid2D<float> TemperatureMap_Prev;
      //Oxygen density. Stored in Kg/m^2.
      Grid2D<float> DensityMap;
      //Velocity Map. Stores 2d vectors.
      Grid2D<float> VelocityMapX;
      Grid2D<float> VelocityMap_PrevX;
      Grid2D<float> VelocityMapY;
      Grid2D<float> VelocityMap_PrevY;
      //Terrain. Simple collision table using enums.
      Grid2D<Material> Terrain;
      //Water and moisture content
      //Grid2D<float> WaterMap;
      int *y_offset;
      
      //Stores the time spent not updating for each column
      //float *dt_Tracker = nullptr;

      static GUIText* guiText;

      vector<FireGroup*> fireGroups;
      vector<std::pair<glm::ivec2, FireStarter*>> FireMap;
      static FireSystem* FIRE;
#pragma endregion


      /*-----------------------------------------------------------------------
      // Private Structs
      -----------------------------------------------------------------------*/
#pragma region Private Structs

      struct conductionProperties
      {
        //string name will be used to map these;
        bool  isFluid;  //Is the material a fluid
        bool  Volatile; //Can this material catch fire
        float Hc;       //Heat transfer coefficient
        float Density;  //Mass of 1 block of material
        float c;        //Specific heat
        float K;        //Thermal conductivity
        float IT;       //Ignition temperature
      };

      std::vector <conductionProperties> materialList;
#pragma endregion

      /*-----------------------------------------------------------------------
      // Private Functions
      -----------------------------------------------------------------------*/
#pragma region Private Functions
      
      void diffuse(int start, int end, const float dt);
      void advect(int start, int end, const float dt);
      
      void Init_Materials();
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

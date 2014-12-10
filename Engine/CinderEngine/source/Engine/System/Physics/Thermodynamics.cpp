/******************************************************************************/
/*!
\file   Thermodynamics.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Physics/Thermodynamics.h"
#include "TDLib.h"
#include "AudioSystem.h"
#include "TerrainCreator.h"
#include "FractalNoise.h"
#include "ThreadFunctions.h"
#include "WindowSystem.h"
#include "Camera.h"
#include "solver.c"

//Used for sending all trees event
#include "EventSystem.h" 
#include "GameEvent.h"

#define SIZE 10

namespace Framework
{
  namespace Physics
  {
    glm::ivec2 ThermodynamicsSystem::MapSize;
    FireSystem* ThermodynamicsSystem::FIRE = nullptr;
    std::vector <glm::vec2> ThermodynamicsSystem::TerrainPoints;

    //!Null untill the ObjectSystem has been created
    ThermodynamicsSystem * THERMODYNAMICS = NULL;
    GUIText* ThermodynamicsSystem::guiText = nullptr;
    namespace Const = Constant;
    //Constructor
    ThermodynamicsSystem::ThermodynamicsSystem()
    {
      //Do stuff
      CellSize = 0.1f;
      THERMODYNAMICS = this;
    }

    bool ThermodynamicsSystem::UpdatesOnPaused()
    {
      return false;
    }

    //Destructor
    ThermodynamicsSystem::~ThermodynamicsSystem()
    {
      ReleaseThreads();
    }

    /*-----------------------------------------------------------------------
    // Public Functions
    -----------------------------------------------------------------------*/

    bool ThermodynamicsSystem::Initialize()
    {
      if (guiText == nullptr)
      {
        GameObject* go = new GameObject(10000);
        guiText = reinterpret_cast<GUIText*> (go->AddComponent("GUIText"));
        guiText->position = { -0.2f, -0.9f };
        guiText->Initialize();
      }

      //Initialize material list
      Init_Materials();
      //Scan level
      MapSize = { 128, 128 };
      std::cout << "Grid " << MapSize.x << "x " << MapSize.y << std::endl;
      MapOffset = { -MapSize.x / 2, MapSize.y / 2 };
      AtmosphericTemperature = 300.f;
      //Allocate heatmap
      TemperatureMap.allocate(MapSize.x, MapSize.y);
      TemperatureMap.fill(300.f);

      //Allocate Oxygen/Density map
      DensityMap.allocate(MapSize.x + 2, MapSize.y + 2);
      DensityMap_Prev.allocate(MapSize.x + 2, MapSize.y + 2);
      DensityMap.fill(Constant::K_Air);
      DensityMap_Prev.fill(Constant::K_Air);

      //Allocate Velocity map
      VelocityMapX.allocate(MapSize.x + 2, MapSize.y + 2);
      VelocityMapY.allocate(MapSize.x + 2, MapSize.y + 2);
      VelocityMap_PrevX.allocate(MapSize.x + 2, MapSize.y + 2);
      VelocityMap_PrevY.allocate(MapSize.x + 2, MapSize.y + 2);
      VelocityMapX.fill({ 0 });
      VelocityMapY.fill({ 0 });
      VelocityMap_PrevX.fill({ 0 });
      VelocityMap_PrevY.fill({ 0 });

      //Allocate Terrain map
      Terrain.allocate(MapSize.x, MapSize.y);
      Terrain.fill(AIR);

      WaterMap.allocate(MapSize.x, MapSize.y);
      WaterMap.fill(0.0f);

      SpawnThreads();

      std::cout << "Thermodynamics Initialized." << std::endl;
      return true;
    }

    void ThermodynamicsSystem::Add_Object(FireStarter *obj)
    {
      if (obj)
      {
        glm::ivec2 sub = obj->GetGridPosition();
        FireMap.push_back(std::make_pair(sub, obj));
        Terrain.Set(sub.x, sub.y, Physics::Material(obj->material_type));
        TemperatureMap.Set(sub.x, sub.y, obj->initTemp);
      }
    }

    void ThermodynamicsSystem::Add_Group(FireGroup *obj)
    {
      if (obj)
      {
        for (auto fs : obj->firePoints)
        {
          Add_Object(fs);
        }

        //Add the group if it isn't listed
        fireGroups.push_back(obj);
      }
    }

    // Called every frame
    void ThermodynamicsSystem::Update(const float& dt)
    {
      UpdateMultiThreaded();
      UpdateFire(dt);
     
      //solver.vel_step
      //  (
      //  MapSize.x,
      //  VelocityMapX.GetArray(), VelocityMapY.GetArray(),
      //  VelocityMap_PrevX.GetArray(), VelocityMap_PrevY.GetArray(),
      //  0.0f,
      //  0.1f
      //  );

      //solver.dens_step
      //  (
      //  MapSize.x,
      //  DensityMap.GetArray(), DensityMap_Prev.GetArray(),
      //  VelocityMapX.GetArray(), VelocityMapY.GetArray(),
      //  0.0f,
      //  0.1f
      //  );
    }

    // Getters
    //Get cell temperature
    float ThermodynamicsSystem::GetCellTemperature(int x, int y)
    {
      if (x < 0 || x >= MapSize.x || y < 0 || y >= MapSize.y)
        return AtmosphericTemperature;
      return TemperatureMap.Get(x, y);
    }

    //Get Terrain material
    int ThermodynamicsSystem::GetCellMaterial(int x, int y)
    {
      if (x < 0 || x >= MapSize.x || y < 0 || y >= MapSize.y)
        return AIR;
      return Terrain.Get(x, y);
    }

    //Get cell oxygen content
    float ThermodynamicsSystem::GetCellOxygen(const float& x, const float& y)
    {
      glm::ivec2 sub = GetSubscript(x, y);
      int sub_x = int(sub.x);
      int sub_y = int(sub.y);
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
        return 0.f;
      return DensityMap.Get(sub_x, sub_y);
    }
    //Get cell velocity
    vec2 ThermodynamicsSystem::GetCellVelocity(const float& x, const float& y)
    {
      glm::ivec2 sub = GetSubscript(x, y);
      int sub_x = int(sub.x);
      int sub_y = int(sub.y);
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
      {
        return vec2(0, 0);
      }
      return vec2(VelocityMapX.Get(sub_x, sub_y), VelocityMapY.Get(sub_x, sub_y));
    }

    // Setters
    void ThermodynamicsSystem::ToggleAutoDissipation()
    {
      EqualizePressure = !EqualizePressure;
    }

    float ThermodynamicsSystem::SetCellTemperature(const float& x, const float& y, const float& temp, const float& dt)
    {
      int sub_x = int(x);
      int sub_y = int(y);
      float dQ;
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
        dQ = ConductiveHeatTransfer(Const::K_Wood, AtmosphericTemperature, temp, dt, 1);
      else
      {
        dQ = ConductiveHeatTransfer(Const::K_Air, TemperatureMap.Get(sub_x, sub_y), temp, dt, 1);
        float deltaTemp = dTemp(dQ, DensityMap.Get(sub_x, sub_y) * 0.001f, /*Const::c_Air*/100);
        TemperatureMap.Set(sub_x, sub_y, TemperatureMap.Get(sub_x, sub_y) + deltaTemp);
      }
      return dQ;
    }

    void ThermodynamicsSystem::SetCellVelocity(const int x, const int y, vec2 v)
    {
      if (x < 0 || x > MapSize.x || y < 0 || y > MapSize.y)
        return;
      VelocityMapX.Set(x, y, v.x);
      VelocityMapY.Set(x, y, v.y);
    }

    /*-----------------------------------------------------------------------
    // Private Functions
    -----------------------------------------------------------------------*/

    //Initialize material list
    void ThermodynamicsSystem::Init_Materials()
    {
      using namespace Const;
      conductionProperties new_p;
      //air
      new_p.c = c_Air;
      new_p.Density = p_Air;
      new_p.Hc = Hc_Air;
      new_p.isFluid = true;
      new_p.K = K_Air;
      new_p.Volatile = false;
      materialList.push_back(new_p);
      //water
      new_p.c = c_Water;
      new_p.Density = p_Water;
      new_p.Hc = Hc_Water;
      new_p.isFluid = true;
      new_p.K = K_Water;
      new_p.Volatile = false;
      materialList.push_back(new_p);
      //wood
      new_p.c = c_Wood;
      new_p.Density = p_Wood;
      new_p.isFluid = false;
      new_p.IT = IT_Wood;
      new_p.K = K_Wood;
      new_p.Volatile = true;
      materialList.push_back(new_p);
      //soil
      new_p.c = c_Soil;
      new_p.Density = p_Soil;
      new_p.isFluid = false;
      new_p.K = K_drySoil;
      new_p.Volatile = false;
      materialList.push_back(new_p);
      //grass
      new_p.c = c_Wood;
      new_p.Density = p_Wood;
      new_p.isFluid = false;
      new_p.IT = IT_Grass;
      new_p.K = K_Wood;
      new_p.Volatile = true;
      materialList.push_back(new_p);
      //stone
      new_p.c = c_Stone;
      new_p.Density = p_Rock;
      new_p.isFluid = false;
      new_p.K = K_Stone;
      new_p.Volatile = false;
      materialList.push_back(new_p);
      //append more materials in the right order
    }

    //Update temperatures
    void ThermodynamicsSystem::UpdateTemp(const int& start_index, const int& end_index, const float& dt)
    {
      //std::cout << start_index << "\n";
      //std::cout << "Updated Temperature/Density/Pressure" << std::endl;
      for (int j = start_index; j < end_index; ++j)
      {
        for (int i = 0; i < MapSize.x; ++i)
        {
          float netdQ = 0.f;
          float oTemp = TemperatureMap.Get(i, j);
          //Loop through surrounding cells
          for (int y = j - 1; y <= j + 1; ++y)
          {
            for (int x = i - 1; x <= i + 1; ++x)
            {
              if (x != i || y != j)
              {
                if (x < MapSize.x && x >= 0 && y < MapSize.y && y >= 0)
                {
                  float dQ = ConductiveHeatTransfer(materialList[Terrain.Get(i, j)].K,
                    TemperatureMap.Get(i, j), TemperatureMap.Get(x, y), dt, 1.0f);
                  netdQ += dQ;
                  float oTemp = TemperatureMap.Get(x, y);
                  TemperatureMap.Set(x, y, TemperatureMap.Get(x, y) - dTemp(dQ, DensityMap.Get(x, y) * 1.0f, Const::c_Air));

                  float factor = TemperatureMap.Get(x, y) / oTemp;
                  //DensityMap.Set (x, y, DensityMap.Get (x, y) / factor);
                }
                else
                {
                  float dQ = ConductiveHeatTransfer(materialList[Terrain.Get(i, j)].K,
                    TemperatureMap.Get(i, j), AtmosphericTemperature, dt, 1.0f);
                  netdQ += dQ;
                }
              }
            }
          }
          if (j < MapSize.y - 1)
          {
            if (materialList[Terrain.Get(i, j)].isFluid && materialList[Terrain.Get(i, j + 1)].isFluid)
            {
              float dQConv = ConvectiveHeatTransfer(materialList[Terrain.Get(i, j)].Hc,
                TemperatureMap.Get(i, j), TemperatureMap.Get(i, j + 1), dt);
              float oTempConv = TemperatureMap.Get(i, j + 1);
              netdQ += dQConv;
              TemperatureMap.Set(i, j + 1, TemperatureMap.Get(i, j + 1) - dTemp(dQConv, DensityMap.Get(i, j + 1) * 1.0f, Const::c_Air));
              float factor2 = TemperatureMap.Get(i, j + 1) / oTempConv;
              //DensityMap.Set(i, j + 1, DensityMap.Get(i, j) / factor2);
            }
          }
          else
          {
            if (Terrain.Get(i, j) == 0 && Terrain.Get(i, j + 1) == 0)
            {
              float dQConv = ConvectiveHeatTransfer(materialList[Terrain.Get(i, j)].Hc,
                TemperatureMap.Get(i, j), AtmosphericTemperature, dt);
              netdQ += dQConv;
            }
          }
          TemperatureMap.Set(i, j, TemperatureMap.Get(i, j) + dTemp(netdQ, DensityMap.Get(i, j) * 1.0f, Const::c_Air));
          float factor1 = TemperatureMap.Get(i, j) / oTemp;
          //DensityMap.Set(i, j, DensityMap.Get(i, j) / factor1);
        }//for
      }//for
    }//function

    //Update velocity vectors
    void ThermodynamicsSystem::ComputeVelocity(const int& start_index, const int& end_index, const float& dt)
    {

    }

    /* glm::vec2 ThermodynamicsSystem::GetConvecDir(const unsigned i, const unsigned j)
     {

     }*/

    //Update fire
    void ThermodynamicsSystem::UpdateFire(const float& dt)
    {
      if (fireGroups.size() == 0)
        return;

      int numTreesLeft = 0;
      int numTreesStart = 0;
      for (auto fg : fireGroups)
      {
        //printf("Percentage of fuel unused: %f\n", (manager->numTreesLeft / (float)manager->numTreesStart));
        if (fg && fg->gameObject && fg->firePoints.size())
        {
          ++numTreesStart;
          if (!fg->onFire)
            ++numTreesLeft;
        }
      }

      std::cout << CinderConsole::green;
      if (numTreesLeft == numTreesStart)
      {
        guiText->text = "Burn things to keep the flame alive.";
        //TODO_AUDIO: Play HUD update sound.
      }
      else
      {
        guiText->text = "Trees Remaining: " + std::to_string(numTreesLeft);
        //TODO_AUDIO: Play HUD update sound.
      }
      std::cout << CinderConsole::red;

      static bool treesburned = false; // Make this event only happen once per level swtich
      if (numTreesLeft == 0)
      {
        if (treesburned == false)
        {
          //TODO_AUDIO: play victory sound;
          treesburned = true;
          BaseEvent b;
          EVENTSYSTEM->TriggerEvent(Events::ALLLTREESBURNED, b);
          return;
        }
      }
      else
      {
        treesburned = false;
      }

      for (auto i = FireMap.begin(); i != FireMap.end(); ++i)
      {
        if ((*i).second->onFire)
        {
          if (TemperatureMap.Get((*i).first.x, (*i).first.y) < Const::BT_Organics)
          {
            TemperatureMap.Set((*i).first.x, (*i).first.y,
              TemperatureMap.Get((*i).first.x, (*i).first.y) + dt);
          }
          (*i).second->Update(dt);
        }
        //Fire triangle consists of 3 elements :
        //Oxygen, Fuel and Heat. Test if all 3 are
        //present to decide whether or not to set
        //an object on fire.
        float cur_temp = TemperatureMap.Get((*i).first.x, (*i).first.y);
        float ign_temp = materialList[(*i).second->material_type].IT;
        float fuel_left = (*i).second->Fuel;
        bool oxygen = true;
        if (Terrain.Get((*i).first.x, (*i).first.y + 1) == AIR)
          oxygen = true;
        //if all 3 conditions are satisfied light fire
        if (cur_temp >= ign_temp && fuel_left > 0.0f && oxygen)
        {
          (*i).second->LightOnFire();
        }
        //otherwise douse fire
        else
        {
          (*i).second->DouseFire();
        }
      } //for
    }//function

    void ThermodynamicsSystem::Reset()
    {
      AtmosphericTemperature = 300.f;
      TemperatureMap.fill(300.f);

      DensityMap.fill(Const::p_Air);

      VelocityMapX.fill({ 0 });
      VelocityMapY.fill({ 0 });
      VelocityMap_PrevX.fill({ 0 });
      VelocityMap_PrevY.fill({ 0 });

      fireGroups.clear();

      for (unsigned i = 0; i < FireMap.size(); ++i)
      {
        FireMap.at(i).second->DouseFire();
      }
      FireMap.clear();
    }

    glm::ivec2 ThermodynamicsSystem::GetSubscript(const float &x, const float &y)
    {
      int sub_x = int(std::abs(((x)* (MapSize.x / 2 - 1) + MapOffset.x - 1)));
      int sub_y = int(std::abs(((y)* (MapSize.y / 2 - 1) + MapOffset.y - 1)));
      glm::ivec2 res(sub_x, sub_y);
      return res;
    }

    void ThermodynamicsSystem::SpawnThreads()
    {
      // Temperature Threads
      for (int i = 0; i < kNumThreads; ++i)
      {
        eventStartTemperature[i] = CreateEvent(NULL, false, NULL, NULL);
        eventEndTemperature[i] = CreateEvent(NULL, true, NULL, NULL);
        m_TemperatureThreads[i] = CreateThread(NULL, 0, UpdateTemperatureFunc, NULL, 0, NULL);
      }

      // Velocity Threads
      for (int i = 0; i < kNumThreads; ++i)
      {
        eventStartVelocity[i] = CreateEvent(NULL, false, NULL, NULL);
        eventEndFire[i] = CreateEvent(NULL, true, NULL, NULL);
        m_VelocityThreads[i] = CreateThread(NULL, 0, UpdateVelocityFunc, NULL, 0, NULL);
      }

      // Fire Threads
      for (int i = 0; i < kNumThreads; ++i)
      {
        eventStartFire[i] = CreateEvent(NULL, false, NULL, NULL);
        eventEndFire[i] = CreateEvent(NULL, true, NULL, NULL);
        m_FireThreads[i] = CreateThread(NULL, 0, UpdateFireFunc, NULL, 0, NULL);
      }
    }

    void ThermodynamicsSystem::ReleaseThreads()
    {
      for (int i = 0; i < kNumThreads; ++i)
      {
        CloseHandle(eventStartTemperature[i]);
        CloseHandle(eventEndTemperature[i]);
        CloseHandle(m_TemperatureThreads[i]);
      }

      for (int i = 0; i < kNumThreads; ++i)
      {
        CloseHandle(eventStartVelocity[i]);
        CloseHandle(eventEndVelocity[i]);
        CloseHandle(m_VelocityThreads[i]);
      }

      for (int i = 0; i < kNumThreads; ++i)
      {
        CloseHandle(eventStartFire[i]);
        CloseHandle(eventEndFire[i]);
        CloseHandle(m_FireThreads[i]);
      }
    }

    void ThermodynamicsSystem::UpdateMultiThreaded()
    {
      // Start Temperature Threads
      for (int i = 0; i < kNumThreads; ++i)
      {
        SetEvent(eventStartTemperature[i]);
      }

      // Start Temperature Threads
      for (int i = 0; i < kNumThreads; ++i)
      {
        SetEvent(eventStartVelocity[i]);
      }

      // Start Temperature Threads
      /*for (int i = 0; i < kNumThreads; ++i)
      {
      SetEvent (eventStartFire [i]);
      }*/

      WaitForMultipleObjects(kNumThreads, eventEndTemperature, true, INFINITE);
      WaitForMultipleObjects(kNumThreads, eventEndVelocity, true, INFINITE);
      //WaitForMultipleObjects (kNumThreads, eventEndFire, true, INFINITE);

      for (int i = 0; i < kNumThreads; ++i)
      {
        ResetEvent(eventEndTemperature[i]);
      }
      for (int i = 0; i < kNumThreads; ++i)
      {
        ResetEvent(eventEndVelocity[i]);
      }
      /*for (int i = 0; i < kNumThreads; ++i)
      {
      ResetEvent (eventEndFire [i]);
      }*/
    }

    void ThermodynamicsSystem::Draw()
    {
      glUseProgram(0);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(Camera::main->GetFOV(), (float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height(), 0, 100.0f);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      glm::vec3 eye = glm::vec3(0, 0, 1) * Camera::main->GetSize() + glm::vec3(Camera::main->gameObject->Transform->GetPosition().x, Camera::main->gameObject->Transform->GetPosition().y, 0);
      glm::vec3 center = Camera::main->gameObject->Transform->GetPosition();
      glm::vec3 up = glm::vec3(0, 1, 0);
      gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
      //glTranslatef (0, 32, 0);
      //glScalef (64, 64, 1.0f);
      glBegin(GL_QUADS);
      {
        for (int i = 0; i < Terrain.getSize().y; ++i)
        {
          for (int j = 0; j < Terrain.getSize().x; ++j)
          {
            glColor4f(TemperatureMap.Get(j, i) / Constant::BT_Organics,
              TemperatureMap.Get(j, i) / Constant::BT_Organics,
              TemperatureMap.Get(j, i) / Constant::BT_Organics,
              TemperatureMap.Get(j, i) / Constant::BT_Organics * 0.4f);
            glVertex2f(j - (MapSize.x * 0.5f) - 1, i - (MapSize.y * 0.5f) - 1);
            glVertex2f(j - (MapSize.x * 0.5f) - 2, i - (MapSize.y * 0.5f) - 1);
            glVertex2f(j - (MapSize.x * 0.5f) - 2, i - (MapSize.y * 0.5f) - 2);
            glVertex2f(j - (MapSize.x * 0.5f) - 1, i - (MapSize.x * 0.5f) - 2);
          }
        }
      }
      glEnd();

      //glColor4f (1, 1, 1, 0.2f);
      //glBegin (GL_QUADS);
      //{
      //  for (unsigned i = 0; i < TerrainPoints.size (); i+=4)
      //  {
      //    glVertex2f (TerrainPoints.at (i).x, TerrainPoints.at (i).y);
      //    glVertex2f (TerrainPoints.at (i + 1).x, TerrainPoints.at (i + 1).y);
      //    glVertex2f (TerrainPoints.at (i + 2).x, TerrainPoints.at (i + 2).y);
      //    glVertex2f (TerrainPoints.at (i + 3).x, TerrainPoints.at (i + 3).y);
      //  }
      //}
      //glEnd ();

      //glBegin (GL_QUADS);
      //{
      //  for (int i = 0; i < MapSize.y; ++i)
      //  {
      //    for (int j = 0; j < MapSize.x; ++j)
      //    {
      //      glColor4f (0, 0, 0, 0);
      //      glVertex2f (j, i);
      //      glVertex2f (j - 1, i);
      //      glVertex2f (j - 1, i - 1);
      //      glVertex2f (j, i - 1);
      //    }
      //  }
      glPointSize(10.0f);
      glBegin(GL_POINTS);
      FireStarter* firePoint;
      for (auto i = FireMap.begin(); i != FireMap.end(); ++i)
      {


        firePoint = (*i).second;

        if (firePoint)
        {
          if (firePoint->onFire)
          {
            glColor4f(1, 0, 0, 1.0f);
          }
          else
          {
            glColor4f(0, 1, 0, 1.0f);
          }
          vec2 pos = firePoint->GetPosition();

          glVertex2f(pos.x, pos.y);
        }
        //glVertex2f ((*i).first.x - 1, (*i).first.y);
        //glVertex2f ((*i).first.x - 1, (*i).first.y - 1);
        //glVertex2f ((*i).first.x, (*i).first.y - 1);
      }
      glEnd();
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
    } //draw function

  }//namespace Physics
}//namespace Framework

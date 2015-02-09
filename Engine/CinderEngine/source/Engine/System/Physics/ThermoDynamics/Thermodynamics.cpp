/******************************************************************************/
/*!
\file   Thermodynamics.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <Precompiled.h>
#include "Thermodynamics.h"
#include "TDLib.h"
#include "AudioSystem.h"
#include "TerrainCreator.h"
#include "FractalNoise.h"
#include "ThreadFunctions.h"
#include "WindowSystem.h"
#include "Camera.h"
#include "AudioEvents.h"
#include "Events.h"

#include "ResourceManager.h" //Used for playing victory sound

//Used for sending all trees event
#include "EventSystem.h" 
#include "GameEvent.h"

namespace Framework
{
  namespace Physics
  {
    static float time = 0.f;
    static const int Update_Width = 32;
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
      CellSize = 1.f;
      THERMODYNAMICS = this;
    }

    bool ThermodynamicsSystem::UpdatesOnPaused()
    {
      return false;
    }

    //Destructor
    ThermodynamicsSystem::~ThermodynamicsSystem()
    {
      EVENTSYSTEM->mDisconnect <KeyEvent, ThermodynamicsSystem>(Events::KEY_ANY, this, &ThermodynamicsSystem::OnKeyPressed);
      Clear();
      //ReleaseThreads();
    }

    /*-----------------------------------------------------------------------
    // Public Functions
    -----------------------------------------------------------------------*/

    bool ThermodynamicsSystem::Initialize()
    {
#ifdef _DEBUG 
      EVENTSYSTEM->mConnect <KeyEvent, ThermodynamicsSystem>(Events::KEY_ANY, this, &ThermodynamicsSystem::OnKeyPressed);
#endif
      /*if (guiText == nullptr)
      {
        GameObject* go = new GameObject(10000);
    Component* tsfm = go->AddComponent("Transform");
    tsfm->Initialize();

        guiText = reinterpret_cast<GUIText*> (go->AddComponent("GUIText"));
        guiText->position = { -0.2f, -0.9f };
        guiText->Initialize();
      }*/

      simulation_speed = 10;
      EqualizePressure = true;
      viscosity = Const::visc_Air;

      //Initialize material list
      Init_Materials();
      y_offset = nullptr;

      Allocated = false;

      //Initialize solver
      //solver.Obstacles = &Terrain;
      //solver.y_offset = y_offset;

      //SpawnThreads();

      std::cout << "Thermodynamics Initialized." << std::endl;
      return true;
    }

    void ThermodynamicsSystem::OnKeyPressed(KeyEvent* key)
    {
      if (key->KeyDown)
        switch (key->KeyValue)
      {
        case GLFW_KEY_P:
          EqualizePressure = !EqualizePressure;
          break;
        default:
          break;
      }
    }

    void ThermodynamicsSystem::Add_Object(FireStarter *obj)
    {
      if (obj)
      {
        glm::ivec2 sub = obj->GetGridPosition();
        sub.y -= y_offset[sub.x];
        FireMap.push_back(std::make_pair(sub, obj));
        Terrain.Set(sub.x, sub.y, Physics::MaterialType(obj->material_type));
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
      //test
      time += dt;
      TemperatureMap.Set(65, 5, std::abs(3000.f * std::cos(time)));
      VelocityMapY.Set(65, 5, 100.f);
      VelocityMapY.Set(60, 5, 100.f);
      //VelocityMapY.Set(65, 5, std::abs(100.f * std::cos(time / 1.f)));
      //VelocityMapX.Set(65, 5, 100.f * std::sin(time / 1.f));
      if (paused)
        return;

      int center = static_cast<Transform*>(Camera::main->gameObject->GetComponent("Transform"))->GetGridPosition().x;
      float fov = Camera::main->GetSize();

      int start = center - Update_Width;
      if (start < 0)
        start = 0;
      int end = start + 2 * Update_Width;
      if (end >= MapSize.x)
        end = MapSize.x - 1;
      
      //Update velocity field
      vel_step(start, end, dt);
      //Update temperature field
      temp_step(start, end, dt);
      //Update fire
      UpdateFire(dt);
    }

    // Getters
    //Get cell temperature
    float ThermodynamicsSystem::GetCellTemperature(int x, int y)
    {
      y -= y_offset[x];
      if (x < 0 || x >= MapSize.x || y < 0 || y >= MapSize.y)
        return AtmosphericTemperature;
      return TemperatureMap.Get(x, y);
    }

    //Get Terrain material
    int ThermodynamicsSystem::GetCellMaterial(int x, int y)
    {
      y -= y_offset[x];
      if (x < 0 || x >= MapSize.x || y < 0 || y >= MapSize.y)
        return AIR;
      return Terrain.Get(x, y);
    }

    //Get cell oxygen content
    float ThermodynamicsSystem::GetCellOxygen(const float& x, const float& y)
    {
      glm::ivec2 sub(x, y);
      int sub_x = int(sub.x);
      int sub_y = int(sub.y);
      if (sub_x < 0 || sub_x > MapSize.x || sub_y < 0 || sub_y > MapSize.y)
        return 0.f;
      return DensityMap.Get(sub_x, sub_y);
    }
    //Get cell velocity
    vec2 ThermodynamicsSystem::GetCellVelocity(int x, int y)
    {
      y -= y_offset[x];
      if (x < 0 || x > MapSize.x || y < 0 || y > MapSize.y)
      {
        return vec2(0, 0);
      }
      return vec2(VelocityMapX.Get(x, y), VelocityMapY.Get(x, y));
    }

    // Setters
    void ThermodynamicsSystem::SetMapSize(int size_x, int size_y)
    {
      Clear();
      //Scan level
      MapSize = { size_x, size_y };
      std::cout << "Grid " << MapSize.x << "x " << MapSize.y << std::endl;
      MapOffset = { -MapSize.x / 2, MapSize.y / 2 };
      AtmosphericTemperature = 300.f;
      //Allocate heatmap
      TemperatureMap.allocate(MapSize.x, MapSize.y);
      TemperatureMap.fill(AtmosphericTemperature);
      TemperatureMap_Prev.allocate(MapSize.x, MapSize.y);
      TemperatureMap_Prev.fill(AtmosphericTemperature);

      //Allocate Oxygen/Density map
      DensityMap.allocate(MapSize.x + 2, MapSize.y + 2);
      DensityMap.fill(Constant::K_Air);

      //Allocate Velocity map
      VelocityMapX.allocate(MapSize.x + 2, MapSize.y + 2);
      VelocityMapY.allocate(MapSize.x + 2, MapSize.y + 2);
      VelocityMap_PrevX.allocate(MapSize.x + 2, MapSize.y + 2);
      VelocityMap_PrevY.allocate(MapSize.x + 2, MapSize.y + 2);
      VelocityMapX.fill({ 0.f });
      VelocityMapY.fill({ 0.f });
      VelocityMap_PrevX.fill({ 0.f });
      VelocityMap_PrevY.fill({ 0.f });

      //Allocate Terrain map
      Terrain.allocate(MapSize.x, MapSize.y);
      Terrain.fill(AIR);

      y_offset = new int[MapSize.x];

      Allocated = true;
    }

    void ThermodynamicsSystem::ToggleAutoDissipation()
    {
      EqualizePressure = !EqualizePressure;
    }

    float ThermodynamicsSystem::SetCellTemperature(int x, int y, const float temp, const float dt)
    {
      y -= y_offset[x];
      float dQ;
      if (x < 0 || x > MapSize.x || y < 0 || y > MapSize.y)
        dQ = ConductiveHeatTransfer(Const::K_Wood, AtmosphericTemperature, temp, dt, 1);
      else
      {
        dQ = ConductiveHeatTransfer(Const::K_Air, TemperatureMap.Get(x, y), temp, dt, 1);
        float deltaTemp = dTemp(dQ, DensityMap.Get(x, y) * 1.f, Const::c_Air);
        TemperatureMap.Set(x, y, TemperatureMap.Get(x, y) + deltaTemp);
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

    /* glm::vec2 ThermodynamicsSystem::GetConvecDir(const unsigned i, const unsigned j)
     {

     }*/

    //Update fire
    void ThermodynamicsSystem::UpdateFire(const float& dt)
    {
      //This needs to be somewhere else. Not in thermo.
      //-----------------------------------------------
      //if (fireGroups.size() == 0)
      //  return;

      //int numTreesLeft = 0;
      //int numTreesStart = 0;
      //for (auto fg : fireGroups)
      //{
      //  //printf("Percentage of fuel unused: %f\n", (manager->numTreesLeft / (float)manager->numTreesStart));
      //  if (fg && fg->gameObject && fg->firePoints.size())
      //  {
      //    ++numTreesStart;
      //    if (!fg->onFire)
      //      ++numTreesLeft;
      //  }
      //}

      //std::cout << CinderConsole::green;
      //if (numTreesLeft == numTreesStart)
      //{
      //  guiText->text = "Burn things to keep the flame alive.";
      //  //TODO_AUDIO: Play HUD update sound.
      //}
      //else
      //{
      //  guiText->text = "Trees Remaining: " + std::to_string(numTreesLeft);
      //  //TODO_AUDIO: Play HUD update sound.
      //}
      //std::cout << CinderConsole::red;

      //static bool treesburned = false; // Make this event only happen once per level swtich
      //if (numTreesLeft == 0)
      //{
      //  if (treesburned == false)
      //  {
      //    //TODO_AUDIO: play victory sound;
      //    Sound* winFX = Resources::RS->Get_Sound("fx_win.ogg");
      //    AUDIOEVENTS->unmanagedSounds.push_back(winFX);
      //    winFX->Play();
      //    treesburned = true;
      //    BaseEvent b;
      //    EVENTSYSTEM->TriggerEvent(Events::ALLLTREESBURNED, b);
      //    return;
      //  }
      //}
      //else
      //{
      //  treesburned = false;
      //}

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
      if (Allocated)
      {
        TemperatureMap.fill(AtmosphericTemperature);
        TemperatureMap_Prev.fill(AtmosphericTemperature);

        DensityMap.fill(Const::p_Air);

        VelocityMapX.fill({ 0 });
        VelocityMapY.fill({ 0 });
        VelocityMap_PrevX.fill({ 0 });
        VelocityMap_PrevY.fill({ 0 });
      }

      fireGroups.clear();

      for (unsigned i = 0; i < FireMap.size(); ++i)
      {
        FireMap.at(i).second->DouseFire();
      }
      FireMap.clear();
    }

    void ThermodynamicsSystem::Clear()
    {
      TemperatureMap.clean();
      TemperatureMap_Prev.clean();
      DensityMap.clean();
      VelocityMapX.clean();
      VelocityMap_PrevX.clean();
      VelocityMapY.clean();
      VelocityMap_PrevY.clean();
      Terrain.clean();
      
      fireGroups.clear();
      FireMap.clear();

      if (y_offset != nullptr)
        delete[] y_offset;

      y_offset = nullptr;

      Allocated = false;
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
    //Debug draw
    void ThermodynamicsSystem::Draw()
    {

      Transform* tform = static_cast<Transform*>(Camera::main->gameObject->GetComponent("Transform"));
      glm::vec2 c_center = tform->GetGridPosition();
      float fov = Camera::main->GetSize();
      int h_start = c_center.x - fov;
      if (h_start < 0)
        h_start = 0;
      int h_end = c_center.x + fov;
      if (h_end >= MapSize.x)
        h_end = MapSize.x - 1;
      int v_start = 0;// c_center.y - fov;
      if (v_start < 0)
        v_start = 0;
      int v_end = MapSize.y;// c_center.y + fov;
      if (v_end >= MapSize.y)
        v_end = MapSize.y - 1;
      glUseProgram(0);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(Camera::main->GetFOV(), (float)WINDOWSYSTEM->Get_Width() / WINDOWSYSTEM->Get_Height(), 0, 100.0f);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      glm::vec3 eye = glm::vec3(0, 0, 1) * Camera::main->GetSize() + glm::vec3(tform->GetPosition().x, tform->GetPosition().y, 0);
      glm::vec3 center = tform->GetPosition();
      glm::vec3 up = glm::vec3(0, 1, 0);
      gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
      
      //draw Terrain
      glBegin(GL_QUADS);
      {
        for (int i = v_start; i < v_end; ++i)
        {
          for (int j = h_start; j < h_end; ++j)
          {
            glColor4f(float(Terrain.Get(j, i) / STONE),
              float(Terrain.Get(j, i)) / STONE,
              float(Terrain.Get(j, i)) / STONE,
              0.5f);
            glVertex2f(j - (MapSize.x * 0.5f) + 2, i - 0 + y_offset[j]);
            glVertex2f(j - (MapSize.x * 0.5f) + 3, i - 0 + y_offset[j]);
            glVertex2f(j - (MapSize.x * 0.5f) + 3, i + 1 + y_offset[j]);
            glVertex2f(j - (MapSize.x * 0.5f) + 2, i + 1 + y_offset[j]);
          }
        }
      }
      glEnd();
      //draw temperature
      glBegin(GL_QUADS);
      {
        for (int i = v_start; i < v_end; ++i)
        {
          for (int j = h_start; j < h_end; ++j)
          {
            glColor4f(TemperatureMap.Get(j, i) / Constant::BT_Organics, 0.f, 0.f,
              TemperatureMap.Get(j, i) / Constant::BT_Organics);
            glVertex2f(j - (MapSize.x * 0.5f) + 2, i - 0 + y_offset[j]);
            glVertex2f(j - (MapSize.x * 0.5f) + 3, i - 0 + y_offset[j]);
            glVertex2f(j - (MapSize.x * 0.5f) + 3, i + 1 + y_offset[j]);
            glVertex2f(j - (MapSize.x * 0.5f) + 2, i + 1 + y_offset[j]);
          }
        }
      }
      glEnd();

      //draw wind
      glBegin(GL_LINES);
      {
        for (int i = v_start; i < v_end; ++i)
        {
          for (int j = h_start; j < h_end; ++j)
          {
            float x_off = VelocityMapX.Get(j, i) / 50.f;
            float y_off = VelocityMapY.Get(j, i) / 50.f;
            glColor4f(1.f, 1.f, 1.f, 1.f);
            glVertex2f(j - (MapSize.x * 0.5f) + 2.5f, i + 0.5f + y_offset[j]); //Base
            glVertex2f(j - (MapSize.x * 0.5f) + 2.5f + x_off, i + 0.5f + y_offset[j] + y_off); //Tip
          }
        }
      }
      glEnd();


      glPointSize(10.0f);
      //draw fire starters
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

      glBegin(GL_LINES);
      {
        int c = c_center.x;
        int hstart = c - Update_Width;
        if (hstart < 0)
          hstart = 0;
        int hend = hstart + 2 * Update_Width;
        if (hend > MapSize.x - 1)
          hend = MapSize.x - 1;
        //left
        glColor4f(1.f, 0.25f, 0.5f, 1.f);
        glVertex2f(hstart - (MapSize.x * 0.5f) + 2.f, y_offset[hstart]); //Base
        glVertex2f(hstart - (MapSize.x * 0.5f) + 2.f, y_offset[hstart] + MapSize.y); //Tip
        //right
        glColor4f(1.f, 0.25f, 0.5f, 1.f);
        glVertex2f(hend - (MapSize.x * 0.5f) + 2.f, y_offset[hend]); //Base
        glVertex2f(hend - (MapSize.x * 0.5f) + 2.f, y_offset[hend] + MapSize.y); //Tip
      }
      glEnd();

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
    } //draw function

  }//namespace Physics
}//namespace Framework

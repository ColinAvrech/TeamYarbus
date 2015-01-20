/******************************************************************************/
/*!
\file   Core.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains all functionality to start and run the gameloop. This includes

Adding Systems.
Initializeing all systems.
Updating all systems.
Destorying all Systems.
Main Game Loop.
*/
/******************************************************************************/

#include <Precompiled.h>
#ifdef _DEBUG
#include "Profiler\SamplingProfiler.h"
#endif

namespace Framework
{
  //! Global pointer to the Engine Core
  CoreEngine* CORE;

#ifdef _DEBUG
  vector<SamplingProfiler *> vecProfilers;
  void CALLBACK ProfilerCallback(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR)
  {
    for (auto gProfiler : vecProfilers)
    {
      if (!gProfiler->IsFull())
      {
        gProfiler->TakeSample();
      }
      else if (!gProfiler->IsExported())
      {
        gProfiler->SetExported();
      }
      else
      {
        gProfiler->Exit();
      }
    }
  }
#endif

  ZilchDefineType(CoreEngine, CinderZilch)
  {
	  
	  //ZilchBindMethod(SetPaused);
  }

  CoreEngine::CoreEngine()
  {
    CORE = this;
    GameActive = true;
    GamePaused = false;
  }

  CoreEngine::~CoreEngine()
  {
    //! probs nothing to destory, but we humans have a distructive nature...
    //! So it is here for when we need to destory something.
#ifdef _DEBUG
    if (vecProfilers.size() > 0)
    {
      vecProfilers.front()->SetFull(); //Used to stop the game profiler from collecting more samples;
      
      if (!vecProfilers.back()->IsFull())
        vecProfilers.back()->SetFull();
      
      for (auto gProfiler : vecProfilers)
      {
        delete gProfiler;
        gProfiler = nullptr;
      }
      vecProfilers.clear();
    }
#endif

    DestroySystems();
  }

  //!Update all the systems
  void CoreEngine::GameLoop()
  {
    //required for deterministic physics
    float accumulator = 0;
    float frameStart = (float)GetCurrentTime();
    float dt = 0.016f;
    
    //!Gameloop
    while (GameActive)
    {
      if (WINDOWSYSTEM->IsInFocus())
      {
        //! Marks the begining of a frame
        StartGameLoop_dt();
     
        for (unsigned i = 0; i < Systems.size(); ++i)
        {
          if (!GamePaused || GamePaused && Systems[i]->UpdatesOnPaused())
          {
            //printf("%s updating. \n", Systems[i]->GetName().c_str());
            Systems[i]->Update(dt);//_dt);
          }

        }

        //!FPS limiter, FPS define can be found in Core.h
        FrameLimiter();
        //! marks the end of a frame and calculates the dt, average dt
        EndGameLoop_dt();
      }
      else
      {
        WINDOWSYSTEM->Update(dt);
      }
    }
  }

  //!Destroy all system in reverse order that they were added
  void CoreEngine::DestroySystems()
  {
    /*!Delete all system in reverse order.
    This is done to reduce the probability of
    dependency problems between systems.*/
    while (Systems.size() != 0)
    {
      delete Systems[Systems.size() - 1];
      Systems[Systems.size() - 1] = nullptr;
      Systems.pop_back();
    }
  }

  //! Add a system to be updated every frame.
  void CoreEngine::AddSystem(BaseSystem* system)
  {
    /*! Add a system to the Vecter
        This will allow us to run all systems in
        rapid sucession. */
    Systems.push_back(system);
  }

  //! initializes all systems in the game.
  void CoreEngine::Initialize()
  {
    for (unsigned i = 0; i < Systems.size(); ++i)
      Systems[i]->Initialize();

    //vecProfilers.push_back(new SamplingProfiler(0)); //0 collects samples until told to stop
  }

  void CoreEngine::QuitGame()
  {
    GameActive = false;
  }
  
  //!
  void CoreEngine::FrameLimiter()
  {
    //! If we are ahead of schedule, rest for a while.
    if (_dt < (1 / FPS))
    {
      std::chrono::nanoseconds sleepTime(static_cast<unsigned>( ((1/FPS) - _dt) * (1000*1000*1000) ));
      std::this_thread::sleep_for(sleepTime);
    }
  }

  //!Call at the begining of every gameloop
  void CoreEngine::StartGameLoop_dt(void)
  {
    _start_tp = high_resolution_clock::now();
  }

  //!Call at the end of every gameloop
  void CoreEngine::EndGameLoop_dt(void)
  {
    _end_tp = high_resolution_clock::now();

    //!Get the number of ticks from teach time_point
    long long start_ticks = _start_tp.time_since_epoch().count();
    long long end_ticks = _end_tp.time_since_epoch().count();

    //!The length of time between each tick. the Period.
    const double period = high_resolution_clock::period::num
      / static_cast<double>(high_resolution_clock::period::den);

    /*!Calculate the delta time by multiplying the number of ticks by
    the length of time per tick (Period)  */
    _dt = static_cast<double>(end_ticks - start_ticks) * period;

    /*!The average system dt is affected more by recent deltas*/
    _dt_ave = (11 * _dt_ave + _dt) / 12;
  }

  void CoreEngine::TogglePaused ()
  {
    GamePaused = !GamePaused;
    PauseEvent pause;
    pause.Paused = GamePaused;
    EVENTSYSTEM->TriggerEvent (Events::PAUSE, pause);
  }

  void CoreEngine::SetPaused(bool state)
  {
	  GamePaused = state;
	  PauseEvent pause;
	  pause.Paused = GamePaused;
	  EVENTSYSTEM->TriggerEvent(Events::PAUSE, pause);
  }

#ifdef _DEBUG
  void CoreEngine::ToggleProfiling()
  {
    if (!vecProfilers.empty())
    {
      auto gProfiler = vecProfilers.back();
      if (gProfiler->GetID() != 0 && !gProfiler->IsFull()) //id 0 is reserved for the profiler that is run for the entire lifetime of the game.
      {
        gProfiler->SetFull();
        return;
      }
    }
    vecProfilers.push_back(new SamplingProfiler(NOMAX)); //0 collects samples until told to stop
    // 10000 is the default max number of samples to collect. For a fuller profile increase this number and for a quicker report decrease it.
  }
#endif
}
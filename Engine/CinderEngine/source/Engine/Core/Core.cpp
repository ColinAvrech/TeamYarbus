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


#include "Core.h"

namespace Framework
{
  //! Global pointer to the Engine Core
  CoreEngine* CORE;

  CoreEngine::CoreEngine()
  {
    CORE = this;
    GameActive = true;
  }

  CoreEngine::~CoreEngine()
  {
    //! probs nothing to destory, but we humans have a distructive nature...
    //! So it is here for when we need to destory something.
  }

  //!Update all the systems
  void CoreEngine::GameLoop()
  {
	//required for deterministic physics
	float accumulator = 0;
	float frameStart = float (GetCurrentTime());

    //!Gameloop
    while (GameActive)
    {
      //! Marks the begining of a frame
      StartGameLoop_dt();
      
	  for (unsigned i = 0; i < Systems.size(); ++i)
	  {
		  Systems[i]->Update(_dt);
	  }

      //!FPS limiter, FPS define can be found in Core.h
      FrameLimiter();
      //! marks the end of a frame and calculates the dt, average dt
      EndGameLoop_dt();
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

}
/******************************************************************************/
/*!
\file   Core.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Core handles all the engine's main functionality. 
*/
/******************************************************************************/

#pragma once



//! The highest FPS the game can run at.
#define FPS 60.0f

using namespace std::chrono;


namespace Framework
{
  class CoreEngine
  {
  public:
    CoreEngine();
    ~CoreEngine();
	ZilchDeclareBaseType(CoreEngine, Zilch::TypeCopyMode::ReferenceType);
    //!Update all the systems
    void GameLoop();

    //!Destroy all system in reverse order that they were added
    void DestroySystems();

    //! Add a system to be updated every frame.
    void AddSystem(BaseSystem* system);

    void TogglePaused();
    bool IsPaused(){ return GamePaused; }
	void CoreEngine::SetPaused(bool state);
    //! Stops the game for whatever reason
    void QuitGame();

    //! initializes all systems in the game.
    void Initialize();

  private:

    //!Hold pointers to all system in the game
    std::vector<BaseSystem*> Systems;
    //!Is the game running (true) or not (false, shut down)
    bool GameActive;
    bool GamePaused;

    /*!dt for each frame
    A const reference is used for systems
    and can be found in the framework*/
    double _dt = 0.0f;

    /*!average dt for each frame
    A const reference is used for systems
    and can be found in the framework*/
    double _dt_ave = 0.0f;

    //!time points to measure
    high_resolution_clock::time_point _start_tp;
    high_resolution_clock::time_point _end_tp;

    //!Call at the begining of the update
    void StartGameLoop_dt(void);
    //!Call at the end of the update function
    void EndGameLoop_dt(void);
    //!Call to limit the number of FPS
    void FrameLimiter(void);
  };
  
  //! Global Pointer for the Game engine core
  extern CoreEngine* CORE;
}
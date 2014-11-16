/******************************************************************************/
/*!
\file   Core.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Core handles all the engine's main functionality. 

As a condition of your accessing this area, you agree to be bound by the
following terms and conditions :

The games software was created by students of DigiPen Institute of
Technology(DigiPen), and all copyright and other rights in such is owned by
DigiPen.While DigiPen allows you to access, download and use the software
for non - commercial, home use you hereby expressly agree that you will not
otherwise copy, distribute, modify, or(to the extent not otherwise
permitted by law) decompile, disassemble or reverse engineer the games
software.

THE GAMES SOFTWARE IS MADE AVAILABLE BY DIGIPEN AS - IS AND WITHOUT
WARRANTY OF ANY KIND BY DIGIPEN.DIGIPEN HEREBY EXPRESSLY DISCLAIMS ANY SUCH
WARRANTY, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTY OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. WITHOUT LIMITING THE
GENERALITY OF THE FOREGOING, DIGIPEN SHALL NOT BE LIABLE IN DAMAGES OR
OTHERWISE FOR ANY DAMAGE OR INJURY FROM DOWNLOADING, DECOMPRESSING, RUNNING
OR ATTEMPTING TO RUN, USING OR OTHERWISE DEALING WITH, IN ANY WAY, THE
GAMES SOFTWARE CONTAINED IN THIS AREA, NOR SHALL DIGIPEN BE LIABLE FOR ANY
INCIDENTAL, CONSEQUENTIAL, EXEMPLARY OR OTHER TYPES OF DAMAGES ARISING FROM
ACCESS TO OR USE OF THE GAMES SOFTWARE.

YOU HEREBY AGREE TO INDEMNIFY, DEFEND AND HOLD HARMLESS DIGIPEN AND ITS
DIRECTORS, OFFICERS, EMPLOYEES, AGENTS, CONSULTANTS AND CONTRACTORS AGAINST
ALL LIABILITY OF ANY KIND ARISING OUT OF YOUR DOWNLOADING, DECOMPRESSING,
RUNNING OR ATTEMPTING TO RUN, USING OR OTHERWISE DEALING WITH, IN ANY WAY,
THE GAMES SOFTWARE. DIGIPEN MAKES NO WARRANTIES OR REPRESENTATIONS THAT THE
GAMES SOFTWARE IS FREE OF MALICIOUS PROGRAMMING, INCLUDING, WITHOUT
LIMITATION, VIRUSES, TROJAN HORSE PROGRAMS, WORMS, MACROS AND THE LIKE.AS
THE PARTY ACCESSING THE GAMES SOFTWARE IT IS YOUR RESPONSIBILITY TO GUARD
AGAINST AND DEAL WITH THE EFFECTS OF ANY SUCH MALICIOUS PROGRAMMING.
*//******************************************************************************/#pragma once


//! The highest FPS the game can run at.
#define FPS 60.0f

#include "Common.h"
#include "BaseSystem.h"

using namespace std::chrono;


namespace Framework
{
  class CoreEngine
  {
  public:
    CoreEngine();
    ~CoreEngine();

    //!Update all the systems
    void GameLoop();

    //!Destroy all system in reverse order that they were added
    void DestroySystems();

    //! Add a system to be updated every frame.
    void AddSystem(BaseSystem* system);
   
    //! Stops the game for whatever reason
    void QuitGame();

    //! initializes all systems in the game.
    void Initialize();

  private:

    //!Hold pointers to all system in the game
    std::vector<BaseSystem*> Systems;
    //!Is the game running (true) or not (false, shut down)
    bool GameActive;

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
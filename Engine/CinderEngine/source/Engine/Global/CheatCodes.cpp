/******************************************************************************/
/*!
\file   CheatCodes.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Cheats!
*/
/******************************************************************************/

#include <Precompiled.h>
#include "CheatCodes.h"
#include "EventSystem.h"
#include "KeyEvent.h"
#include "GLFW3.h"
#include "Pipeline.h"
#include "PlayerStats.h"
#include "Events.h"
#include "Lantern.h"

namespace Framework
{
  namespace Cheats
  {
    void InitializeCheats()
    {
      EVENTSYSTEM->gConnect<KeyEvent>(Events::KEY_ANY, &CheckToEnableCheats);
    }

    void CheckToEnableCheats(KeyEvent * e)
    {
      if (e->KeyDown)
      {
        GameObject* player = OBJECTSYSTEM->ptrPlayer;

		    Lantern* la = player->C<Lantern>();
        CharacterController* cc = static_cast<CharacterController*>(player->GetComponent("CharacterController"));
        PlayerStats* ps = static_cast<PlayerStats*>(OBJECTSYSTEM->ptrPlayer->GetComponent("PlayerStats"));
        if (e->SHIFTPressed)
        {
          switch (e->KeyValue)
          {
            case GLFW_KEY_F: // Toggle Flying
              if (player && cc)
              {
                cc->ToggleFlying();
              }
              break;

#ifdef _DEBUG
            case GLFW_KEY_P: //start profiler
              CORE->ToggleProfiling();
              break;
#endif

            case GLFW_KEY_R: // Restart Level
              OBJECTSYSTEM->RestartLevel();
              break;

            case GLFW_KEY_L: // Toggle Invincibility:   Shift + L
			      {
								 if (la)
								 {
									 std::cout << "Toggle Invincibility Cheat!" << std::endl;
									 la->ToggleInvulnerability();
								 }
              }
              break;

            case GLFW_KEY_N: // Goto NextLevel: Shift + N

              // Check for player and its next level, or splash screen
              if ((OBJECTSYSTEM->ptrPlayer && ps) ||
                OBJECTSYSTEM->FindObjectByName("Logo") != nullptr)
              {
                std::cout << "Goto NextLevel Cheat!" << std::endl;
                OBJECTSYSTEM->NextLevel();
              }
              break;

            case GLFW_KEY_H: // Enter Debug Mode: H
              OPENGL->ToggleDebugDraw();
              break;
          }
        }
        else if (e->CTRLPressed)
        {
          switch (e->KeyValue)
          {
            case GLFW_KEY_P:
              CORE->TogglePaused();
              break;
          }
        }
      }
    }
  }// namespace Cheats  
}// namespace Framework

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
      if (e->KeyDown && e->SHIFTPressed)
      {
        GameObject* player = OBJECTSYSTEM->ptrPlayer;

        Health* hp = static_cast<Health*>(player->GetComponent("Health"));
        CharacterController* cc = static_cast<CharacterController*>(player->GetComponent("CharacterController"));
        PlayerStats* ps = static_cast<PlayerStats*>(OBJECTSYSTEM->ptrPlayer->GetComponent("PlayerStats"));
        switch (e->KeyValue)
        {
          case GLFW_KEY_F: // Toggle Flying
            if (player && cc)
            {
              cc->ToggleFlying();
            }
            break;

          case GLFW_KEY_P: //start profiler
            CORE->ToggleProfiling();
            break;

          case GLFW_KEY_R: // Restart Level
            OBJECTSYSTEM->RestartLevel();
            break;            

          case GLFW_KEY_I: // Toggle Invincibility:   Shift + I
            if (player && hp)
    	      {
    	        std::cout << "Toggle Invincibility Cheat!" << std::endl;
              hp->ToggleInvulnerability();
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
    }
  }// namespace Cheats  
}// namespace Framework

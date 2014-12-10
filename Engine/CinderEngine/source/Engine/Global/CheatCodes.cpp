/******************************************************************************/
/*!
\file   CheatCodes.cpp
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Cheats!
*/
/******************************************************************************/

#include "CheatCodes.h"
#include "EventSystem.h"
#include "KeyEvent.h"
#include "GLFW3.h"
#include "Pipeline.h"
#include "PlayerStats.h"

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
        
        switch (e->KeyValue)
        {
          case GLFW_KEY_F:
            if (player && player->CharacterController)
            {
              player->CharacterController->ToggleFlying();
            }
            break;

          case GLFW_KEY_I: // Toggle Invincibility:   Shift + I
            if (player && player->Health)
    	      {
    	        std::cout << "Toggle Invincibility Cheat!" << std::endl;
              player->Health->ToggleInvulnerability();
            }
            break;

          case GLFW_KEY_N: // Goto NextLevel: Shift + N

            // Check for player and its next level, or splash screen
            if ((OBJECTSYSTEM->ptrPlayer && OBJECTSYSTEM->ptrPlayer->PlayerStats) ||
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

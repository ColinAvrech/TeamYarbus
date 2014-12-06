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
        GameObject* player;
        switch (e->KeyValue)
        {
          case GLFW_KEY_I: // Toggle Invincibility:   Shift + I
		  	    std::cout << "Toggle Invincibility Cheat!" << std::endl;

		  	    player = OBJECTSYSTEM->ptrPlayer;
		  	    if (player && player->Health)
		  	    {
		  		    player->Health->ToggleInvulnerability();
		  	    }
            break;

          case GLFW_KEY_N: // Goto NextLevel: Shift + N
		  	    std::cout << "Goto NextLevel Cheat!" << std::endl;
            OBJECTSYSTEM->NextLevel();
            break;

          case GLFW_KEY_H: // Enter Debug Mode: H
            OPENGL->ToggleDebugDraw();

            if (CharacterController::PLAYER)
            {
              CharacterController::PLAYER->ToggleFlying();
            }
            break;
        }
      }
    }
  }// namespace Cheats  
}// namespace Framework

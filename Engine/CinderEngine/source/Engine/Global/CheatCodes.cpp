<<<<<<< HEAD
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
      // Toggle Invincibility:   Shift + I
      if (e->SHIFTPressed && e->KeyDown && e->KeyValue == GLFW_KEY_I)
      {
        std::cout << "Toggle Invincibility Cheat!" << std::endl;

		GameObject* player = OBJECTSYSTEM->FindObjectByName(std::string("Player"));
		if (player != nullptr)
		{
			player->Health->ToggleInvulnerability();
		}

      }

      // Goto NextLevel: Shift + N
      if (e->SHIFTPressed && e->KeyDown && e->KeyValue == GLFW_KEY_N)
      {
        std::cout << "Goto NextLevel Cheat!" << std::endl;
		OBJECTSYSTEM->LoadLevel(CharacterController::PLAYER->gameObject->PlayerStats->NextLevel.c_str());
      }


      // Enter Debug Mode: Tilda Key ' ` '
      if (e->KeyDown && e->KeyValue == GLFW_KEY_H)
      {
        OPENGL->ToggleDebugDraw();

        if (OBJECTSYSTEM->ptrPlayer && OBJECTSYSTEM->ptrPlayer->CharacterController)
        {
          OBJECTSYSTEM->ptrPlayer->CharacterController->ToggleFlying();
        }
      }
    }
  }// namespace Cheats  
}// namespace Framework
=======
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
      // Toggle Invincibility:   Shift + I
      if (e->SHIFTPressed && e->KeyDown && e->KeyValue == GLFW_KEY_I)
      {
        std::cout << "Toggle Invincibility Cheat!" << std::endl;

        OBJECTSYSTEM->FindObjectByID(5)->Health->ToggleInvulnerability();
      }

      // Goto NextLevel: Shift + N
      if (e->SHIFTPressed && e->KeyDown && e->KeyValue == GLFW_KEY_N)
      {
        std::cout << "Goto NextLevel Cheat!" << std::endl;

      }


      // Enter Debug Mode: Tilda Key ' ` '
      if (e->KeyDown && e->KeyValue == GLFW_KEY_H)
      {
        OPENGL->ToggleDebugDraw();

        if (CharacterController::PLAYER)
        {
          CharacterController::PLAYER->ToggleFlying();
        }
      }
    }
  }// namespace Cheats  
}// namespace Framework
>>>>>>> be0571b82dddc73c07ab5f9dc35d2ad7aa73454b
